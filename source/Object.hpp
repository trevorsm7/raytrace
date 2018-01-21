#pragma once

#include <glm/vec3.hpp>
#include "Material.hpp"
#include "Ray.hpp"

#include <vector>

class IObject
{
    IMaterial* material_;

public:
    IObject(IMaterial* material):
        material_(material) {}

    IMaterial* GetMaterial() {return material_;}

    virtual bool Hit(const Ray& ray, float t_min, float t_max, HitRecord& hit) = 0;
};

class Sphere : public IObject
{
private:
    glm::vec3 center_;
    float radius_;

public:
    Sphere(IMaterial* material, const glm::vec3& center, float radius):
        IObject(material), center_(center), radius_(radius) {}

    bool Hit(const Ray& ray, float t_min, float t_max, HitRecord& hit) override;
};

class ObjectList : public IObject
{
private:
    std::vector<IObject*> objects_;

public:
    ObjectList(const std::vector<IObject*>& objects):
        IObject(nullptr), objects_(objects) {}

    bool Hit(const Ray& ray, float t_min, float t_max, HitRecord& hit) override;
};
