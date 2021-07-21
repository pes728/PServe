//
//  PMath.h
//  PRender
//
//  Created by Peter Sandquist on 9/29/20.
//

#ifndef PMath_h
#define PMath_h

#include <cmath>
#include <limits>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double(){
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max){
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max){
    if(x > max) return max;
    if(x < min) return min;
    return x;
}

inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max+1));
}

#endif /* PMath_h */
