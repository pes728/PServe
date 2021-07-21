//
//  Color.h
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//



#ifndef Color_h
#define Color_h

#include <stdint.h>
#include "vec3.h"

class Color{
public:
    Color(uint8_t r, uint8_t g, uint8_t b){
        data[0] = r;
        data[1] = g;
        data[2] = b;
    }
    
    Color(const vec3& v, int samples){
        auto r = v.e[0];
        auto g = v.e[1];
        auto b = v.e[2];
        
        auto scale = 1.0 / samples;
        
        r = sqrt(scale * r);
        g = sqrt(scale * g);
        b = sqrt(scale * b);
        
        data[0] = static_cast<int>(256 * clamp(r, 0.0, 0.999));
        data[1] = static_cast<int>(256 * clamp(g, 0.0, 0.999));
        data[2] = static_cast<int>(256 * clamp(b, 0.0, 0.999));
    }
    
    uint8_t data[3];
};

#endif /* Color_h */
