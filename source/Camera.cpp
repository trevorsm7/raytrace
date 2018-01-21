#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

using glm::vec2;
using glm::vec3;

Camera::Camera(uint32_t width, uint32_t height, float fovy):
    width_(width), inv_width_(1.f/width), inv_height_(1.f/height), fovy_(fovy)
{
    SetOrientation(vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, -1.f), vec3(0.f, 1.f, 0.f));
}

void Camera::SetOrientation(glm::vec3 origin, glm::vec3 look_at, glm::vec3 up)
{
    origin_ = origin;

    // Establish coordinate system
    auto forward = glm::normalize(look_at - origin);
    auto right = glm::normalize(glm::cross(forward, up));
    up = glm::cross(right, forward);

    // Scale axes by FoV
    float scale = glm::tan(glm::radians(fovy_ * 0.5f));
    right *= scale * width_ * inv_height_;
    auto down = up * -scale; // screen space y-down to world space y-up

    // Compute camera vectors
    corner_ = forward - right - down;
    vert_ = down * 2.f;
    horz_ = right * 2.f;
}

Ray Camera::GetRay(vec2 pixel)
{
    // TODO could make this a matrix multiply
    auto right = pixel[0] * inv_width_ * horz_;
    auto down = pixel[1] * inv_height_ * vert_;
    return Ray(origin_, corner_ + right + down);
}
