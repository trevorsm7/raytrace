#pragma once

#include "Ray.hpp"

#include <glm/vec3.hpp>

class IMaterial;

struct HitRecord
{
    float t;
    glm::vec3 p;
    glm::vec3 normal;
    IMaterial* material;
};


class ITexture
{
public:
    virtual glm::vec3 Shade(const HitRecord& hit) = 0;
};

class FlatColor : public ITexture
{
    glm::vec3 color_;

public:
    FlatColor(const glm::vec3& color): color_(color) {}

    glm::vec3 Shade(const HitRecord& hit) override {return color_;}
};

class DebugNormal : public ITexture
{
public:
    glm::vec3 Shade(const HitRecord& hit) override {return 0.5f*(hit.normal+1.f);}
};


class IMaterial
{
public:
    virtual bool Scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& out) = 0;
};

class Lambertian : public IMaterial
{
    ITexture* albedo_;

public:
    Lambertian(ITexture* albedo): albedo_(albedo) {}

    bool Scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& out) override;
};

class Metal : public IMaterial
{
    ITexture* albedo_;
    float roughness_;

public:
    Metal(ITexture* albedo, float roughness):
        albedo_(albedo), roughness_(roughness) {}

    bool Scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& out) override;
};

class Dielectric : public IMaterial
{
    // TODO add attenuation function; maybe add accumulate/integrate to ITexture
    float ri_in_, ri_out_;

public:
    Dielectric(float ri_in, float ri_out=1.f):
        ri_in_(ri_in), ri_out_(ri_out) {}

    bool Scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& out) override;
};
