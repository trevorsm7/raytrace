#include "Object.hpp"

#include <glm/glm.hpp>

bool Sphere::Hit(const Ray& ray, float t_min, float t_max, HitRecord& hit)
{
    auto offset = ray.Origin() - center_;
    float a = glm::dot(ray.Direction(), ray.Direction());
    float b = glm::dot(ray.Direction(), offset);
    float c = glm::dot(offset, offset) - radius_*radius_;

    // Compute the discriminant; bail if negative
    float disc = b*b - a*c;
    if (disc < 0.f)
        return false;
    disc = sqrtf(disc);

    // Try smaller t first
    auto t = (-b - disc) / a;
    if (t <= t_min)
    {
        // If too small, try bigger t
        t = (-b + disc) / a;
        if (t <= t_min)
            return false;
    }
    if (t >= t_max)
        return false;

    hit.t = t;
    hit.p = ray.PointAtParameter(t);
    hit.normal = (hit.p - center_) / radius_; // normalize with radius
    hit.material = GetMaterial();
    return true;
}

bool ObjectList::Hit(const Ray& ray, float t_min, float t_max, HitRecord& hit)
{
    bool found_hit = false;

    for (auto object : objects_)
    {
        if (object->Hit(ray, t_min, t_max, hit))
        {
            found_hit = true;
            t_max = hit.t;
        }
    }

    return found_hit;
}
