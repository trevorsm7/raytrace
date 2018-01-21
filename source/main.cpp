#include "ppm.h"
#include "Ray.hpp"
#include "Material.hpp"
#include "Object.hpp"
#include "Sampler.hpp"
#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <memory>
#include <cmath>
#include <limits>

using glm::vec2;
using glm::vec3;

vec3 Color(IObject* object, const Ray& ray, int depth)
{
    // Limit tail recursion
    if (depth <= 0)
        return vec3(0.f);

    HitRecord hit;
    float t_max = std::numeric_limits<float>::infinity();

    // TODO handle null material?
    if (object->Hit(ray, 0.01f, t_max, hit))
    {
        // TODO consider moving this whole block into material
        Ray scattered;
        vec3 attenuation;
        if (hit.material->Scatter(ray, hit, attenuation, scattered))
        {
            return attenuation * Color(object, scattered, depth-1);
        }
        else
        {
            // TODO emmissive materials/lights?
            return vec3(0.f);
        }
    }

    // TODO add a skybox object/material for background?
    vec3 unit_dir = glm::normalize(ray.Direction());
    float t = 0.5f * (unit_dir[1] + 1.f);
    return (1.f-t)*vec3(1.f, 1.f, 1.f) + t*vec3(0.5f, 0.7f, 1.f);
}

int main(int argc, char* argv[])
{
    const auto width = 800u;
    const auto height = 600u;
    const auto depth = 255u;
    const auto samples = 512u;

    const auto inv_samples = 1.f / float(samples);

    // Init camera
    Camera camera(width, height, 20.f);
    camera.SetOrientation(vec3(-2.f, 2.f, 1.f), vec3(0.f, 0.f, -1.f), vec3(0.f, 1.f, 0.f));
    GridSampler sampler(samples, 26.6f, 1.118f);
    //RandomSampler sampler;

    // Init materials
    FlatColor color1(vec3(0.1f, 0.2f, 0.5f));
    FlatColor color2(vec3(0.8f, 0.8f, 0.0f));
    FlatColor color3(vec3(0.8f, 0.6f, 0.2f));

    Lambertian material1(&color1);
    Lambertian material2(&color2);
    Metal material3(&color3, 0.f);
    Dielectric material4(1.5f);

    // Init objects
    Sphere sphere1(&material1, vec3(-0.5f, 0.f, -1.5f), 0.5f);
    Sphere sphere2(&material2, vec3(-0.5f, -100.5f, -1.f), 100.f);
    Sphere sphere3(&material3, vec3(1.f, 0.f, -1.5f), 0.5f);
    Sphere sphere4(&material4, vec3(0.f, 0.f, -0.5f), 0.5f);
    Sphere sphere5(&material4, vec3(0.f, 0.f, -0.5f), -0.45f);
    ObjectList objects({&sphere1, &sphere2, &sphere3, &sphere4, &sphere5});

    // Init frame buffer
    auto data = std::make_unique<float[]>(width*height*3);
    auto ptr = data.get();

    for (auto h = 0; h < height; ++h)
    {
        for (auto w = 0u; w < width; ++w)
        {
            vec3 color;
            for (auto s = 0u; s < samples; ++s)
            {
                auto pixel = vec2(float(w), float(h));
                auto sample = sampler.GetSample(s);
                auto ray = camera.GetRay(pixel + sample);
                color += Color(&objects, ray, 16);
            }

            // gamma correction
            color = glm::sqrt(color * inv_samples) * float(depth);
            *ptr++ = color[0];
            *ptr++ = color[1];
            *ptr++ = color[2];
        }
    }

    WritePPM("test.ppm", data.get(), width, height, depth);

    return 0;
}
