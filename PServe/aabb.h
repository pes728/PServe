//
//  aabb.h
//  PRender
//
//  Created by Peter Sandquist on 10/21/20.
//


#ifndef aabb_h
#define aabb_h

#include "common.h"


class aabb{
public:
    aabb(){}
    aabb(const vec3& a, const vec3& b){ _min = a; _max = b; }

    vec3 min() const { return _min; }
    vec3 max() const { return _max; }

    inline bool hit(const ray& r, double tmin, double tmax) const;
    

    vec3 _min, _max;
};

aabb surrounding_box(aabb box0, aabb box1);

inline bool aabb::hit(const ray& r, double tmin, double tmax) const {
    for (int a = 0; a < 3; a++) {
        auto invD = 1.0f / r.direction()[a];
        auto t0 = (min()[a] - r.origin()[a]) * invD;
        auto t1 = (max()[a] - r.origin()[a]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin)
            return false;
    }
    return true;
}



#endif /* aabb_h */
