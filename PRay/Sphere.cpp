//
//  Sphere.cpp
//  PRender
//
//  Created by Peter Sandquist on 10/20/20.
//

#include "Sphere.h"

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    glm::vec3 oc = r.origin() - center;
    float a = glm::length2(r.direction());
    float half_b = dot(oc, r.direction());
    float c = glm::length2(oc) - radius*radius;
    float discriminant = half_b*half_b - a*c;

    if (discriminant > 0) {
        float root = sqrt(discriminant);

        float temp = (-half_b - root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            glm::vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv(outward_normal, rec.u, rec.v);
            rec.mat = mat;
            return true;
        }

        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            glm::vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv(outward_normal, rec.u, rec.v);
            rec.mat = mat;
            return true;
        }
    }

    return false;
}


bool sphere::bounding_box(float time0, float time1, aabb& outputBox) const {
    outputBox = aabb(center - glm::vec3(radius, radius, radius),
                     center + glm::vec3(radius, radius, radius));
    return true;
}

void sphere::toFile(std::ofstream& file) const
{
    file << "#Sphere (center, radius, mat)\n";
    file << glm::to_string(this->center) << "\n";
    file << this->radius << "\n";
    this->mat->toFile(file);
}

