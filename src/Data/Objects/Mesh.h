
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
        AABB AABB;
        MaterialData MaterialData;
    };

} // RayTracer

#endif //CENG477_RAYTRACER_MESH_H
