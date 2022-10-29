
#ifndef CENG477_RAYTRACER_INTERSECTIONRESULT_H
#define CENG477_RAYTRACER_INTERSECTIONRESULT_H

#include "../Objects/ObjectId.h"

namespace RayTracer {

    struct IntersectionResult {
        ObjectId ObjectId;
        float Distance;

        IntersectionResult(struct ObjectId objectId, float distance) {
            ObjectId = objectId;
            Distance = distance;
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_INTERSECTIONRESULT_H
