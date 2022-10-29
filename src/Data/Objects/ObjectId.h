
#ifndef CENG477_RAYTRACER_OBJECTID_H
#define CENG477_RAYTRACER_OBJECTID_H

namespace RayTracer {

    enum ObjectType {
        None,
        Sphere,
        Triangle,
        MeshTriangle,
    };

    struct ObjectId {
        ObjectType Type;
        int Index;
        int MeshIndex;

        bool operator==(ObjectId other) const {
            return Type == other.Type && Index == other.Index && MeshIndex == other.MeshIndex;
        }

        bool operator!=(ObjectId other) const {
            // return Type != other.Type || Index != other.Index || MeshIndex != other.MeshIndex;
            return !(*this == other);
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_OBJECTID_H
