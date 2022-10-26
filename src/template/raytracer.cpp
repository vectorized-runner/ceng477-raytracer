#include <iostream>
#include "parser.h"
#include "ppm.h"

using namespace std;

typedef unsigned char RGB[3];

int main(int argc, char* argv[])
{
    cout << "Running Raytracer!" << endl;

    // Sample usage for reading an XML scene file
    parser::Scene scene;

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
                    { 255, 255, 255 },  // 100% White
                    { 255, 255,   0 },  // Yellow
                    {   0, 255, 255 },  // Cyan
                    {   0, 255,   0 },  // Green
                    { 255,   0, 255 },  // Magenta
                    { 255,   0,   0 },  // Red
                    {   0,   0, 255 },  // Blue
                    {   0,   0,   0 },  // Black
            };

    int width = 640, height = 480;
    int columnWidth = width / 8;

    unsigned char* image = new unsigned char [width * height * 3];

    int i = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
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
