
#ifndef CENG477_RAYTRACER_CAMERADATA_H
#define CENG477_RAYTRACER_CAMERADATA_H

#include "../../Math/float3.h"

namespace RayTracer {

    struct CameraData {
        float3 Position;
        float3 Forward;
        float3 Right;
        float3 Up;
    };

} // RayTracer

#endif //CENG477_RAYTRACER_CAMERADATA_H
