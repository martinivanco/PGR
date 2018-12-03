#pragma once

#include "scene_base.h"

class Quadric : public SceneObject {
 public:
  Quadric(V3 origin_coord, V3 param_vec, int quadric_type, Material* material, float bounding_box_range);
  Intersection intersect(Ray ray);

 private:
  void count_eq_type1(Ray ray, int e1, int e2, float *ax, float *bx, float *cx);
  void count_eq_type2(Ray ray, int e1, int e2, int e3, float *ax, float *bx, float *cx);
  bool check_bounding_box(V3 p);

  V3 origin_coord_;
  V3 param_vec_;
  int quadric_type_;
  float bounding_box_range_;
};