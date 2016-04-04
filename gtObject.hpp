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
#include <vector>
#include <list>
#include <map>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "global.h"

using namespace glm;

namespace GTCalc {
extern float precision;

void printVector(vec3 v);

inline float dot(vec3 v1, vec3 v2);

inline bool lowerequal(vec3 v1, vec3 v2);

inline bool greaterequal(vec3 v1, vec3 v2);
} // namespace GTCalc

class GTObject {
public:
  vec3 position;

  virtual ~GTObject() { }
};

class GTLight : public GTObject {
public:
  vec3 intensity;

  ~GTLight() { }
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
  bool isRefractObject;
  float refractiveIndex;
  float transmissivity;

  bool isVirtual;
  bool isAccelerated;

  GTModel() {
    isVirtual = false;
    isAccelerated = false;
  };
  ~GTModel() { }

  virtual vec3 getNormal(vec3 surfPoint);

  virtual vec3 getAmbient(vec3 point);

  virtual vec3 getDiffuse(vec3 point);

  virtual vec3 getSpecular(vec3 point);

  virtual float intersect(vec3 eye, vec3 ray, vec3 *hit, bool far);
  virtual void getIntersectingObject(vec3 eye, vec3 ray, std::vector<GTModel *> *container);

  virtual float refracted(vec3 inRay, vec3 inPoint, vec3 *outRay, vec3 *outPoint);

  virtual bool refractRay(vec3 inRay, vec3 inPoint, vec3 *outRay);

  virtual bool isInCubeRange(vec3 min, vec3 max);
};

class GTTriangle : public GTModel {
public:
  vec3 vertex[3];
  vec3 vector[2];
  vec3 normal;

  ~GTTriangle() {
    std::cout << "Recycle Group\n";
    if (modelGroup) delete modelGroup;
  };

  vec3 getNormal(vec3 surfPoint);

  float intersect(vec3 eye, vec3 ray, vec3 *hit, bool far);

  float refracted(vec3 inRay, vec3 inPoint, vec3 *outRay, vec3 *outPoint);

  bool refractRay(vec3 inRay, vec3 inPoint, vec3 *outRay);

  bool isInCubeRange(vec3 min, vec3 max);

  void setReference(std::list<GTModel *> *ref);

  float groupIntersect(vec3 inPoint, vec3 inRay, vec3 *hit);
};

class GTPlane : public GTModel {
public:
  vec3 normal;
  float xLength;
  float yLength;
  float zLength;

  ~GTPlane() { }

  vec3 getNormal(vec3 surfPoint);

  virtual float intersect(vec3 eye, vec3 ray, vec3 *hit, bool far);
};

class GTSphere : public GTModel {
public:
  float radius;

  ~GTSphere() { }

  vec3 getNormal(vec3 surfPoint);

  float intersect(vec3 eye, vec3 ray, vec3 *hit, bool far);

  float refracted(vec3 inRay, vec3 inPoint, vec3 *outRay, vec3 *outPoint);

  bool refractRay(vec3 inRay, vec3 inPoint, vec3 *outRay);
};


class GTChessBoard : public GTPlane {
  ~GTChessBoard() { }

  vec3 getAmbient(vec3 point);

  vec3 getDiffuse(vec3 point);

  vec3 getSpecular(vec3 point);

  float intersect(vec3 eye, vec3 ray, vec3 *hit, bool far);
};

class GTOctTree {
public:
  bool isLeaf;
  float xmin, xmax;
  float ymin, ymax;
  float zmin, zmax;
  vec3 cubeMin, cubeMax;
  GTOctTree *space;
  std::vector<GTModel *> content;

  GTOctTree();

  ~GTOctTree();

  void setRange(float xi, float xa, float yi, float ya, float zi, float za);

  bool containPoint(vec3 point);

  void addObject(GTModel *obj);

  void splitSpace(int step);
};

class GTBoundary : public GTModel {
public:
  GTBoundary() {
    isVirtual = true;
  }

  GTPlane box[6];
  GTOctTree root;

  void getIntersectingObject(vec3 eye, vec3 ray, std::vector<GTModel *> *container);
  GTOctTree *locateTree(GTOctTree *tree, vec3 position);
};

#endif /* gtObject_hpp */