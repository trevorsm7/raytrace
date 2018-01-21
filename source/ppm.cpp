#include "ppm.h"

#include <fstream>

void WritePPM(std::string filename, const float* data, uint32_t width, uint32_t height, uint32_t depth)
{
    std::fstream file(filename, std::ios::out | std::ios::binary);

    file << "P3\n"; // P6 uses binary samples
    file << width << " " << height << "\n";
    file << depth << "\n";

    for (auto h = 0u; h < height; ++h)
    {
        for (auto w = 0u; w < width; ++w)
        {
            auto r = uint32_t(*data++);
            auto g = uint32_t(*data++);
            auto b = uint32_t(*data++);
            file << r << " " << g << " " << b << " ";
        }
        file << "\n";
    }
}
