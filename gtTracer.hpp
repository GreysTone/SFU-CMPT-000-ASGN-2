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

#include "global.h"

using namespace glm;

enum GTTracerSetting {
  NOUN = 0,
  USER_SCENE = 1,
  SHADOW = 2
};

class GTTracer {
  bool userSceneActived;
  bool shadowActived;
  int maxStep;


/*********************************************************************
 * Phong illumination - you need to implement this!
 *********************************************************************/
  GTScene *scene;

  vec3 phong(vec3 q, vec3 v, vec3 surf_normal, GTObject *sph) {
    vec3 color;
    return color;
  }


/************************************************************************
 * This is the recursive ray tracer - you need to implement this!
 * You should decide what arguments to use.
 ************************************************************************/
  vec3 recursive_ray_trace() {
//
// do your thing here
//
    vec3 color;
    return color;
  }

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
