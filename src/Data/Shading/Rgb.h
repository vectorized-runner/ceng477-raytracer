
#ifndef CENG477_RAYTRACER_RGB_H
#define CENG477_RAYTRACER_RGB_H

#include "../../Math/float3.h"
#include <string>

using namespace std;

namespace RayTracer {

    // This RGB value is in [0, 255] range (it's not [0, 1])
    struct Rgb {

        float3 Value;

        Rgb() {

        }

        Rgb(float3 value) {
            Value = value;
        }

        Rgb operator+(Rgb other) const {
            return Rgb(Value + other.Value);
        }

        Rgb operator-(Rgb other) const {
            return Rgb(Value - other.Value);
        }

        string ToString() const {
            return Value.ToString();
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_RGB_H
