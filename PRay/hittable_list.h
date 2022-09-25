//
//  World.h
//  PRender
//
//  Created by Peter Sandquist on 9/29/20.
//

#ifndef hittable_list_h
#define hittable_list_h
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
    
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const override;
    
    virtual bool bounding_box(float time0, float time1, aabb& outputBox) const override;

    virtual void toFile(std::ofstream& file) const override;

    std::vector<std::shared_ptr<hittable>> objects;
};

#endif /* World_h */
