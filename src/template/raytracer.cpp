#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "../Math/float3.h"
#include "../Data/Collision/AABB.h"
#include "../Data/Objects/ObjectId.h"
#include "../Data/Lights/PointLightData.h"
#include "../Data/Lights/AmbientLightData.h"
#include "../Data/Objects/Sphere.h"
#include "../Data/Objects/Mesh.h"
#include "../Data/Objects/Scene.h"
#include "../Math/float3u.h"
#include "../Data/Shading/Rgb.h"

using namespace std;
using namespace RayTracer;

typedef unsigned char RGB[3];

Rgb BackgroundColor = Rgb(0);
float ShadowRayEpsilon = 0.0f;
int MaxBounces = 0;
Scene scene;

void ConvertTemplateDataIntoSelfData(parser::p_scene& parseScene){
    auto sceneBg = parseScene.background_color;
    BackgroundColor = Rgb(float3(sceneBg.x, sceneBg.y, sceneBg.z));

    ShadowRayEpsilon = parseScene.shadow_ray_epsilon;

    MaxBounces = parseScene.max_recursion_depth;

    // TODO: Parse cameras here.

    auto ambientLight = parseScene.ambient_light;
    AmbientLightData ambientLightData;
    ambientLightData.Radiance = float3(ambientLight.x, ambientLight.y, ambientLight.z);

    auto pointLightCount = parseScene.point_lights.size();
    auto pointLights = new PointLightData[pointLightCount];
    for (int i = 0; i < pointLightCount; ++i) {
        auto& parseLight = parseScene.point_lights[i];
        auto& light = pointLights[i];
        auto intensity = parseLight.intensity;
        auto position = parseLight.position;
        light.Intensity = float3(intensity.x, intensity.y, intensity.z);
        light.Position = float3(position.x, position.y, position.z);
    }

    auto materialCount = parseScene.materials.size();
    auto materials = new MaterialData[materialCount];

    for (int i = 0; i < materialCount; ++i) {
        auto& parseMat = parseScene.materials[i];
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
    auto vertices = new float3[vertexCount];
    for (int i = 0; i < vertexCount; ++i) {
        auto& parseVertex = parseScene.vertex_data[i];
        vertices[i] = float3(parseVertex.x, parseVertex.y, parseVertex.z);
    }

    // If the ID is 1, it corresponds to 0th index, so decrement by one
    auto sphereCount = parseScene.spheres.size();
    auto spheres = new Sphere[sphereCount];
    auto sphereMaterials = new MaterialData[sphereCount];

    for (int i = 0; i < sphereCount; ++i) {
        auto& parseSphere = parseScene.spheres[i];
        auto& sphere = spheres[i];
        sphere.RadiusSquared = parseSphere.radius * parseSphere.radius;
        sphere.Center = vertices[parseSphere.center_vertex_id - 1];
        sphereMaterials[i] = materials[parseSphere.material_id - 1];
    }

    auto triangleCount = parseScene.triangles.size();
    auto triangles = new Triangle[triangleCount];
    auto triangleMaterials = new MaterialData[triangleCount];
    auto triangleNormals = new float3[triangleCount];
    for (int i = 0; i < triangleCount; ++i) {
        auto& parseTriangle = parseScene.triangles[i];
        auto v0 = vertices[parseTriangle.indices.v0_id - 1];
        auto v1 = vertices[parseTriangle.indices.v1_id - 1];
        auto v2 = vertices[parseTriangle.indices.v2_id - 1];
        triangles[i].Vertex0 = float3(v0.x, v0.y, v0.z);
        triangles[i].Vertex1 = float3(v1.x, v1.y, v1.z);
        triangles[i].Vertex2 = float3(v2.x, v2.y, v2.z);
        triangleNormals[i] = triangles[i].GetNormal();
        triangleMaterials[i] = materials[parseTriangle.material_id - 1];
    }

    auto meshCount = parseScene.meshes.size();
    auto meshes = new Mesh[meshCount];
    for (int i = 0; i < meshCount; ++i) {
        auto& parseMesh = parseScene.meshes[i];
        auto& mesh = meshes[i];
        auto meshTriangles = parseMesh.faces.size();
        mesh.MaterialData = materials[parseMesh.material_id - 1];
        mesh.TriangleCount = meshTriangles;
        mesh.Triangles = new Triangle[meshTriangles];
        mesh.TriangleNormals = new float3[meshTriangles];

        for (int j = 0; j < meshTriangles; ++j) {
            auto& parseTriangle = parseMesh.faces[j];
            auto& meshTriangle = mesh.Triangles[j];
            auto v0 = vertices[parseTriangle.v0_id - 1];
            auto v1 = vertices[parseTriangle.v1_id - 1];
            auto v2 = vertices[parseTriangle.v2_id - 1];
            meshTriangle.Vertex0 = float3(v0.x, v0.y, v0.z);
            meshTriangle.Vertex1 = float3(v1.x, v1.y, v1.z);
            meshTriangle.Vertex2 = float3(v2.x, v2.y, v2.z);
            mesh.TriangleNormals[j] = triangles[i].GetNormal();
        }
    }

    MeshData meshData;
    meshData.Meshes = meshes;

    TriangleData triangleData;
    triangleData.Triangles = triangles;
    triangleData.Count = triangleCount;
    triangleData.Materials = triangleMaterials;
    triangleData.Normals = triangleNormals;

    SphereData sphereData;
    sphereData.Count = sphereCount;
    sphereData.Materials = sphereMaterials;
    sphereData.Spheres = spheres;

    scene.SphereData = sphereData;
    scene.MeshData = meshData;
    scene.TriangleData = triangleData;

    scene.PointLightCount = pointLightCount;
    scene.PointLights = pointLights;

    scene.AmbientLight = ambientLightData;

    // TODO: Mesh AABB
    // TODO: Scene AABB

    delete[] materials;
    delete[] vertices;
}

void FreeResources(){
    delete[] scene.PointLights;
    delete[] scene.TriangleData.Triangles;
    delete[] scene.TriangleData.Materials;
    delete[] scene.TriangleData.Normals;
    delete[] scene.SphereData.Materials;
    delete[] scene.SphereData.Spheres;

    for(int i = 0; i < scene.MeshData.Count; i++){
        delete[] scene.MeshData.Meshes[i].Triangles;
        delete[] scene.MeshData.Meshes[i].TriangleNormals;
    }

    delete[] scene.MeshData.Meshes;
}

int main(int argc, char *argv[]) {

    cout << "Running Raytracer!" << endl;

    // Sample usage for reading an XML scene file
    parser::p_scene scene;

    cout << "Last arg:" << endl;
    cout << argv[argc - 1] << endl;

    auto lastArg = argv[argc - 1];

    // TODO-Use the correct argument here for the final build!
    scene.loadFromXml(lastArg);

    ConvertTemplateDataIntoSelfData(scene);

    cout << "This is after scene load!" << endl;

    // The code below creates a test pattern and writes
    // it to a PPM file to demonstrate the usage of the
    // ppm_write function.
    //
    // Normally, you would be running your ray tracing
    // code here to produce the desired image.

    const RGB BAR_COLOR[8] =
            {
                    {255, 255, 255},  // 100% White
                    {255, 255, 0},  // Yellow
                    {0,   255, 255},  // Cyan
                    {0,   255, 0},  // Green
                    {255, 0,   255},  // Magenta
                    {255, 0,   0},  // Red
                    {0,   0,   255},  // Blue
                    {0,   0,   0},  // Black
            };

    int width = 640, height = 480;
    int columnWidth = width / 8;

    unsigned char *image = new unsigned char[width * height * 3];

    int i = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int colIdx = x / columnWidth;
            image[i++] = BAR_COLOR[colIdx][0];
            image[i++] = BAR_COLOR[colIdx][1];
            image[i++] = BAR_COLOR[colIdx][2];
        }
    }

    cout << "This is before write ppm!" << endl;

    write_ppm("test.ppm", image, width, height);

    cout << "Finished!" << endl;

}
