//
//  World.cpp
//  PRender
//
//  Created by Peter Sandquist on 9/30/20.
//

#include "World.h"
#include <iostream>
bool World::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    auto hit_anything = false;
    auto closest_so_far = t_max;

    for (int i = 0; i < objects.size(); i++) {
        //std::cerr << objects[i] << std::endl;
        if (objects[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
