#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "../float3.h"

using namespace std;
using namespace RayTracer;

typedef unsigned char RGB[3];



struct AABB {
    float3 Min;
    float3 Max;

    void Encapsulate(float3 point) {
        Min = min(point, Min);
        Max = max(point, Max);
    }

    void Encapsulate(AABB other) {
        Min = min(Min, other.Min);
        Max = max(Max, other.Max);
    }
};

struct Ray
{
    float3 Origin;
    float3 Direction;

    Ray(float3 origin, float3 direction) : Origin(origin), Direction(direction)
    {
    }

    float3 GetPoint(float distance)
    {
        // TODO: Handle multiplication
        return float3(0, 0, 0);
        // return Origin + Direction * distance;
    }

    string toString()
    {
        return "Origin: " + Origin.ToString() + ", Direction: " + Direction.ToString();
    }
};

struct AmbientLightData
{
    float3 Radiance;
};

struct PointLightData
{
    float3 Position;
    float Intensity;
};

enum ObjectType
{
    None,
    Sphere,
    Triangle,
    MeshTriangle,
};

void debug_assert(bool condition) {
    if (!condition) {
        printf("Assertion Failed.");
    }
}

int main(int argc, char *argv[]) {
    auto f = float3(0, 0, 0);

    printf("This code is doing nothing!");

    return 0;

    cout << "Running Raytracer!" << endl;

    // Sample usage for reading an XML scene file
    parser::p_scene scene;

    cout << "First arg:" << endl;
    cout << argv[0] << endl;

    cout << "Last arg:" << endl;
    cout << argv[argc - 1] << endl;

    // TODO-Use the correct argument here for the final build!
    scene.loadFromXml(argv[argc - 1]);

    cout << "This is after scene load!" << endl;

    // The code below creates a test pattern and writes
    // it to a PPM file to demonstrate the usage of the
    // ppm_write function.
    //
    // Normally, you would be running your ray tracing
    // code here to produce the desired image.

    const RGB BAR_COLOR[8] =
            {
                    {255, 255, 255},  // 100% White
                    {255, 255, 0},  // Yellow
                    {0,   255, 255},  // Cyan
                    {0,   255, 0},  // Green
                    {255, 0,   255},  // Magenta
                    {255, 0,   0},  // Red
                    {0,   0,   255},  // Blue
                    {0,   0,   0},  // Black
            };

    int width = 640, height = 480;
    int columnWidth = width / 8;

    unsigned char *image = new unsigned char[width * height * 3];

    int i = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int colIdx = x / columnWidth;
            image[i++] = BAR_COLOR[colIdx][0];
            image[i++] = BAR_COLOR[colIdx][1];
            image[i++] = BAR_COLOR[colIdx][2];
        }
    }

    cout << "This is before write ppm!" << endl;

    write_ppm("test.ppm", image, width, height);

    cout << "Finished!" << endl;

}
