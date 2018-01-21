#include "Sampler.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec2;

GridSampler::GridSampler(uint32_t samples, float degrees, float scale)
{
    if (samples == 0)
        samples = 1;

    // Compute grid dimensions
    float rows = floor(sqrtf(samples));
    float cols = ceil(samples / rows);
    float w = scale / cols;
    float h = scale / rows;
    float x0 = (w - scale) / 2.f;
    float y0 = (h - scale) / 2.f;

    // Establish coordinate system
    float radians = glm::radians(degrees);
    auto origin = glm::rotate(vec2(x0, y0), radians) + 0.5f;
    auto right = glm::rotate(vec2(w, 0), radians);
    auto down = glm::rotate(vec2(0, h), radians);

    // Generate sample LUT
    for (uint32_t y = 0; y < uint32_t(rows); ++y)
    {
        for (uint32_t x = 0; x < uint32_t(cols); ++x)
        {
            samples_.push_back(origin + x*right + y*down);
        }
    }
}

vec2 GridSampler::GetSample(uint32_t i)
{
    return samples_[i];
}

vec2 RandomSampler::GetSample(uint32_t)
{
    return vec2(uniform_(generator_), uniform_(generator_));
}
