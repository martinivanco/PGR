#include "quadric.h"
#include "stdio.h"

Quadric::Quadric(V3 origin_coord, V3 param_vec, int quadric_type, Material* material)
    : origin_coord_(origin_coord), param_vec_(param_vec), quadric_type_(quadric_type) {
  material_ = material;
}
   
Intersection Quadric::intersect(Ray ray) {
  float ax, bx, cx, D;

  // Select type of quadric and count a, b, c
  switch (quadric_type_) {
      case 1:
        count_eq_type1(ray, 1, 1, &ax, &bx, &cx);
        break;
      case 2:
        count_eq_type2(ray, 1, -1, 0, &ax, &bx, &cx);
        break;
      case 3:
        count_eq_type2(ray, -1, -1, 0, &ax, &bx, &cx);
        break;
      case 4:
        count_eq_type1(ray, -1, 1, &ax, &bx, &cx);
        break;
      case 5:
        count_eq_type1(ray, -1, -1, &ax, &bx, &cx);
        break;
      case 6:
        count_eq_type1(ray, -1, 0, &ax, &bx, &cx);
        break;
      case 7:
        count_eq_type2(ray, 1, 0, 1, &ax, &bx, &cx);
        break;
      case 8:
        count_eq_type2(ray, -1, 0, 1, &ax, &bx, &cx);
        break;
      default:
        printf("Warning: You chose invalid type of quadric. Defaulting to ellipsoid.\n");
        count_eq_type1(ray, 1, 1, &ax, &bx, &cx);
        break;
  }
  
  // Count intersection
  Intersection intersection;
  intersection.happened_ = false;
  D = bx * bx - ax * cx * 4;
  if (D > 0) {
    float sD = sqrt(D);
    float t1 = (-bx + sD) / (2 * ax);
    float t2 = (-bx - sD) / (2 * ax);
    float t = -1;
    if (t1 > 0 && t2 > 0)
      t = t1 > t2 ? t2 : t1;
    else
      t = t1 > 0 ? t1 : t2;

    if (t > 0) {
      V3 hitpoint = ray.origin_coord_ + ray.direction_unit_vec_ * t;
      V3 normal = V3((hitpoint.x() - origin_coord_.x()) / (param_vec_.x() * param_vec_.x()), \
                     (hitpoint.y() - origin_coord_.y()) / (param_vec_.y() * param_vec_.y()), \
                     (hitpoint.z() - origin_coord_.z()) / (param_vec_.z() * param_vec_.z()));

      intersection.happened_ = true;
      intersection.contact_coord_ = hitpoint;
      intersection.normal_unit_vec_ = normal.unit();
    }
  }

  return intersection;
}

void Quadric::count_eq_type1(Ray ray, int e1, int e2, float *ax, float *bx, float *cx) {
  float a2, b2, c2, ab2, ac2, bc2;
  // Count useful variables
  a2 = param_vec_.x() * param_vec_.x();
  b2 = param_vec_.y() * param_vec_.y();
  c2 = param_vec_.z() * param_vec_.z();
  ab2 = a2 * b2;
  ac2 = a2 * c2;
  bc2 = b2 * c2;

  // Count parameters
  *ax = ray.direction_unit_vec_.x() * ray.direction_unit_vec_.x() * bc2 + \
        ray.direction_unit_vec_.y() * ray.direction_unit_vec_.y() * ac2 + \
        ray.direction_unit_vec_.z() * ray.direction_unit_vec_.z() * ab2 * e1;
  *bx = ((ray.origin_coord_.x() - origin_coord_.x()) * ray.direction_unit_vec_.x() * bc2  + \
         (ray.origin_coord_.y() - origin_coord_.y()) * ray.direction_unit_vec_.y() * ac2  + \
         (ray.origin_coord_.z() - origin_coord_.z()) * ray.direction_unit_vec_.z() * ab2 * e1) * 2;
  *cx = (ray.origin_coord_.x() * ray.origin_coord_.x() + origin_coord_.x() * origin_coord_.x() - 2 * ray.origin_coord_.x() * origin_coord_.x()) * bc2 + \
        (ray.origin_coord_.y() * ray.origin_coord_.y() + origin_coord_.y() * origin_coord_.y() - 2 * ray.origin_coord_.y() * origin_coord_.y()) * ac2 + \
        (ray.origin_coord_.z() * ray.origin_coord_.z() + origin_coord_.z() * origin_coord_.z() - 2 * ray.origin_coord_.z() * origin_coord_.z()) * ab2 * e1 - ab2 * c2 * e2;
}

void Quadric::count_eq_type2(Ray ray, int e1, int e2, int e3, float *ax, float *bx, float *cx) {
  float a2, b2, ab2;
  // Count useful variables
  a2 = param_vec_.x() * param_vec_.x();
  b2 = param_vec_.y() * param_vec_.y();
  ab2 = a2 * b2;

  // Count parameters
  *ax = ray.direction_unit_vec_.x() * ray.direction_unit_vec_.x() * b2 + \
        ray.direction_unit_vec_.y() * ray.direction_unit_vec_.y() * a2 * e1;
  *bx = (ray.origin_coord_.x() - origin_coord_.x()) * ray.direction_unit_vec_.x() * b2 * 2 + \
        (ray.origin_coord_.y() - origin_coord_.y()) * ray.direction_unit_vec_.y() * a2 * 2 * e1 + \
        ray.direction_unit_vec_.z() * ab2 * e2;
  *cx = (ray.origin_coord_.x() * ray.origin_coord_.x() + origin_coord_.x() * origin_coord_.x() - 2 * ray.origin_coord_.x() * origin_coord_.x()) * b2 + \
        (ray.origin_coord_.y() * ray.origin_coord_.y() + origin_coord_.y() * origin_coord_.y() - 2 * ray.origin_coord_.y() * origin_coord_.y()) * a2 * e1 + \
        (ray.origin_coord_.z() - origin_coord_.z()) * ab2 * e2 - ab2 * e3;
}