
#ifndef CENG477_RAYTRACER_SCENE_H
#define CENG477_RAYTRACER_SCENE_H

#include "TriangleData.h"
#include "MeshData.h"
#include "SphereData.h"
#include "../Lights/PointLightData.h"
#include "../Lights/AmbientLight.h"
#include "../../Math/float3u.h"
#include "../../Debug/Debug.h"
#include "../Collision/IntersectionResult.h"

using namespace std;

namespace RayTracer {

    struct Scene {
        TriangleData TriangleData;
        MeshData MeshData;
        SphereData SphereData;
        PointLightData PointLights;
        AmbientLight AmbientLight;
        AABB SelfAABB;
        MaterialData* Materials;

        void CalculateAABB() {
            auto aabb = AABB();
            aabb.Min = Math::FloatMax;
            aabb.Max = Math::FloatMin;

            for (int i = 0; i < MeshData.Count; ++i) {
                aabb.Encapsulate(MeshData.Meshes[i].SelfAABB);
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

            SelfAABB = aabb;
        }

        static bool RayAABBIntersection(Ray ray, const AABB box) {
            auto inverseDir = float3u(Math::Rcp(ray.Direction));
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

        static bool RayTriangleIntersection(Ray ray, Triangle triangle, float& intersectionDistance) {
            auto vertex0 = triangle.Vertex0;
            auto vertex1 = triangle.Vertex1;
            auto vertex2 = triangle.Vertex2;
            auto edge1 = vertex1 - vertex0;
            auto edge2 = vertex2 - vertex0;
            auto h = Math::Cross(ray.Direction, edge2);
            auto a = Math::Dot(edge1, h);
            auto epsilon = Math::Epsilon;

            // No, this fails for some models
//            if (a > -epsilon && a < epsilon) {
//                intersectionDistance = 0.0f;
//                return false;
//            }

            auto f = 1.0f / a;
            auto s = ray.Origin - vertex0;
            auto u = f * Math::Dot(s, h);
            if (u < 0.0f || u > 1.0f) {
                intersectionDistance = 0.0f;
                return false;
            }

            auto q = Math::Cross(s, edge1);
            auto v = f * Math::Dot(ray.Direction, q);
            if (v < 0.0f || u + v > 1.0f) {
                intersectionDistance = 0.0f;
                return false;
            }

            auto t = f * Math::Dot(edge2, q);
            if (t > epsilon)
            {
                intersectionDistance = t;
                return true;
            }

            intersectionDistance = 0.0f;
            return false;
        }

        static bool RaySphereIntersection(Ray ray, Sphere sphere, float& closestIntersectionDistance) {
            Debug::Assert(Math::IsNormalized(ray.Direction), "Ray isn't normalized.");

            auto oc = ray.Origin - sphere.Center;
            auto uoc = Math::Dot(ray.Direction, oc);
            auto discriminant = uoc * uoc - (Math::LengthSq(oc) - sphere.RadiusSquared);

            if (discriminant < 0) {
                closestIntersectionDistance = 0.0f;
                return false;
            }

            // Ignore discriminant == 0 because it won't practically happen
            auto sqrtDiscriminant = sqrt(discriminant);
            auto bigRoot = -uoc + sqrtDiscriminant;

            if (bigRoot < 0) {
                closestIntersectionDistance = 0.0f;
                return false;
            }

            auto smallRoot = -uoc - sqrtDiscriminant;
            closestIntersectionDistance = smallRoot < 0 ? bigRoot : smallRoot;
            return true;
        }

        IntersectionResult IntersectRay(Ray ray) const {
            auto smallestIntersectionDistance = Math::FloatMax;
            auto hitObject = ObjectId(ObjectType::None, -1, -1);

            // If ray doesn't intersect with Scene AABB, there's no need to check any object
            if (!RayAABBIntersection(ray, SelfAABB)) {
                return IntersectionResult(hitObject, smallestIntersectionDistance);
            }

            for (int meshIndex = 0; meshIndex < MeshData.Count; meshIndex++) {
                auto& mesh = MeshData.Meshes[meshIndex];
                if (RayAABBIntersection(ray, mesh.SelfAABB)) {
                    for (auto triIndex = 0; triIndex < mesh.TriangleCount; triIndex++) {
                        auto& triangle = mesh.Triangles[triIndex];
                        auto intersectionDistance = 0.0f;

                        if (RayTriangleIntersection(ray, triangle, intersectionDistance)) {
                            if (smallestIntersectionDistance > intersectionDistance) {
                                smallestIntersectionDistance = intersectionDistance;
                                hitObject.Type = ObjectType::MeshTriangleObject;
                                hitObject.Index = triIndex;
                                hitObject.MeshIndex = meshIndex;
                            }
                        }
                    }
                }
            }

            for (auto sphereIndex = 0; sphereIndex < SphereData.Count; sphereIndex++) {
                auto sphere = SphereData.Spheres[sphereIndex];
                auto closestIntersectionDistance = 0.0f;

                if (RaySphereIntersection(ray, sphere, closestIntersectionDistance)) {
                    if (smallestIntersectionDistance > closestIntersectionDistance) {
                        smallestIntersectionDistance = closestIntersectionDistance;
                        hitObject.Type = ObjectType::SphereObject;
                        hitObject.Index = sphereIndex;
                    }
                }
            }

            for (auto triIndex = 0; triIndex < TriangleData.Count; triIndex++) {
                auto& triangle = TriangleData.Triangles[triIndex];
                auto intersectionDistance = 0.0f;

                if (RayTriangleIntersection(ray, triangle, intersectionDistance)) {
                    if (smallestIntersectionDistance > intersectionDistance) {
                        smallestIntersectionDistance = intersectionDistance;
                        hitObject.Type = ObjectType::TriangleObject;
                        hitObject.Index = triIndex;
                    }
                }
            }

            return IntersectionResult(hitObject, smallestIntersectionDistance);
        }
    };


} // RayTracer

#endif //CENG477_RAYTRACER_SCENE_H
