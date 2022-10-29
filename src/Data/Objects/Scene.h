
#ifndef CENG477_RAYTRACER_SCENE_H
#define CENG477_RAYTRACER_SCENE_H

#include "TriangleData.h"
#include "MeshData.h"
#include "SphereData.h"
#include "../Lights/PointLightData.h"
#include "../Lights/AmbientLightData.h"

namespace RayTracer {

    struct Scene {
        TriangleData TriangleData;
        MeshData MeshData;
        SphereData SphereData;

        PointLightData* PointLights;
        AmbientLightData AmbientLight;

        AABB AABB;
    };

} // RayTracer

#endif //CENG477_RAYTRACER_SCENE_H
