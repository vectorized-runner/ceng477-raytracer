
#ifndef CENG477_RAYTRACER_MATH_H
#define CENG477_RAYTRACER_MATH_H

#include "float3.h"
#include "../Data/Collision/Ray.h"
#include "../Data/Collision/AABB.h"
#include <cmath>

namespace RayTracer {

    struct Math {

        constexpr static const float FloatMax = numeric_limits<float>::max();
        constexpr static const float FloatMin = numeric_limits<float>::min();
        constexpr static const float Epsilon = 0.0001f;

        // TODO: Unity is lhs, ensure this is correct
        static float3 Cross(float3 x, float3 y) {
            return (x * y.yzx() - x.yzx() * y).yzx();
        }

        static float3 Length(float3 v) {
            return sqrt(Dot(v, v));
        }

        static float Dot(float3 x, float3 y) {
            return x.x * y.x + x.y * y.y + x.z * y.z;
        }

        static float Min(float x, float y) {
            return x < y ? x : y;
        }

        static float3 Min(float3 x, float3 y) {
            return float3(Min(x.x, y.x), Min(x.y, y.y), Min(x.z, y.z));
        }

        static float Max(float x, float y) {
            return x > y ? x : y;
        }

        static float3 Max(float3 x, float3 y) {
            return float3(Max(x.x, y.x), Max(x.y, y.y), Max(x.z, y.z));
        }

        static float3 Rcp(float3 x) {
            return 1.0f / x;
        }

        static float LengthSq(float3 v) {
            return Dot(v, v);
        }

        static bool IsLengthEqual(float3 v, float length) {
            return abs(LengthSq(v) - length * length) < Epsilon;
        }

        static bool IsNormalized(float3 v){
            return IsLengthEqual(v, 1.0f);
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_MATH_H
