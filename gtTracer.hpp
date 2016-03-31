//
//  gtTracer.hpp
//  RayCast
//
//  Created by GreysTone on 2016-03-27.
//  Copyright © 2016 Danyang Song (Arthur). All rights reserved.
//

#ifndef gtTracer_hpp
#define gtTracer_hpp

#include <GL/glew.h>
#include <stdio.h>
#include <GL/glut.h>
#include <string.h>
#include "gtScene.hpp"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtx/rotate_vector.hpp"

#include "global.h"

using namespace glm;

enum GTTracerSetting {
  NOUN = 0,
  USER_SCENE = 1,
  SHADOW = 2,
  REFLECTION = 4
};

class GTTracer {
  bool userSceneActived;
  bool shadowActived;
  bool reflectionActived;
  int maxStep;

  GTScene *scene;

  vec3 getReflectionVector(vec3 pointEye, vec3 pointSurf, std::list<GTSphere>::iterator object);

  vec3 phong(vec3 pointSurf, vec3 vecProject, GTLight light, std::list<GTSphere>::iterator obj);
  vec3 recursive_ray_trace(vec3 ray, GTLight light);
public:
  int win_width;
  int win_height;
  GLfloat frame[WIN_HEIGHT][WIN_WIDTH][3];
  float image_width;
  float image_height;

  GTTracer();

  void setMaxStep(int arg);

  void setConfiguration(GTTracerSetting arg);

  void buildScene();

  void traceRay();

  void saveImage();

  void normalizeHistogram();
};

#endif /* gtTracer_hpp */
