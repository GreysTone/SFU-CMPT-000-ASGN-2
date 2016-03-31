//
//  gtScene.hpp
//  RayCast
//
//  Created by GreysTone on 2016-03-27.
//  Copyright © 2016 Danyang Song (Arthur). All rights reserved.
//

#ifndef gtScene_hpp
#define gtScene_hpp

#include <list>
#include <vector>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "gtObject.hpp"
#include "global.h"

using namespace glm;

#define OVER_RANGE (1E+10)

typedef struct match {
  vec3 point;
  float value;
  std::list<GTSphere>::iterator it;
} Match;

class GTScene {
  vec3 null_color;
public:
  vec3 background_color;

  vec3 eye_pos;
  float image_plane;

  vec3 global_ambient;
  vec3 decay;

  std::list<GTLight> lightList;
  std::list<GTSphere> modelList;
  std::list<GTSphere>::iterator depthObject[WIN_HEIGHT][WIN_WIDTH];
  vec3 depthPoint[WIN_HEIGHT][WIN_WIDTH];
  float depthValue[WIN_HEIGHT][WIN_WIDTH];

  GTScene();

  ~GTScene();

  void addLight(GTLight arg);

  void addModel(GTSphere arg);

  void buildUserScene();
  void buildDefaultScene();

//  void intersectScene(vec3 eye, vec3 ray, vec3 *intersectPoint, float *value);
  bool intersectScene(vec3 eye, vec3 ray, int i, int j);
  bool intersectScene(vec3 eye, vec3 ray, Match *result);
};

#endif /* gtScene_hpp */
