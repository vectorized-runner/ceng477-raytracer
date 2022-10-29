
#ifndef CENG477_RAYTRACER_AABB_H
#define CENG477_RAYTRACER_AABB_H

#include "../../float3.h"
#include "../../Math/Math.h"

namespace RayTracer {

    struct AABB {
        float3 Min;
        float3 Max;

        void Encapsulate(float3 point) {
            Min = Math::min(point, Min);
            Max = Math::max(point, Max);
        }

        void Encapsulate(AABB other) {
            Min = Math::min(Min, other.Min);
            Max = Math::max(Max, other.Max);
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_AABB_H
