#include "Material.hpp"

#include <glm/gtc/random.hpp>

using glm::vec3;

#include <random>
std::uniform_real_distribution<float> g_uniform;
std::mt19937 g_generator;

bool Lambertian::Scatter(const Ray& in, const HitRecord& hit, vec3& attenuation, Ray& out)
{
    // R < 1 so we don't get 0 vector, normalize to keep t_min relative
    out = Ray(hit.p, glm::normalize(hit.normal + glm::ballRand(0.99f)));
    attenuation = albedo_->Shade(hit);
    return true;

    //float p; // some function of attenuation
    //attenuation = albedo_->Shade(hit) / p;
    //return g_uniform(g_generator) >= p;
}

bool Metal::Scatter(const Ray& in, const HitRecord& hit, vec3& attenuation, Ray& out)
{
    // Discard rays from behind
    // TODO flip normal to handle other side?
    auto dot = glm::dot(in.Direction(), hit.normal);
    if (dot > 0.f)
        return false;

    vec3 reflection = in.Direction() - 2*dot*hit.normal;
    out = Ray(hit.p, glm::normalize(reflection) + glm::ballRand(roughness_));
    attenuation = albedo_->Shade(hit);
    return glm::dot(out.Direction(), hit.normal) > 0.f;
}

inline float Schlick(float cosine, float ref_idx)
{
    float r0 = (1.f - ref_idx) / (1.f + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.f - r0)*powf(1.f - cosine, 5.f);
}

bool Dielectric::Scatter(const Ray& in, const HitRecord& hit, vec3& attenuation, Ray& out)
{
    auto unit_in = glm::normalize(in.Direction());
    auto dot = glm::dot(unit_in, hit.normal);

    // Handle inward and outward rays
    vec3 normal_out;
    float ni_over_nt;
    float cosine;
    if (dot > 0.f)
    {
        normal_out = -hit.normal;
        ni_over_nt = ri_in_ / ri_out_;
        cosine = ri_in_ * dot;
    }
    else
    {
        normal_out = hit.normal;
        ni_over_nt = ri_out_ / ri_in_;
        cosine = -dot;
    }

    attenuation = vec3(1.f, 1.f, 1.f); // TODO try attenuation methods

    auto discriminant = 1.f - ni_over_nt*ni_over_nt*(1.f - dot*dot);
    auto reflect_prob = discriminant >= 0.f ? Schlick(cosine, ri_in_) : 1.f;

    if (g_uniform(g_generator) < reflect_prob)
    {
        out = Ray(hit.p, unit_in - 2*dot*hit.normal);
    }
    else
    {
        out = Ray(hit.p, ni_over_nt*(unit_in - dot*hit.normal) - sqrtf(discriminant)*normal_out);
    }

    return true;
}
