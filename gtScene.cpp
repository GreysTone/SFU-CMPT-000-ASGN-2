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
}

void GTScene::addLight(GTLight arg) {
  lightList.push_back(arg);
}

void GTScene::addModel(GTSphere arg) {
  modelList.push_back(arg);
}

void GTScene::buildUserScene() { }

void GTScene::buildDefaultScene() {
  background_color = vec3(0.5, 0.05, 0.8);
  global_ambient = vec3(0.2, 0.2, 0.2);
  decay = vec3(0.5, 0.3, 0.0);

  GTLight light1;
  light1.position = vec3(-2.0, 5.0, 1.0);
  light1.intensity = vec3(1.0, 1.0, 1.0);
  // add light
  addLight(light1);

  GTSphere sphere[3];
  // sphere 1
  sphere[0].position = vec3(1.5, -0.2, -3.2);
  sphere[0].radius = 1.23;
  sphere[0].ambient = vec3(0.7, 0.7, 0.7);
  sphere[0].diffuse = vec3(0.1, 0.5, 0.8);
  sphere[0].specular = vec3(1.0, 1.0, 1.0);
  sphere[0].shineness = 10;
  sphere[0].reflectance = 0.4;
  // sphere 2
  sphere[1].position = vec3(-1.5, 0.0, -3.5);
  sphere[1].radius = 1.5;
  sphere[1].ambient = vec3(0.6, 0.6, 0.6);
  sphere[1].diffuse = vec3(1.0, 0.0, 0.25);
  sphere[1].specular = vec3(1.0, 1.0, 1.0);
  sphere[1].shineness = 6;
  sphere[1].reflectance = 0.3;
  // sphere 3
  sphere[2].position = vec3(-0.35, 1.75, -2.25);
  sphere[2].radius = 0.5;
  sphere[2].ambient = vec3(0.2, 0.2, 0.2);
  sphere[2].diffuse = vec3(0.0, 1.0, 0.25);
  sphere[2].specular = vec3(0.0, 1.0, 0.0);
  sphere[2].shineness = 30;
  sphere[2].reflectance = 0.3;
  // add sphere
  for (int i = 0; i < 3; i++) {
    addModel(sphere[i]);
  }
}

/*********************************************************************
 * This function returns a pointer to the sphere object that the
 * ray intersects first; NULL if no intersection. You should decide
 * which arguments to use for the function. For example, note that you
 * should return the point of intersection to the calling function.
 **********************************************************************/
bool GTScene::intersectScene(vec3 eye, vec3 ray, int i, int j) {
//  std::cout << "eye:";
//  GTCalc::printVector(eye);
//  std::cout << "ray:";
//  GTCalc::printVector(ray);


  std::vector<Match> matchBox;
  for (std::list<GTSphere>::iterator it = modelList.begin(); it != modelList.end(); ++it) {
    Match matcher;
    matcher.value = (*it).intersect(eye, ray, &(matcher.point));
    matcher.it = it;
//    GTCalc::printVector(matcher.point);
//    std::cout << "\t" << matcher.value << "\n";
    if(matcher.value >= GTCalc::precision /*&& /*matcher.value <= OVER_RANGE && */
        /*finite(matcher.value) && !isnan(matcher.value)*/)   matchBox.push_back(matcher);
  }
//  std::cout << "\n";

  vec3 minPoint;
  float minValue;
  std::list<GTSphere>::iterator minIt;
  if(!matchBox.empty()) {
    minPoint = matchBox.begin()->point;
    minValue = matchBox.begin()->value;
    minIt = matchBox.begin()->it;
    for(std::vector<Match>::iterator it = matchBox.begin(); it != matchBox.end(); ++it) {
      if(it == matchBox.begin()) continue;
      if(it->value < minValue) {
        minPoint = it->point;
        minValue = it->value;
        minIt = it->it;
      }
    }
  } else {
    minPoint = vec3(0.0, 0.0, 0.0);
    minValue = -1;
    minIt = modelList.end();
  }

  if(i != JMP || j != JMP) {
    depthPoint[i][j] = minPoint;
    depthValue[i][j] = minValue;
    depthObject[i][j] = minIt;
  }
  if(minIt == modelList.end()) return false;
  else return true;
}