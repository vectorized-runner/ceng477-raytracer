
#ifndef CENG477_RAYTRACER_RESOLUTION_H
#define CENG477_RAYTRACER_RESOLUTION_H

#include <string>

using namespace std;

namespace RayTracer {

    struct Resolution {
        int X;
        int Y;

        string ToString() {
            return "(" + to_string(X) + ", " + to_string(Y) + ")";
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_RESOLUTION_H
