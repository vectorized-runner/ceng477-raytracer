
#ifndef CENG477_RAYTRACER_MESH_H
#define CENG477_RAYTRACER_MESH_H

#include "Triangle.h"
#include "../Shading/MaterialData.h"
#include "../Collision/AABB.h"

namespace RayTracer {

    struct Mesh {
        int TriangleCount;
        Triangle* Triangles;
        float3* TriangleNormals;
        AABB SelfAABB;
        char MaterialIndex;

        void CalculateAABB() {
            auto aabb = AABB();
            aabb.Min = Math::FloatMax;
            aabb.Max = Math::FloatMin;

            for (int i = 0; i < TriangleCount; ++i) {
                aabb.Encapsulate(Triangles[i].Vertex0);
                aabb.Encapsulate(Triangles[i].Vertex1);
                aabb.Encapsulate(Triangles[i].Vertex2);
            }

            SelfAABB = aabb;
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_MESH_H
