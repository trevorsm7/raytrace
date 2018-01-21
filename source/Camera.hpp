#pragma once

#include "Ray.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Camera
{
    float width_, inv_width_, inv_height_, fovy_;
    glm::vec3 origin_, corner_, vert_, horz_;

public:
    Camera(uint32_t width, uint32_t height, float fovy);

    void SetOrientation(glm::vec3 origin, glm::vec3 look_at, glm::vec3 up);

    Ray GetRay(glm::vec2 pixel);
};
