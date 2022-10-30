
#ifndef CENG477_RAYTRACER_DEBUG_H
#define CENG477_RAYTRACER_DEBUG_H

#include <iostream>

using namespace std;

namespace RayTracer {

    struct Debug {
        static void Assert(bool condition, const string &message) {
            if (!condition) {
                Log("Assertion Failed: " + message);
            }
        }

        static void Log(const string &message) {
            cout << message << endl;
        }

        static void Log(const string &message, int i) {
            cout << message << i << endl;
        }

        static void Log(const string &message, float f) {
            cout << message << f << endl;
        }

        static void Log(const string &message, char* c) {
            cout << message << c << endl;
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_DEBUG_H
