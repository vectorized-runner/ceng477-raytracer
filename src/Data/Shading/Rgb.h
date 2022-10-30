
#ifndef CENG477_RAYTRACER_RGB_H
#define CENG477_RAYTRACER_RGB_H

#include "../../Math/float3.h"
#include <string>

using namespace std;

namespace RayTracer {

    // This RGB value is in [0, 255] range (it's not [0, 1])
    struct Rgb {

        float3 Value;

        __attribute__((always_inline))
        Rgb() {
            Value = 0;
        }

        __attribute__((always_inline))
        Rgb(float3 value) {
            Debug::Assert(Math::IsNonNegative(value), "Rgb");
            Value = value;
        }

        __attribute__((always_inline))
        Rgb operator+(Rgb other) const {
            return Rgb(Value + other.Value);
        }

        __attribute__((always_inline))
        Rgb operator-(Rgb other) const {
            return Rgb(Value - other.Value);
        }

        __attribute__((always_inline))
        string ToString() const {
            return Value.ToString();
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_RGB_H
