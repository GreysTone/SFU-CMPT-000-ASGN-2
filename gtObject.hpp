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
#include <list>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

using namespace glm;

namespace GTCalc {
  extern float precision;

  void printVector(vec3 v);
  inline float dot(vec3 v1, vec3 v2);
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
protected:
  std::list<GTModel *> *modelGroup;
public:
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shineness;
  float reflectance;

  bool refract;
  float refractivity;
  float refractance;

  virtual vec3 getNormal(vec3 surfPoint);
  virtual vec3 getAmbient(vec3 point);
  virtual vec3 getDiffuse(vec3 point);
  virtual vec3 getSpecular(vec3 point);
  virtual float intersect(vec3 eye, vec3 ray, vec3 *hit, bool far);
  virtual float refracted(vec3 inRay, vec3 inPoint, vec3 *outRay, vec3* outPoint);
  virtual bool refractRay(vec3 inRay, vec3 inPoint, vec3 *outRay);
};

class GTSphere : public GTModel {
public:
  float radius;

  vec3 getNormal(vec3 surfPoint);
  vec3 getAmbient(vec3 point);
  vec3 getDiffuse(vec3 point);
  vec3 getSpecular(vec3 point);
  float intersect(vec3 eye, vec3 ray, vec3 *hit, bool far);
  float refracted(vec3 inRay, vec3 inPoint, vec3 *outRay, vec3* outPoint);
  bool refractRay(vec3 inRay, vec3 inPoint, vec3 *outRay);
};

class GTPlane : public GTModel {
public:
  vec3 normal;
  vec3 xAxis;
  vec3 zAxis;
  int xLength;
  int zLength;

  vec3 getNormal(vec3 surfPoint);
  vec3 getAmbient(vec3 point);
  vec3 getDiffuse(vec3 point);
  vec3 getSpecular(vec3 point);
  float intersect(vec3 eye, vec3 ray, vec3 *hit, bool far);
  float refracted(vec3 inRay, vec3 inPoint, vec3 *outRay, vec3* outPoint);
  bool refractRay(vec3 inRay, vec3 inPoint, vec3 *outRay);
};

class GTTriangle : public GTModel {
public:
  vec3 vertex[3];
  vec3 vector[2];
  vec3 normal;

  vec3 getNormal(vec3 surfPoint);
  vec3 getAmbient(vec3 point);
  vec3 getDiffuse(vec3 point);
  vec3 getSpecular(vec3 point);
  float intersect(vec3 eye, vec3 ray, vec3 *hit, bool far);
  float refracted(vec3 inRay, vec3 inPoint, vec3 *outRay, vec3* outPoint);
  bool refractRay(vec3 inRay, vec3 inPoint, vec3 *outRay);

  void setReference(std::list<GTModel *> *ref);
  float groupIntersect(vec3 inPoint, vec3 inRay, vec3* hit);
};

#endif /* gtObject_hpp */