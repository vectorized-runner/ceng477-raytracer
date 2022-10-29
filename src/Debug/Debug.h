
#ifndef CENG477_RAYTRACER_DEBUG_H
#define CENG477_RAYTRACER_DEBUG_H

#include <iostream>

using namespace std;

namespace RayTracer {

    struct Debug {
        static void Assert(bool condition) {
            if (!condition) {
                cout << "Assertion Failed." << endl;
            }
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_DEBUG_H
