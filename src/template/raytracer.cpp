#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "../Math/float3.h"
#include "../Data/Collision/AABB.h"
#include "../Data/Objects/ObjectId.h"
#include "../Data/Lights/PointLight.h"
#include "../Data/Lights/AmbientLight.h"
#include "../Data/Objects/Sphere.h"
#include "../Data/Objects/Mesh.h"
#include "../Data/Objects/Scene.h"
#include "../Data/Shading/Rgb.h"
#include "../Data/Camera/CameraData.h"
#include "../Data/Camera/ImagePlane.h"
#include <chrono>

using namespace std;
using namespace RayTracer;
using namespace chrono;

typedef unsigned char RGB[3];

Rgb BackgroundColor = Rgb(0);
float ShadowRayEpsilon = 0.0f;
int MaxBounces = 0;
Scene scene;
int CameraCount;
CameraData* cameras;
ImagePlane* imagePlanes;
Rgb** outputColors;
string* outputFileNames;

void ConvertTemplateDataIntoSelfData(parser::p_scene& parseScene) {
    auto sceneBg = parseScene.background_color;
    BackgroundColor = Rgb(float3(sceneBg.x, sceneBg.y, sceneBg.z));

    ShadowRayEpsilon = parseScene.shadow_ray_epsilon;
    Debug::Assert(ShadowRayEpsilon > 0.0f, "ShadowRayEpsilon");

    MaxBounces = parseScene.max_recursion_depth;
    Debug::Assert(MaxBounces > 0, "MaxBounces");

    CameraCount = parseScene.cameras.size();
    Debug::Assert(CameraCount > 0, "CameraCount");

    cameras = new CameraData[CameraCount];
    imagePlanes = new ImagePlane[CameraCount];
    outputFileNames = new string[CameraCount];
    outputColors = new Rgb* [CameraCount];

    for (int i = 0; i < CameraCount; ++i) {
        const auto& parseCam = parseScene.cameras[i];
        auto& camera = cameras[i];
        auto& plane = imagePlanes[i];
        camera.Position = float3(parseCam.position.x, parseCam.position.y, parseCam.position.z);
        camera.Up = float3(parseCam.up.x, parseCam.up.y, parseCam.up.z);
        camera.Forward = float3(parseCam.gaze.x, parseCam.gaze.y, parseCam.gaze.z);
        camera.Right = Math::Cross(camera.Up, -camera.Forward);

        plane.Resolution = Resolution(parseCam.image_width, parseCam.image_height);
        plane.DistanceToCamera = parseCam.near_distance;

        auto pixelCount = plane.Resolution.X * plane.Resolution.Y;
        outputColors[i] = new Rgb[pixelCount];

        auto left = parseCam.near_plane.x;
        auto right = parseCam.near_plane.y;
        auto top = parseCam.near_plane.z;
        auto bottom = parseCam.near_plane.w;

        Debug::Assert(abs(left) == abs(right), "ImagePlane");
        Debug::Assert(abs(top) == abs(bottom), "ImagePlane");

        plane.HalfHorizontalLength = abs(left);
        plane.HalfVerticalLength = abs(top);

        outputFileNames[i] = parseCam.image_name;
    }

    auto ambientLight = parseScene.ambient_light;
    AmbientLight ambientLightData;
    ambientLightData.Radiance = float3(ambientLight.x, ambientLight.y, ambientLight.z);

    auto pointLightCount = parseScene.point_lights.size();
    Debug::Assert(pointLightCount > 0, "PointLightCount");

    auto pointLights = new PointLight[pointLightCount];
    for (int i = 0; i < pointLightCount; ++i) {
        const auto& parseLight = parseScene.point_lights[i];
        auto& light = pointLights[i];
        auto intensity = parseLight.intensity;
        auto position = parseLight.position;
        light.Intensity = float3(intensity.x, intensity.y, intensity.z);
        light.Position = float3(position.x, position.y, position.z);
    }

    auto materialCount = parseScene.materials.size();
    Debug::Assert(materialCount > 0, "MaterialCount");

    auto materials = new MaterialData[materialCount];

    for (int i = 0; i < materialCount; ++i) {
        const auto& parseMat = parseScene.materials[i];
        auto& material = materials[i];
        auto ambient = parseMat.ambient;
        auto diffuse = parseMat.diffuse;
        auto specular = parseMat.specular;
        auto mirror = parseMat.mirror;
        auto phongExponent = parseMat.phong_exponent;
        auto isMirror = parseMat.is_mirror;

        material.AmbientReflectance = float3(ambient.x, ambient.y, ambient.z);
        material.DiffuseReflectance = float3(diffuse.x, diffuse.y, diffuse.z);
        material.SpecularReflectance = float3(specular.x, specular.y, specular.z);
        material.MirrorReflectance = float3(mirror.x, mirror.y, mirror.z);
        material.PhongExponent = phongExponent;
        material.IsMirror = isMirror;
    }

    auto vertexCount = parseScene.vertex_data.size();
    Debug::Assert(vertexCount > 0, "VertexCount");

    auto vertices = new float3[vertexCount];
    for (int i = 0; i < vertexCount; ++i) {
        const auto& parseVertex = parseScene.vertex_data[i];
        vertices[i] = float3(parseVertex.x, parseVertex.y, parseVertex.z);
    }

    // If the ID is 1, it corresponds to 0th index, so decrement by one
    auto sphereCount = parseScene.spheres.size();
    if (sphereCount > 0) {
        auto spheres = new Sphere[sphereCount];
        auto sphereMaterialIndices = new char[sphereCount];

        for (int i = 0; i < sphereCount; ++i) {
            const auto& parseSphere = parseScene.spheres[i];
            auto& sphere = spheres[i];
            sphere.RadiusSquared = parseSphere.radius * parseSphere.radius;
            sphere.Center = vertices[parseSphere.center_vertex_id - 1];
            sphereMaterialIndices[i] = (char)(parseSphere.material_id - 1);
        }

        SphereData sphereData;
        sphereData.Count = sphereCount;
        sphereData.MaterialIndices = sphereMaterialIndices;
        sphereData.Spheres = spheres;

        scene.SphereData = sphereData;
    }

    auto triangleCount = parseScene.triangles.size();
    if (triangleCount > 0) {
        auto triangles = new Triangle[triangleCount];
        auto triangleMaterialIndices = new char[triangleCount];
        auto triangleNormals = new float3[triangleCount];
        for (int i = 0; i < triangleCount; ++i) {
            const auto& parseTriangle = parseScene.triangles[i];
            auto v0 = vertices[parseTriangle.indices.v0_id - 1];
            auto v1 = vertices[parseTriangle.indices.v1_id - 1];
            auto v2 = vertices[parseTriangle.indices.v2_id - 1];
            triangles[i].Vertex0 = float3(v0.x, v0.y, v0.z);
            triangles[i].Vertex1 = float3(v1.x, v1.y, v1.z);
            triangles[i].Vertex2 = float3(v2.x, v2.y, v2.z);
            triangleNormals[i] = triangles[i].GetNormal();
            triangleMaterialIndices[i] = (char)(parseTriangle.material_id - 1);
        }

        TriangleData triangleData;
        triangleData.Count = triangleCount;
        triangleData.Triangles = triangles;
        triangleData.MaterialIndices = triangleMaterialIndices;
        triangleData.Normals = triangleNormals;

        scene.TriangleData = triangleData;
    }

    auto meshCount = parseScene.meshes.size();
    if(meshCount > 0){
        auto meshes = new Mesh[meshCount];
        for (int i = 0; i < meshCount; ++i) {
            const auto& parseMesh = parseScene.meshes[i];
            auto& mesh = meshes[i];
            auto meshTriangles = parseMesh.faces.size();
            mesh.MaterialIndex = (char)(parseMesh.material_id - 1);
            mesh.TriangleCount = meshTriangles;
            mesh.Triangles = new Triangle[meshTriangles];
            mesh.TriangleNormals = new float3[meshTriangles];

            for (int j = 0; j < meshTriangles; ++j) {
                const auto& parseTriangle = parseMesh.faces[j];
                auto& meshTriangle = mesh.Triangles[j];
                auto& meshNormal = mesh.TriangleNormals[j];
                auto v0 = vertices[parseTriangle.v0_id - 1];
                auto v1 = vertices[parseTriangle.v1_id - 1];
                auto v2 = vertices[parseTriangle.v2_id - 1];
                meshTriangle.Vertex0 = float3(v0.x, v0.y, v0.z);
                meshTriangle.Vertex1 = float3(v1.x, v1.y, v1.z);
                meshTriangle.Vertex2 = float3(v2.x, v2.y, v2.z);
                meshNormal = meshTriangle.GetNormal();
            }

            mesh.CalculateAABB();
        }

        MeshData meshData;
        meshData.Count = meshCount;
        meshData.Meshes = meshes;

        scene.MeshData = meshData;
    }

    scene.PointLights.Count = pointLightCount;
    scene.PointLights.PointLights = pointLights;

    scene.AmbientLight = ambientLightData;

    scene.Materials = materials;

    scene.CalculateAABB();

    delete[] vertices;
}


