
#ifndef CENG477_RAYTRACER_DEBUG_H
#define CENG477_RAYTRACER_DEBUG_H

#include <iostream>

using namespace std;

namespace RayTracer {

    struct Debug {
        static void Assert(bool condition, string message) {
            if (!condition) {
                cout << "Assertion Failed: " << message << endl;
            }
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_DEBUG_H
