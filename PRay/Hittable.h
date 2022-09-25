//
//  Hittable.h
//  PRender
//
//  Created by Peter Sandquist on 9/29/20.
//

#ifndef Hittable_h
#define Hittable_h

#include "common.h"
#include <memory>
#include "aabb.h"
#include <string>

class material;

struct hit_record{
public:
    glm::vec3 p;
    glm::vec3 normal;
    shared_ptr<material> mat;
    double t;
    double u;
    double v;
    bool front_face;
    
    inline void set_face_normal(const ray& r, const glm::vec3& outward_normal) {
        front_face = glm::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
        virtual bool bounding_box(float time0, float time1, aabb& outputBox) const = 0;
        virtual void toFile(std::ofstream& file) const = 0;
};

#endif /* Hittable_h */
