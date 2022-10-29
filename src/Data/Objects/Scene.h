
#ifndef CENG477_RAYTRACER_SCENE_H
#define CENG477_RAYTRACER_SCENE_H

#include "TriangleData.h"
#include "MeshData.h"
#include "SphereData.h"
#include "../Lights/PointLightData.h"
#include "../Lights/AmbientLightData.h"

using namespace std;

namespace RayTracer {

    struct Scene {
        TriangleData TriangleData;
        MeshData MeshData;
        SphereData SphereData;

        PointLightData* PointLights;
        AmbientLightData AmbientLight;

        AABB selfAABB;

        void CalculateAABB() {
            auto aabb = AABB();
            aabb.Min = Math::floatMax;
            aabb.Max = Math::floatMin;

            for (int i = 0; i < MeshData.Count; ++i) {
                aabb.Encapsulate(MeshData.Meshes[i].AABB);
            }

            for (int i = 0; i < TriangleData.Count; ++i) {
                auto triangle = TriangleData.Triangles[i];
                aabb.Encapsulate(triangle.Vertex0);
                aabb.Encapsulate(triangle.Vertex1);
                aabb.Encapsulate(triangle.Vertex2);
            }

            for (int i = 0; i < SphereData.Count; ++i) {
                aabb.Encapsulate(SphereData.Spheres[i].GetAABB());
            }

            selfAABB = aabb;
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_SCENE_H
