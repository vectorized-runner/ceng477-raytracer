
#ifndef CENG477_RAYTRACER_SPHEREDATA_H
#define CENG477_RAYTRACER_SPHEREDATA_H

#include "Sphere.h"
#include "../Shading/MaterialData.h"

namespace RayTracer {

    struct SphereData {
        int Count;
        Sphere* Spheres;
        char* MaterialIndices;
    };


} // RayTracer

#endif //CENG477_RAYTRACER_SPHEREDATA_H