Rgb
CalculateDiffuse(float3 receivedIrradiance, float3 diffuseReflectance, float3 surfaceNormal, float3 lightDirection) {
    // Debug::Assert(receivedIrradiance >= 0.0f, "Irradiance");
    Debug::Assert(Math::IsNormalized(surfaceNormal), "SurfaceNormal");
    Debug::Assert(Math::IsNormalized(lightDirection), "LightDirection");

    auto cosNormalAndLightDir = Math::Max(0, Math::Dot(lightDirection, surfaceNormal));
    return Rgb(diffuseReflectance * cosNormalAndLightDir * receivedIrradiance);
}

Ray Reflect(float3 surfacePoint, float3 surfaceNormal, float3 rayDirection) {
    auto newRayOrigin = surfacePoint + surfaceNormal * ShadowRayEpsilon;
    auto newNormal = -rayDirection + (2 * Math::Dot(rayDirection, surfaceNormal) * surfaceNormal);
    return Ray(newRayOrigin, newNormal);
}

Rgb CalculateSpecular(float3 lightDirection, float3 rayDirection, float3 surfaceNormal,
                      float3 specularReflectance, float3 receivedIrradiance, float phongExponent) {
    Debug::Assert(Math::IsNormalized(lightDirection), "LightDir");
    Debug::Assert(Math::IsNormalized(rayDirection), "RayDir");
    Debug::Assert(Math::IsNormalized(surfaceNormal), "SurfaceNormal");

    auto lightDotNormal = Math::Dot(lightDirection, surfaceNormal);
    // Angle works like this since both vectors are normalized
    auto angle = Math::Degrees(Math::Acos(lightDotNormal));
    // If this assertion fails, take the absolute of angle
    Debug::Assert(angle > 0.0f, "SpecularAngle");

    // Light is coming from behind the surface
    if (angle > 90.0f) {
        return Rgb(0);
    }

    auto v = lightDirection + rayDirection;
    auto halfwayVector = v / Math::Length(v);
    Debug::Assert(Math::IsNormalized(halfwayVector), "Halfway");

    auto cosNormalAndHalfway = Math::Max(0, Math::Dot(surfaceNormal, halfwayVector));
    return Rgb(specularReflectance * pow(cosNormalAndHalfway, phongExponent) * receivedIrradiance);
}


