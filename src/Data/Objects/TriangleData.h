
#ifndef CENG477_RAYTRACER_TRIANGLEDATA_H
#define CENG477_RAYTRACER_TRIANGLEDATA_H

#include "Triangle.h"
#include "../Shading/MaterialData.h"

namespace RayTracer {

    struct TriangleData {
        int Count;
        Triangle* Triangles;
        float3* Normals;
        char* MaterialIndices;
    }; // RayTracer
}
#endif //CENG477_RAYTRACER_TRIANGLEDATA_H
