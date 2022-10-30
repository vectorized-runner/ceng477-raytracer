
#ifndef CENG477_RAYTRACER_RAY_H
#define CENG477_RAYTRACER_RAY_H

#include "../../Math/float3.h"
#include <string>

using namespace std;

namespace RayTracer {

    struct Ray {
        float3 Origin;
        float3 Direction;

        __attribute__((always_inline))
        Ray(float3 origin, float3 direction) {
            Origin = origin;
            Direction = direction;
        }

        __attribute__((always_inline))
        float3 GetPoint(float distance) const {
            return Origin + Direction * distance;
        }

        __attribute__((always_inline))
        string ToString() const {
            return "Origin: " + Origin.ToString() + ", Direction: " + Direction.ToString();
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_RAY_H