float3 GetSphereNormal(float3 surfacePoint, int index) {
    auto sphere = scene.SphereData.Spheres[index];
    auto surfaceNormal = Math::Normalize(surfacePoint - sphere.Center);
    return surfaceNormal;
}

void GetSurfaceNormalAndMaterial(float3 surfacePoint, ObjectId id, float3& outNormal, MaterialData& outMaterial) {
    switch (id.Type) {
        case ObjectType::SphereObject: {
            outNormal = GetSphereNormal(surfacePoint, id.Index);
            outMaterial = scene.Materials[scene.SphereData.MaterialIndices[id.Index]];
            return;
        }
        case ObjectType::TriangleObject: {
            outNormal = scene.TriangleData.Normals[id.Index];
            outMaterial = scene.Materials[scene.TriangleData.MaterialIndices[id.Index]];
            return;
        }
        case ObjectType::MeshTriangleObject: {
            outNormal = scene.MeshData.Meshes[id.MeshIndex].TriangleNormals[id.Index];
            outMaterial = scene.Materials[scene.MeshData.Meshes[id.MeshIndex].MaterialIndex];
            return;
        }
        case ObjectType::None: {
            Debug::Log("Error: None reached.");
            return;
        }
    }
}

Rgb CalculateAmbient(float3 ambientReflectance, float3 ambientRadiance) {
    return Rgb(ambientRadiance * ambientReflectance);
}

