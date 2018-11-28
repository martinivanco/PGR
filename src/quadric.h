#pragma once

#include "scene_base.h"

class Quadric : public SceneObject {
 public:
  Quadric(V3 origin_coord, V3 param_vec, int quadric_type, Material* material);
   
  Intersection intersect(Ray ray);

 private:
  V3 origin_coord_;
  V3 param_vec_;
  int quadric_type_;
};