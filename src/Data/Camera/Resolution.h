
#ifndef CENG477_RAYTRACER_RESOLUTION_H
#define CENG477_RAYTRACER_RESOLUTION_H

#include <string>

using namespace std;

namespace RayTracer {

    struct Resolution {
        int X;
        int Y;

        Resolution(int x, int y) {
            X = x;
            Y = y;
        }

        string ToString() const {
            return "(" + to_string(X) + ", " + to_string(Y) + ")";
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_RESOLUTION_H