Rgb Shade(Ray pixelRay, int currentRayBounce) {
    Debug::Assert(Math::IsNormalized(pixelRay.Direction), "PixelRayNormalize");

    auto hitResult = scene.IntersectRay(pixelRay);
    auto pixelRayHitObject = hitResult.ObjectId;
    if (pixelRayHitObject.Type == ObjectType::None) {
        return BackgroundColor;
    }

    Debug::Assert(pixelRayHitObject.Type != ObjectType::None, "PixelRayHit");

    auto surfacePoint = pixelRay.GetPoint(hitResult.Distance);
    float3 surfaceNormal;
    MaterialData material;
    GetSurfaceNormalAndMaterial(surfacePoint, pixelRayHitObject, surfaceNormal, material);

    auto color = CalculateAmbient(material.AmbientReflectance, scene.AmbientLight.Radiance);
    auto rayDirection = -pixelRay.Direction;

    for (int i = 0; i < scene.PointLights.Count; ++i) {
        const auto pointLight = scene.PointLights.PointLights[i];
        const auto lightPosition = pointLight.Position;
        const auto lightDirection = Math::Normalize(lightPosition - surfacePoint);
        const auto shadowRayOrigin = surfacePoint + surfaceNormal * ShadowRayEpsilon;
        const auto shadowRay = Ray(shadowRayOrigin, lightDirection);
        const auto shadowRayHitResult = scene.IntersectRay(shadowRay);
        const auto lightDistanceSq = Math::DistanceSq(surfacePoint, lightPosition);

        // TODO-Optimize: We can remove this branch, if ray-scene intersection returns infinite distance by default
        if (shadowRayHitResult.ObjectId.Type != ObjectType::None) {
            const auto hitDistanceSq = shadowRayHitResult.Distance * shadowRayHitResult.Distance;
            if (hitDistanceSq < lightDistanceSq) {
                // Shadow ray intersects with an object before light, no contribution from this light
                continue;
            }
        }

        // Shadow ray hit this object again, shouldn't happen
        Debug::Assert(shadowRayHitResult.ObjectId != pixelRayHitObject, "ShadowRayHitSameObject");

        const auto receivedIrradiance = pointLight.Intensity / lightDistanceSq;
        const auto diffuseRgb = CalculateDiffuse(receivedIrradiance, material.DiffuseReflectance, surfaceNormal,
                                                 lightDirection);
        const auto specularRgb = CalculateSpecular(lightDirection, rayDirection, surfaceNormal,
                                                   material.SpecularReflectance, receivedIrradiance,
                                                   material.PhongExponent);
        color = color + specularRgb + diffuseRgb;
    }

    if (material.IsMirror && currentRayBounce < MaxBounces) {
        auto reflectRay = Reflect(surfacePoint, surfaceNormal, rayDirection);
        auto mirrorReflectance = material.MirrorReflectance;
        color = color + Rgb(mirrorReflectance * Shade(reflectRay, currentRayBounce + 1).Value);
    }

    return color;
}

