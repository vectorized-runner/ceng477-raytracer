
#ifndef CENG477_RAYTRACER_MATH_H
#define CENG477_RAYTRACER_MATH_H

#include "float3.h"
#include <cmath>

namespace RayTracer {

    struct Math {
        // TODO: Unity is lhs, ensure this is correct
        static float3 cross(float3 x, float3 y) {
            return (x * y.yzx() - x.yzx() * y).yzx();
        }

        static float3 length(float3 v) {
            return sqrt(dot(v, v));
        }

        static float dot(float3 x, float3 y) {
            return x.x * y.x + x.y * y.y + x.z * y.z;
        }

        static float min(float x, float y) {
            return x < y ? x : y;
        }

        static float3 min(float3 x, float3 y) {
            return float3(min(x.x, y.x), min(x.y, y.y), min(x.z, y.z));
        }

        static float max(float x, float y) {
            return x > y ? x : y;
        }

        static float3 max(float3 x, float3 y) {
            return float3(max(x.x, y.x), max(x.y, y.y), max(x.z, y.z));
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_MATH_H
