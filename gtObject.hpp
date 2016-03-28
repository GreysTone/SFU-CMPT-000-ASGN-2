//
//  gtObject.hpp
//  RayCast
//
//  Created by GreysTone on 2016-03-27.
//  Copyright © 2016 Danyang Song (Arthur). All rights reserved.
//

#ifndef gtObject_hpp
#define gtObject_hpp

#include <stdio.h>
#include <iostream>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

using namespace glm;

namespace GTClac {
  extern float precision;

  void printVector(vec3 v);
}

class GTObject {
public:
  vec3 position;
};

class GTLight : public GTObject {
public:
  vec3 intensity;
};

class GTModel : public GTObject {
public:
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shineness;
  float reflectance;

  virtual bool intersect(vec3 eye, vec3 ray, vec3 *hit);
};

class GTSphere : public GTModel {
public:
  float radius;

  vec3 normal(vec3 surfPoint);
  bool intersect(vec3 eye, vec3 ray, vec3 *hit, float *value);
};

#endif /* gtObject_hpp */
