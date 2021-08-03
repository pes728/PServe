//
//  World.h
//  PRender
//
//  Created by Peter Sandquist on 9/29/20.
//

#ifndef hittable_list_h
#define hittable_list_h
#include "PMath.h"
#include "Hittable.h"
#include "common.h"

#include <memory>
#include <vector>

class hittable_list : public hittable{
public:
    hittable_list(){ }
    
    void add(shared_ptr<hittable> object){
        objects.push_back(object);
    }
    
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
    
    virtual bool bounding_box(double time0, double time1, aabb& outputBox) const override;

    std::vector<std::shared_ptr<hittable>> objects;
};

#endif /* World_h */
