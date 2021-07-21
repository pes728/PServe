//
//  ray.hpp
//  PRender
//
//  Created by Peter Sandquist on 9/29/20.
//

#ifndef ray_h
#define ray_h

#include "vec3.h"
class ray{
public:
    ray(){}
    ray(const vec3& origin, const vec3& direction) : ori(origin), dir(direction), tm(0) {}
    ray(const vec3& origin, const vec3& direction, double time) : ori(origin), dir(direction), tm(time) {}
    
    vec3 origin() const  { return ori; }
    vec3 direction() const { return dir; }
    double time() const    { return tm; }
    
    vec3 at(double t) const {
        return ori + t * dir;
    }
    
    
    vec3 ori, dir;
    
    double tm;
};



#endif /* ray_hpp */
