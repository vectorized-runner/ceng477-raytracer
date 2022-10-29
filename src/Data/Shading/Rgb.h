
#ifndef CENG477_RAYTRACER_RGB_H
#define CENG477_RAYTRACER_RGB_H

#include "../../Math/float3.h"
#include <string>

using namespace std;

namespace RayTracer {

    // TODO: Fix this, we've ported this from Unity implementation
    // The Rgb value we get is in (0, 255) range, Unity Color value is (0, 1) range
    // public Color Color => new(Value.x / 255f, Value.y / 255f, Value.z / 255f, 1f);
    struct Rgb {
        float3 Value;

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
