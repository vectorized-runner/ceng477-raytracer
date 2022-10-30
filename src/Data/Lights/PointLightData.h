
#ifndef CENG477_RAYTRACER_POINTLIGHTDATA_H
#define CENG477_RAYTRACER_POINTLIGHTDATA_H

#include "PointLight.h"

namespace RayTracer {

    struct PointLightData {
        int Count;
        PointLight* PointLights;
    };

} // RayTracer

#endif //CENG477_RAYTRACER_POINTLIGHTDATA_H
