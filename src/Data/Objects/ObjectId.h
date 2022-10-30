
#ifndef CENG477_RAYTRACER_OBJECTID_H
#define CENG477_RAYTRACER_OBJECTID_H

namespace RayTracer {

    enum ObjectType {
        None,
        SphereObject,
        TriangleObject,
        MeshTriangleObject,
    };

    struct ObjectId {
        ObjectType Type;
        int Index;
        int MeshIndex;

        ObjectId() {
            Type = ObjectType::None;
            Index = 0;
            MeshIndex = 0;
        }

        ObjectId(ObjectType type, int index, int meshIndex) {
            Type = type;
            Index = index;
            MeshIndex = meshIndex;
        }

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
