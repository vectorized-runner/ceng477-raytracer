cmake_minimum_required(VERSION 3.23)
project(ceng477_raytracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(ceng477_raytracer src/template/parser.cpp src/template/parser.h src/template/ppm.cpp src/template/ppm.h src/template/raytracer.cpp src/template/tinyxml2.cpp src/template/tinyxml2.h src/Math/float3.h src/Data/Camera/CameraData.h src/Data/Camera/ImageRect.h src/Data/Camera/Resolution.h src/Data/Camera/ImagePlane.h src/Data/Camera/ImagePlane.h src/Data/Collision/AABB.h src/Math/Math.h src/Data/Collision/Ray.h src/Data/Collision/IntersectionResult.h src/Data/Objects/ObjectId.h src/Data/Lights/AmbientLightData.h src/Data/Lights/PointLightData.h src/Data/Shading/MaterialData.h src/Data/Shading/Rgb.h src/Data/Objects/Triangle.h src/Data/Objects/Sphere.h src/Data/Objects/SphereData.h src/Data/Objects/TriangleData.h src/Data/Objects/Mesh.h src/Data/Objects/MeshData.h src/Data/Objects/Scene.h)
