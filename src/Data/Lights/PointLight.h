
#ifndef CENG477_RAYTRACER_POINTLIGHT_H
#define CENG477_RAYTRACER_POINTLIGHT_H

#include "../../Math/float3.h"

namespace RayTracer {

    struct PointLight {
        float3 Position;
        float3 Intensity;
    };

} // RayTracer

#endif //CENG477_RAYTRACER_POINTLIGHT_H
