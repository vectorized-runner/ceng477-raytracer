
#ifndef CENG477_RAYTRACER_FLOAT3_H
#define CENG477_RAYTRACER_FLOAT3_H

#include <string>

using namespace std;

namespace RayTracer {

    struct float3 {
        float x;
        float y;
        float z;

        float3(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        string ToString() {
            return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
        }
    };
} // RayTracer

#endif //CENG477_RAYTRACER_FLOAT3_H
