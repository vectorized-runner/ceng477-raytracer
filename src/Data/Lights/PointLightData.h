
#ifndef CENG477_RAYTRACER_POINTLIGHTDATA_H
#define CENG477_RAYTRACER_POINTLIGHTDATA_H

#include "../../Math/float3.h"

namespace RayTracer {

    struct PointLightData {
        float3 Position;
        float3 Intensity;
    };

} // RayTracer

#endif //CENG477_RAYTRACER_POINTLIGHTDATA_H
