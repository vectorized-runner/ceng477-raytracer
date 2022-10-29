
#ifndef CENG477_RAYTRACER_FLOAT3U_H
#define CENG477_RAYTRACER_FLOAT3U_H

#include "float3.h"

namespace RayTracer {

    // Union because c++ doesn't support custom indexer
    union float3u {
        float3 xyz;
        float arr[3];

        float3u() {
            xyz = float3(0, 0, 0);
        }

        float3u(float3 v) {
            xyz = v;
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_FLOAT3U_H
