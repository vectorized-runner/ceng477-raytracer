
#ifndef CENG477_RAYTRACER_RAY_H
#define CENG477_RAYTRACER_RAY_H

#include "../../Math/float3.h"
#include <string>

using namespace std;

namespace RayTracer {

    struct Ray {
        float3 Origin;
        float3 Direction;

        Ray(float3 origin, float3 direction) {
            Origin = origin;
            Direction = direction;
        }

        float3 GetPoint(float distance) {
            return Origin + Direction * distance;
        }

        string ToString() {
            return "Origin: " + Origin.ToString() + ", Direction: " + Direction.ToString();
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_RAY_H
