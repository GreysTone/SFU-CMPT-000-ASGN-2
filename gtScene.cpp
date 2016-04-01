//
//  gtScene.cpp
//  RayCast
//
//  Created by GreysTone on 2016-03-27.
//  Copyright © 2016 Danyang Song (Arthur). All rights reserved.
//

#include "gtScene.hpp"

GTScene::GTScene() {
  null_color = vec3(0.0, 0.0, 0.0);
  eye_pos = vec3(0.0, 0.0, 0.0);
  image_plane = -1.5f;
}

GTScene::~GTScene() {
  //TODO: release vector
  for(std::list<GTModel *>::iterator it = modelList.begin(); it != modelList.end(); ++it) {
    delete (*it);
  }
}

void GTScene::addLight(GTLight arg) {
  lightList.push_back(arg);
}

/*void GTScene::addModel(GTSphere arg) {
  modelList.push_back(arg);
}*/
void GTScene::addModel(GTModel *arg) {
  modelList.push_back(arg);
}

void GTScene::buildUserScene(bool chessboard, bool refract) {
  buildDefaultScene(chessboard, refract);
}

void GTScene::buildDefaultScene(bool chessboard, bool refract) {
  background_color = vec3(0.5, 0.05, 0.8);
  global_ambient = vec3(0.2, 0.2, 0.2);
  decay = vec3(0.5, 0.3, 0.0);

  GTLight light1;
  light1.position = vec3(-2.0, 5.0, 1.0);
  light1.intensity = vec3(1.0, 1.0, 1.0);
  // add light
  addLight(light1);

  GTSphere *sphere = new GTSphere[3];
  // sphere 1
  sphere[0].position = vec3(1.5, -0.2, -3.2);
  sphere[0].radius = 1.23;
  sphere[0].ambient = vec3(0.7, 0.7, 0.7);
  sphere[0].diffuse = vec3(0.1, 0.5, 0.8);
  sphere[0].specular = vec3(1.0, 1.0, 1.0);
  sphere[0].shineness = 10;
  sphere[0].reflectance = 0.4;
  if(refract) {
    sphere[0].refract = true;
    sphere[0].refractance = 1.0;
    sphere[0].refractivity = 1.5;
  }
  // sphere 2
  sphere[1].position = vec3(-1.5, 0.0, -3.5);
  sphere[1].radius = 1.5;
  sphere[1].ambient = vec3(0.6, 0.6, 0.6);
  sphere[1].diffuse = vec3(1.0, 0.0, 0.25);
  sphere[1].specular = vec3(1.0, 1.0, 1.0);
  sphere[1].shineness = 6;
  sphere[1].reflectance = 0.3;
  if(refract) {
    sphere[1].refract = true;
    sphere[1].refractance = 1.0;
    sphere[1].refractivity = 2.0;
  }
  // sphere 3
  sphere[2].position = vec3(-0.35, 1.75, -2.25);
  sphere[2].radius = 0.5;
  sphere[2].ambient = vec3(0.2, 0.2, 0.2);
  sphere[2].diffuse = vec3(0.0, 1.0, 0.25);
  sphere[2].specular = vec3(0.0, 1.0, 0.0);
  sphere[2].shineness = 30;
  sphere[2].reflectance = 0.3;
  if(refract) {
    sphere[2].refract = true;
    sphere[2].refractance = 0.5;
    sphere[2].refractivity = 1.5;
  }
  for (int i = 0; i < 3; i++) {
    GTModel *model = &(sphere[i]);
    addModel(model);
  }

  //chessboard
  if(chessboard) {
    GTPlane *board = new GTPlane;
    board->position = vec3(0.0, -2.5, -3.5);
    board->normal = vec3(0,1,0);
    board->xAxis = vec3(1,0,0);
    board->zAxis = vec3(0,0,1);
    board->xLength = 8;
    board->zLength = 8;
    board->ambient = vec3(0.1, 0.1, 0.1);
    board->diffuse = vec3(0,0,0);
    board->specular = vec3(1.0, 1.0, 1.0);
    board->shineness = 40;
    board->reflectance = 1.0;
    if(refract) {
      board->refract = false;
    }
    GTModel *model = board;
    addModel(model);
  }
}

/*********************************************************************
 * This function returns a pointer to the sphere object that the
 * ray intersects first; NULL if no intersection. You should decide
 * which arguments to use for the function. For example, note that you
 * should return the point of intersection to the calling function.
 **********************************************************************/
bool GTScene::intersectScene(vec3 eye, vec3 ray, Match *result, std::list<GTModel *>::iterator ignore) {
  Match tmpMatch, minMatch;
  minMatch.itor = modelList.end();  // set exit boundary
  bool setMatch = false;
  for (std::list<GTModel *>::iterator it = modelList.begin(); it != modelList.end(); ++it) {
    if(it == ignore) continue;
    GTModel *object = (*it);
    tmpMatch.value = object->intersect(eye, ray, &(tmpMatch.point), false);
    tmpMatch.itor = it;
    if(tmpMatch.value < GTCalc::precision) continue;
    if(!setMatch || tmpMatch.value < minMatch.value) {
      setMatch = true;
      minMatch = tmpMatch;
    }
  }

  *result = minMatch;

  if(minMatch.itor == modelList.end()) return false;
  else return true;
}