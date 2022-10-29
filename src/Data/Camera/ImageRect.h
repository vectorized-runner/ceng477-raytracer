
#ifndef CENG477_RAYTRACER_IMAGERECT_H
#define CENG477_RAYTRACER_IMAGERECT_H

#include "../../float3.h"

namespace RayTracer {

    struct ImageRect {
        float3 TopLeft;
        float3 TopRight;
        float3 BottomRight;
        float3 BottomLeft;
    };

} // RayTracer

#endif //CENG477_RAYTRACER_IMAGERECT_H
