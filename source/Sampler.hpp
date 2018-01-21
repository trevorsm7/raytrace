#pragma once

#include <glm/vec2.hpp>
#include <vector>
#include <cstdint>
#include <random>

class ISampler
{
public:
    virtual glm::vec2 GetSample(uint32_t i) = 0;
};

class GridSampler : public ISampler
{
    std::vector<glm::vec2> samples_;

public:
    GridSampler(uint32_t samples, float degrees, float scale);

    glm::vec2 GetSample(uint32_t i) override;
};

class RandomSampler : public ISampler
{
    std::uniform_real_distribution<float> uniform_;
    std::mt19937 generator_;

public:
    glm::vec2 GetSample(uint32_t i);
};
