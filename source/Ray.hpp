#pragma once

#include <glm/vec3.hpp>

class Ray
{
private:
    glm::vec3 a_, b_;

public:
    Ray() = default;
    Ray(const glm::vec3& a, const glm::vec3& b): a_(a), b_(b) {}

    glm::vec3 Origin() const {return a_;}
    glm::vec3 Direction() const {return b_;}
    glm::vec3 PointAtParameter(float t) const {return a_ + b_ * t;}
};
