#include "aabb.h"

aabb surrounding_box(aabb box0, aabb box1) {
    glm::vec3 small(fmin(box0.getMin().x, box1.getMin().x),
        fmin(box0.getMin().y, box1.getMin().y),
        fmin(box0.getMin().z, box1.getMin().z));

    glm::vec3 big(fmax(box0.getMax().x, box1.getMax().x),
        fmax(box0.getMax().y, box1.getMax().y),
        fmax(box0.getMax().z, box1.getMax().z));

    return aabb(small, big);
}