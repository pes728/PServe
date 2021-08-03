//
//  Sphere.h
//  PRender
//
//  Created by Peter Sandquist on 9/29/20.
//

#ifndef Sphere_h
#define Sphere_h

#include "common.h"

#include "Hittable.h"


class sphere : public hittable {
    public:
        sphere() {}

        sphere(vec3 cen, double r, shared_ptr<material> m): center(cen), radius(r), mat(m) {};

        virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& outputBox) const override;

    public:
        vec3 center;
        double radius;
        shared_ptr<material> mat;

    private:
        static void get_sphere_uv(const vec3& point, double& u, double& v) {
            // p: a given point on the sphere of radius one, centered at the origin.
            // u: returned value [0,1] of angle around the Y axis from X=-1.
            // v: returned value [0,1] of angle from Y=-1 to Y=+1.

            auto theta = acos(-point.y());
            auto phi = atan2(-point.z(), point.x()) + pi;

            u = phi / (2 * pi);
            v = theta / pi;
        }
};

#endif /* Sphere_h */
