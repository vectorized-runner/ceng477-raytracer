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
    scene.AmbientLight.Radiance = float3(ambientLight.x, ambientLight.y, ambientLight.z);

    auto pointLightCount = parseScene.point_lights.size();
    scene.PointLights = new PointLightData[pointLightCount];
    for (int i = 0; i < pointLightCount; ++i) {
        auto& parseLight = parseScene.point_lights[i];
        auto& light = scene.PointLights[i];
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

    // TODO: Vertex
    // TODO: Meshes
    // TODO: Triangles
    // TODO: Spheres

    delete[] materials;

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
