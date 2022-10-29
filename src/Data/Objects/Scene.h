
#ifndef CENG477_RAYTRACER_SCENE_H
#define CENG477_RAYTRACER_SCENE_H

#include "TriangleData.h"
#include "MeshData.h"
#include "SphereData.h"
#include "../Lights/PointLightData.h"
#include "../Lights/AmbientLightData.h"
#include "../Math/float3u.h"

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

        // TODO-Port:
        static bool RayAABBIntersection(Ray ray, const AABB box) {
            auto inverseDir = float3u(Math::rcp(ray.Direction));
            auto tmin = 0.0f;
            auto tmax = INFINITY;
            auto boxMin = float3u(box.Min);
            auto boxMax = float3u(box.Max);
            auto origin = float3u(ray.Origin);

            for (int i = 0; i < 3; ++i) {
                auto t1 = (boxMin.arr[i] - origin.arr[i]) * inverseDir.arr[i];
                auto t2 = (boxMax.arr[i] - origin.arr[i]) * inverseDir.arr[i];
                tmin = min(max(t1, tmin), max(t2, tmin));
                tmax = max(min(t1, tmax), min(t2, tmax));
            }

            return tmin <= tmax;
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_SCENE_H
