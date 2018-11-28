#include "quadric.h"

Quadric::Quadric(V3 origin_coord, V3 param_vec, int quadric_type, Material* material)
    : origin_coord_(origin_coord), param_vec_(param_vec), quadric_type_(quadric_type) {
  material_ = material;
}
   
Intersection Quadric::intersect(Ray ray) {
  float a2, b2, c2, ab2, ac2, bc2, ax, bx, cx, D;
  a2 = param_vec_.x() * param_vec_.x();
  b2 = param_vec_.y() * param_vec_.y();
  c2 = param_vec_.z() * param_vec_.z();
  ab2 = a2 * b2;
  ac2 = a2 * c2;
  bc2 = b2 * c2;
  ax = ray.direction_unit_vec_.x() * ray.direction_unit_vec_.x() * bc2 + \
       ray.direction_unit_vec_.y() * ray.direction_unit_vec_.y() * ac2 + \
       ray.direction_unit_vec_.z() * ray.direction_unit_vec_.z() * ab2;
  bx = ((ray.origin_coord_.x() * ray.direction_unit_vec_.x() - ray.direction_unit_vec_.x() * origin_coord_.x()) * bc2  + \
        (ray.origin_coord_.y() * ray.direction_unit_vec_.y() - ray.direction_unit_vec_.y() * origin_coord_.y()) * ac2  + \
        (ray.origin_coord_.z() * ray.direction_unit_vec_.z() - ray.direction_unit_vec_.z() * origin_coord_.z()) * ab2) * 2;
  cx = (ray.origin_coord_.x() * ray.origin_coord_.x() + origin_coord_.x() * origin_coord_.x() - 2 * ray.origin_coord_.x() * origin_coord_.x()) * bc2 + \
       (ray.origin_coord_.y() * ray.origin_coord_.y() + origin_coord_.y() * origin_coord_.y() - 2 * ray.origin_coord_.y() * origin_coord_.y()) * ac2 + \
       (ray.origin_coord_.z() * ray.origin_coord_.z() + origin_coord_.z() * origin_coord_.z() - 2 * ray.origin_coord_.z() * origin_coord_.z()) * ab2 - ab2 * c2;
  D = bx * bx - ax * cx * 4;

  Intersection intersection;
  intersection.happened_ = false;
  if (D > 0) {
    float sD = sqrt(D);
    float t = (-0.5) * (bx + sD) / ax;

    if (t > 0) {
      V3 hitpoint = ray.origin_coord_ + ray.direction_unit_vec_ * t;
      V3 normal = V3((hitpoint.x() - origin_coord_.x()) / a2, (hitpoint.y() - origin_coord_.y()) / b2, (hitpoint.z() - origin_coord_.z()) / c2);

      intersection.happened_ = true;
      intersection.contact_coord_ = hitpoint;
      intersection.normal_unit_vec_ = normal.unit();
    }
  }

  return intersection;
}