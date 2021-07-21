//
//  World.h
//  PRender
//
//  Created by Peter Sandquist on 9/29/20.
//

#ifndef World_h
#define World_h
#include "PMath.h"
#include "Hittable.h"
#include "common.h"

#include <memory>
#include <vector>

class World : public hittable{
public:
    World(){ }
    
    void add(shared_ptr<hittable> object){
        objects.push_back(object);
    }
    
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
    
    std::vector<std::shared_ptr<hittable>> objects;
};

#endif /* World_h */
