
#ifndef CENG477_RAYTRACER_SCENE_H
#define CENG477_RAYTRACER_SCENE_H

#include "TriangleData.h"
#include "MeshData.h"
#include "SphereData.h"
#include "../Lights/PointLightData.h"
#include "../Lights/AmbientLightData.h"
#include "../../Math/float3u.h"

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

        // TODO-Port:
        static bool RayTriangleIntersection(Ray ray, Triangle triangle, float &intersectionDistance) {
            auto vertex0 = triangle.Vertex0;
            auto vertex1 = triangle.Vertex1;
            auto vertex2 = triangle.Vertex2;
            auto edge1 = vertex1 - vertex0;
            auto edge2 = vertex2 - vertex0;
            auto h = Math::cross(ray.Direction, edge2);
            auto a = Math::dot(edge1, h);
            auto epsilon = Math::Epsilon;

            if (a > -epsilon && a < epsilon) {
                intersectionDistance = 0.0f;
                return false; // This ray is parallel to this triangle.
            }

            auto f = 1.0f / a;
            auto s = ray.Origin - vertex0;
            auto u = f * Math::dot(s, h);
            if (u < 0.0f || u > 1.0f) {
                intersectionDistance = 0.0f;
                return false;
            }

            auto q = Math::cross(s, edge1);
            auto v = f * Math::dot(ray.Direction, q);
            if (v < 0.0f || u + v > 1.0f) {
                intersectionDistance = 0.0f;
                return false;
            }

            // At this stage we can compute t to find out where the intersection point is on the line.
            auto t = f * Math::dot(edge2, q);
            if (t > epsilon) // ray intersection
            {
                intersectionDistance = t;
                return true;
            }

            // This means that there is a line intersection but not a ray intersection.
            intersectionDistance = 0.0f;
            return false;
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_SCENE_H
