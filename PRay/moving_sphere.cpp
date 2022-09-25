//
//  moving_sphere.cpp
//  PRender
//
//  Created by Peter Sandquist on 10/21/20.
//

#include "moving_sphere.h"

glm::vec3 moving_sphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

bool moving_sphere::hit(
    const ray& r, float t_min, float t_max, hit_record& rec) const {
    glm::vec3 oc = r.origin() - center(r.time());
    float a = glm::length2(r.direction());
    float half_b = dot(oc, r.direction());
    float c = glm::length2(oc) - radius*radius;

    float discriminant = half_b*half_b - a*c;

    if (discriminant > 0) {
        auto root = sqrt(discriminant);

        auto temp = (-half_b - root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            auto outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;
            return true;
        }

        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            auto outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;
            return true;
        }
    }
    return false;
}

bool moving_sphere::bounding_box(float time0, float time, aabb& outputBox) const
{
    aabb box0(
        center(time0) - glm::vec3(radius, radius, radius),
        center(time0) + glm::vec3(radius, radius, radius));
    aabb box1(
        center(time1) - glm::vec3(radius, radius, radius),
        center(time1) + glm::vec3(radius, radius, radius));
    outputBox = surrounding_box(box0, box1);
    return true;
}

void moving_sphere::toFile(std::ofstream& file) const
{
    file << "#MovingSphere (center0, center1, time0, time1, radius, mat)\n";
    file << glm::to_string(center0) << "\n" << glm::to_string(center1) << "\n" << time0 << "\n" << time1 << "\n" << radius << "\n";
    this->mat->toFile(file);
}
