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

typedef struct match {
  vec3 point;
  float value;
  std::list<GTModel *>::iterator itor;
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
  std::list<GTModel *> modelList;

  GTScene();

  ~GTScene();

  void addLight(GTLight arg);
  void addModel(GTModel *arg);

  void buildUserScene();
  void buildDefaultScene();

  bool intersectScene(vec3 eye, vec3 ray, Match *result, std::list<GTModel *>::iterator ignore);
};

#endif /* gtScene_hpp */
