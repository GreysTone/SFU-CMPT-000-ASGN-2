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
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "gtObject.hpp"
#include "global.h"

using namespace glm;

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
  void intersectScene(vec3 eye, vec3 ray, int i, int j);
};

#endif /* gtScene_hpp */
