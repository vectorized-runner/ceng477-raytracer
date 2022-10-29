
#ifndef CENG477_RAYTRACER_TRIANGLEDATA_H
#define CENG477_RAYTRACER_TRIANGLEDATA_H

#include "Triangle.h"
#include "../Shading/MaterialData.h"

namespace RayTracer {

    struct TriangleData {
        Triangle* Triangles;
        float3* Normals;
        MaterialData* Materials;
        int Count;
    }; // RayTracer
}
#endif //CENG477_RAYTRACER_TRIANGLEDATA_H
