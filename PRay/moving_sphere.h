//
//  moving_sphere.h
//  PRender
//
//  Created by Peter Sandquist on 10/21/20.
//

#ifndef moving_sphere_h
#define moving_sphere_h

#include "common.h"

#include "Hittable.h"
#include "Material.h"

class moving_sphere : public hittable {
    public:
        moving_sphere() {}
        moving_sphere(
            glm::vec3 cen0, glm::vec3 cen1, float t0, float t1, float r, shared_ptr<material> m)
            : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat(m)
        {};

        virtual bool hit(
            const ray& r, float tmin, float tmax, hit_record& rec) const override;
        
        virtual bool bounding_box(float time0, float time, aabb& ouputBox) const override;

        virtual void toFile(std::ofstream& file) const override;

        glm::vec3 center(float time) const;

    public:
        glm::vec3 center0, center1;
        float time0, time1;
        float radius;
        shared_ptr<material> mat;
};

#endif /* moving_sphere_h */
