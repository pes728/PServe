#pragma once

#include "common.h"
#include "Hittable.h"
#include "hittable_list.h"
#include <iostream>


class bvh_node : public hittable
{
public:
	bvh_node();

	bvh_node(hittable_list& list, float time0, float time1) :
		bvh_node(list.objects, 0, list.objects.size(), time0, time1) {}

	bvh_node(std::vector<shared_ptr<hittable>>& src_objects, size_t start, size_t end, double time0, double time1);

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

	virtual bool bounding_box(float time0, float time1, aabb& outputBox) const override;

	virtual void toFile(std::ofstream & file) const override;

	shared_ptr<hittable> left, right;
	aabb box;

};

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis);

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);