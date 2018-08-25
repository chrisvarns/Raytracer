#include "bvh_node.h"
#include "rand.h"

int compare_x(const void* a, const void* b) {
    aabb box_left, box_right;
    hitable* ah = *(hitable**)a;
    hitable* bh = *(hitable**)b;
    if(!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
        abort();
    }
    if(box_left.min.x - box_right.min.x < 0)
        return -1;
    return 1;
}

int compare_y(const void* a, const void* b) {
    aabb box_left, box_right;
    hitable* ah = *(hitable**)a;
    hitable* bh = *(hitable**)b;
    if(!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
        abort();
    }
    if(box_left.min.y - box_right.min.y < 0)
        return -1;
    return 1;
}

int compare_z(const void* a, const void* b) {
    aabb box_left, box_right;
    hitable* ah = *(hitable**)a;
    hitable* bh = *(hitable**)b;
    if(!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
        abort();
    }
    if(box_left.min.z - box_right.min.z < 0)
        return -1;
    return 1;
}

bvh_node::bvh_node(const hitable** l, int n, float time0, float time1) {
    int axis = fastrand() % 3;
    qsort(l,
          n,
          sizeof(hitable*),
          axis == 0 ? compare_x :
            axis == 1 ? compare_y :
            compare_z);

    if(n == 1) {
        left_ = right_ = l[0];
    }
    else if(n == 2) {
        left_ = l[0];
        right_ = l[1];
    }
    else {
        left_ = new bvh_node(l, n/2, time0, time1);
        right_ = new bvh_node(l + n/2, n - n/2, time0, time1);
    }

    aabb box_left, box_right;
    if(!left_->bounding_box(time0, time1, box_left) || !right_->bounding_box(time0, time1, box_right))
        abort();
    box_ = surrounding_box(box_left, box_right);
}

bool bvh_node::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    if(box_.hit(r, t_min, t_max)) {
        hit_record left_rec, right_rec;
        bool hit_left = left_->hit(r, t_min, t_max, left_rec);
        bool hit_right = right_->hit(r, t_min, t_max, right_rec);
        if(hit_left && hit_right) {
            rec = left_rec.t < right_rec.t ? left_rec : right_rec;
            return true;
        }
        else if (hit_left) {
            rec = left_rec;
            return true;
        }
        else if(hit_right) {
            rec = right_rec;
            return true;
        }
    }
    return false;
}

bool bvh_node::bounding_box(float t0, float t1, aabb &b) const {
    b = box_;
    return true;
}
