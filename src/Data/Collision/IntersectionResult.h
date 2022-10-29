
#ifndef CENG477_RAYTRACER_INTERSECTIONRESULT_H
#define CENG477_RAYTRACER_INTERSECTIONRESULT_H

#include "../Objects/ObjectId.h"

namespace RayTracer {

    struct IntersectionResult {
        ObjectId ObjectId;
        float Distance;
    };

} // RayTracer

#endif //CENG477_RAYTRACER_INTERSECTIONRESULT_H
