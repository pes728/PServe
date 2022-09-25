//
//  ray.hpp
//  PRender
//
//  Created by Peter Sandquist on 9/29/20.
//

#ifndef ray_h
#define ray_h

#include "glm/vec3.hpp"

class ray{
public:
    ray(){}
    ray(const glm::vec3& origin, const glm::vec3& direction) : ori(origin), dir(direction), tm(0) {}
    ray(const glm::vec3& origin, const glm::vec3& direction, float time) : ori(origin), dir(direction), tm(time) {}
    
    glm::vec3 origin() const  { return ori; }
    glm::vec3 direction() const { return dir; }
    float time() const    { return tm; }
    
    glm::vec3 at(float t) const {
        return ori + dir * t;
    }
    
    
    glm::vec3 ori, dir;
    
    float tm;
};



#endif /* ray_hpp */
