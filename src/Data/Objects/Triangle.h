
#ifndef CENG477_RAYTRACER_TRIANGLE_H
#define CENG477_RAYTRACER_TRIANGLE_H

#include "../../Math/float3.h"
#include "../../Math/Math.h"

namespace RayTracer {

    struct Triangle {
        float3 Vertex0;
        float3 Vertex1;
        float3 Vertex2;

        float3 GetNormal() const {
            auto v = Math::Cross(Vertex2 - Vertex0, Vertex1 - Vertex0);
            return -v / Math::Length(v);
        }

        float3 GetCenter() const {
            return (Vertex0 + Vertex1 + Vertex2) / 3;
        }

    }; // RayTracer
}

#endif //CENG477_RAYTRACER_TRIANGLE_H
