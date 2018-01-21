#pragma once

#include <cstdint>
#include <string>

void WritePPM(std::string filename, const float* data, uint32_t width, uint32_t height, uint32_t depth);
