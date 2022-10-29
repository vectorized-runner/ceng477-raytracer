
#ifndef CENG477_RAYTRACER_SPHERE_H
#define CENG477_RAYTRACER_SPHERE_H

#include "../../Math/float3.h"
#include "../Collision/AABB.h"
#include <cmath>

namespace RayTracer {

    struct Sphere {
        float3 Center;
        float RadiusSquared;

        string ToString() const {
            return "Center: " + Center.ToString() + ", RadiusSq: " + to_string(RadiusSquared);
        }

        AABB GetAABB() const {
            return AABB(Center - sqrt(RadiusSquared), Center + sqrt(RadiusSquared));
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_SPHERE_H
