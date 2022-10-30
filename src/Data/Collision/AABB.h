
#ifndef CENG477_RAYTRACER_AABB_H
#define CENG477_RAYTRACER_AABB_H

#include "../../Math/float3.h"
#include "../../Math/Math.h"

namespace RayTracer {

    struct AABB {
        float3 Min;
        float3 Max;

        __attribute__((always_inline))
        AABB() : Min(), Max() {
        }

        __attribute__((always_inline))
        AABB(float3 min, float3 max) {
            Min = min;
            Max = max;
        }

        __attribute__((always_inline))
        void Encapsulate(float3 point) {
            Min = Math::Min(point, Min);
            Max = Math::Max(point, Max);
        }

        __attribute__((always_inline))
        void Encapsulate(AABB other) {
            Min = Math::Min(Min, other.Min);
            Max = Math::Max(Max, other.Max);
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_AABB_H
