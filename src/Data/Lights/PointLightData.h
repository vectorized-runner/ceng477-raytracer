
#ifndef CENG477_RAYTRACER_POINTLIGHTDATA_H
#define CENG477_RAYTRACER_POINTLIGHTDATA_H

#include "../../float3.h"

namespace RayTracer {

    struct PointLightData {
        float3 Position;
        float Intensity;
    };

} // RayTracer

#endif //CENG477_RAYTRACER_POINTLIGHTDATA_H
