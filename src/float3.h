
#ifndef CENG477_RAYTRACER_FLOAT3_H
#define CENG477_RAYTRACER_FLOAT3_H

#include <string>

using namespace std;

// TODO: Move this to Math directory

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

        float3 operator+(float3 other) const {
            return float3(x + other.x, y + other.y, z + other.z);
        }

        float3 operator-(float3 other) const {
            return float3(x - other.x, y - other.y, z - other.z);
        }

        float3 operator*(float3 other) const {
            return float3(x * other.x, y * other.y, z * other.z);
        }

        float3 operator*(float other) const {
            return float3(x * other, y * other, z * other);
        }

        string ToString() const {
            return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
        }
    };
} // RayTracer

#endif //CENG477_RAYTRACER_FLOAT3_H
