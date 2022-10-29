
#ifndef CENG477_RAYTRACER_MATH_H
#define CENG477_RAYTRACER_MATH_H

#include "float3.h"
#include "../Data/Collision/Ray.h"
#include "../Data/Collision/AABB.h"
#include <cmath>

namespace RayTracer {

    struct Math {

        constexpr static const float floatMax = numeric_limits<float>::max();
        constexpr static const float floatMin = numeric_limits<float>::min();
        constexpr static const float Epsilon = 0.0001f;

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

        static float3 rcp(float3 x) {
            return 1.0f / x;
        }


    };

} // RayTracer

#endif //CENG477_RAYTRACER_MATH_H