int GetPixelIndex(int px, int py, int resolutionX) {
    return px + py * resolutionX;
}


void CastPixelRays(CameraData cameraData, ImagePlane imagePlane, Rgb* colors) {
    auto resX = imagePlane.Resolution.X;
    auto resY = imagePlane.Resolution.Y;
    auto topLeft = imagePlane.GetRect(cameraData).TopLeft;
    auto up = cameraData.Up;
    auto right = cameraData.Right;
    auto horizontalLength = imagePlane.HorizontalLength();
    auto verticalLength = imagePlane.VerticalLength();
    auto cameraPosition = cameraData.Position;

    // Traverse order swapped for better cache usage
    // arrayIndex = x + y * resX
    for (int y = 0; y < resY; y++) {
        for (int x = 0; x < resX; x++) {
            auto rightMove = (x + 0.5f) * horizontalLength / resX;
            auto downMove = (y + 0.5f) * verticalLength / resY;
            auto pixelPosition = topLeft + rightMove * right - up * downMove;
            auto ray = Ray(cameraPosition, Math::Normalize(pixelPosition - cameraPosition));
            auto index = GetPixelIndex(x, y, resX);
            colors[index] = Shade(ray, 0);
        }
    }
}

int main(int argc, char* argv[]) {

    cout << "Running RayTracer program." << endl;

    auto fullBegin = steady_clock::now();

    // Sample usage for reading an XML scene file
    parser::p_scene parseScene;

    auto xmlPath = argv[argc - 1];
    auto lastArg = xmlPath;
    cout << "XML path is: " << xmlPath << endl;

    auto loadXmlBegin = steady_clock::now();
    parseScene.loadFromXml(lastArg);
    auto loadXmlEnd = steady_clock::now();
    Debug::LogElapsed("LoadXML took ", loadXmlBegin, loadXmlEnd);

    auto convertBegin = steady_clock::now();
    ConvertTemplateDataIntoSelfData(parseScene);
    auto convertEnd = steady_clock::now();
    Debug::LogElapsed("Converting Data took ", convertBegin, convertEnd);

    for (int i = 0; i < CameraCount; ++i) {
        auto rayTraceBegin = steady_clock::now();
        auto colors = outputColors[i];
        auto plane = imagePlanes[i];
        auto camera = cameras[i];
        auto pixelCountX = plane.Resolution.X;
        auto pixelCountY = plane.Resolution.Y;
        auto pixelCount = pixelCountX * pixelCountY;

        CastPixelRays(camera, plane, colors);
        auto* image = new unsigned char[pixelCount * 3];

        for (int j = 0; j < pixelCount; ++j) {
            const auto& color = colors[j];
            // Write rgb, round to nearest
            image[3 * j] = (unsigned char)round(Math::Clamp(color.Value.x, 0, 255));
            image[3 * j + 1] = (unsigned char)round(Math::Clamp(color.Value.y, 0, 255));
            image[3 * j + 2] = (unsigned char)round(Math::Clamp(color.Value.z, 0, 255));
        }

        auto rayTraceEnd = steady_clock::now();
        Debug::LogElapsed("Ray Tracing took ", rayTraceBegin, rayTraceEnd);

        auto writePPMBegin = steady_clock::now();
        write_ppm(outputFileNames[i].c_str(), image, pixelCountX, pixelCountY);
        auto writePPMEnd = steady_clock::now();
        Debug::LogElapsed("WritePPM took ", writePPMBegin, writePPMEnd);

        delete[] image;
    }

    auto fullEnd = chrono::steady_clock::now();

    Debug::LogElapsed("Program took ", fullBegin, fullEnd);
    cout << "Done!" << endl;

    return 0;
}
