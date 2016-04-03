//
//  gtScene.hpp
//  RayCast
//
//  Created by GreysTone on 2016-03-27.
//  Copyright © 2016 Danyang Song (Arthur). All rights reserved.
//

#ifndef gtScene_hpp
#define gtScene_hpp

#include <stdio.h>
#include <list>
#include <vector>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "gtObject.hpp"
#include "global.h"
#include <omp.h>
#include <algorithm>

using namespace glm;

#define SHOW_PROGRESS

typedef struct match {
  vec3 point;
  float value;
  std::vector<GTModel *>::iterator itor;
} Match;

class GTScene {
  vec3 null_color;

  void buildModelFromFile(FILE *fp, bool refract, vec3 translation, float scale);
  void buildFastModelFromFile(FILE *fp, bool refract, vec3 translation, float scale);
  void buildBoundaryWithRange(float minx, float maxx, float miny, float maxy, float minz, float maxz);
public:
  vec3 background_color;

  vec3 eye_pos;
  float image_plane;

  vec3 global_ambient;
  vec3 decay;

  std::list<GTLight> lightList;
  std::vector<GTModel *> modelList;

  GTTriangle *meshListCycle;

  GTScene();

  ~GTScene();

  void addLight(GTLight arg);
  void addModel(GTModel *arg);

  void buildUserScene(bool chessboard, bool refract);
  void buildDefaultScene(bool chessboard, bool refract);
  void buildBonusScene(bool fastbonus, bool refract);

  bool intersectScene(vec3 eye, vec3 ray, Match *result, std::vector<GTModel *>::iterator ignore);
};

#endif /* gtScene_hpp */
