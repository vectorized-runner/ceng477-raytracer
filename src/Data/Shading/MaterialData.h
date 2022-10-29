
#ifndef CENG477_RAYTRACER_MATERIALDATA_H
#define CENG477_RAYTRACER_MATERIALDATA_H

#include "../../Math/float3.h"

namespace RayTracer {

    struct MaterialData {
        float3 DiffuseReflectance;
        float3 AmbientReflectance;
        float3 MirrorReflectance;
        float3 SpecularReflectance;
        float PhongExponent;
        bool IsMirror;
    };

} // RayTracer

#endif //CENG477_RAYTRACER_MATERIALDATA_H
