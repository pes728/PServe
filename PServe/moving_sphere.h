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

class moving_sphere : public hittable {
    public:
        moving_sphere() {}
        moving_sphere(
            vec3 cen0, vec3 cen1, double t0, double t1, double r, shared_ptr<material> m)
            : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m)
        {};

        virtual bool hit(
            const ray& r, double tmin, double tmax, hit_record& rec) const override;

        vec3 center(double time) const;

    public:
        vec3 center0, center1;
        double time0, time1;
        double radius;
        shared_ptr<material> mat_ptr;
};

#endif /* moving_sphere_h */
