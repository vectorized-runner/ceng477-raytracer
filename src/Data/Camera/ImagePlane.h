
#ifndef CENG477_RAYTRACER_IMAGEPLANE_H
#define CENG477_RAYTRACER_IMAGEPLANE_H

#include "Resolution.h"
#include "CameraData.h"
#include "ImageRect.h"

namespace RayTracer {

    struct ImagePlane {
        Resolution Resolution;
        float DistanceToCamera;
        float HalfHorizontalLength;
        float HalfVerticalLength;

        float HorizontalLength() const {
            return HalfHorizontalLength * 2;
        }

        float VerticalLength() const {
            return HalfVerticalLength * 2;
        }

        float3 Center(CameraData cameraData) const {
            return cameraData.Position + cameraData.Forward * DistanceToCamera;
        }

        ImageRect GetRect(CameraData cameraData) const {
            auto center = Center(cameraData);
            auto halfUp = cameraData.Up * HalfVerticalLength;
            auto halfRight = cameraData.Right * HalfHorizontalLength;

            return ImageRect
                    (center - halfRight + halfUp,
                     center + halfRight + halfUp,
                     center + halfRight - halfUp,
                     center - halfRight - halfUp);
        }
    };


} // RayTracer

#endif //CENG477_RAYTRACER_IMAGEPLANE_H
