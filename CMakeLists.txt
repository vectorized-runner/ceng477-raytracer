cmake_minimum_required(VERSION 3.23)
project(ceng477_raytracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(ceng477_raytracer src/template/parser.cpp src/template/parser.h src/template/ppm.cpp src/template/ppm.h src/template/raytracer.cpp src/template/tinyxml2.cpp src/template/tinyxml2.h src/float3.h src/Data/Camera/CameraData.h src/Data/Camera/ImageRect.h)
