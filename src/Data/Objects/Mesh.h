
#ifndef CENG477_RAYTRACER_MESH_H
#define CENG477_RAYTRACER_MESH_H

#include "Triangle.h"
#include "../Shading/MaterialData.h"
#include "../Collision/AABB.h"

namespace RayTracer {

    struct Mesh {
        Triangle* Triangles;
        float3* TriangleNormals;
        MaterialData MaterialData;
        AABB AABB;
    };

} // RayTracer

#endif //CENG477_RAYTRACER_MESH_H
