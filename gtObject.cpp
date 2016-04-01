//
//  gtObject.cpp
//  RayCast
//
//  Created by GreysTone on 2016-03-27.
//  Copyright © 2016 Danyang Song (Arthur). All rights reserved.
//

#include "gtObject.hpp"

namespace GTCalc {
float precision = 1E-6;

void printVector(vec3 v) {
  float length = GTCalc::dot(v, v);
  std::cout << "\tX(R):" << v.x
  << "\tY(G):" << v.y
  << "\tZ(B):" << v.z
  << "\tLEN:" << length << std::endl;
}

inline float dot(vec3 v1, vec3 v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

}

vec3 GTModel::getNormal(vec3 surfPoint) {
  vec3 empty = vec3(0.0, 0.0, 0.0);
  return empty;
}

float GTModel::intersect(vec3 eye, vec3 ray, vec3 *hit) {
  return 0;
}

vec3 GTModel::getAmbient(vec3 point) {
  return ambient;
}

vec3 GTModel::getDiffuse(vec3 point) {
  return diffuse;
}

vec3 GTModel::getSpecular(vec3 point) {
  return specular;
}


vec3 GTSphere::getNormal(vec3 surfPoint) {
  return glm::normalize(surfPoint - this->position);
}

/**********************************************************************
 * This function intersects a ray with a given sphere 'sph'. You should
 * use the parametric representation of a line and do the intersection.
 * The function should return the parameter value for the intersection,
 * which will be compared with others to determine which intersection
 * is closest. The value -1.0 is returned if there is no intersection
 *
 * If there is an intersection, the point of intersection should be
 * stored in the "hit" variable
 **********************************************************************/
float GTSphere::intersect(vec3 eye, vec3 ray, vec3 *hit) {
  vec3 e_pos = eye - this->position;
  float r_square = this->radius * this->radius;

  float a = glm::dot(ray, ray);
  float b = 2 * glm::dot(e_pos, ray);
  float c = glm::dot(e_pos, e_pos) - r_square;
  float delta = b * b - 4 * a * c;
  if (delta < -GTCalc::precision) {
    return -1.0f;
  } else if (delta >= -GTCalc::precision && delta <= GTCalc::precision) {
    float t = (-b) / (2 * a);
    if (t < -GTCalc::precision) {
      return -1.0f;
    } else {
      *hit = eye + t * ray;
      return t;
    }
  } else { // two root
    float t1 = ((-b) + sqrt(delta)) / (2 * a);
    float t2 = ((-b) - sqrt(delta)) / (2 * a);
    if (t1 < -GTCalc::precision) {
      return -1.0f;
    } else if (t2 < -GTCalc::precision) {
      *hit = eye + t1 * ray;
      return t1;
    } else {
      *hit = eye + t2 * ray;
      return t2;
    }
  }
}

vec3 GTSphere::getAmbient(vec3 point) {
  return GTModel::getAmbient(point);
}

vec3 GTSphere::getDiffuse(vec3 point) {
  return GTModel::getDiffuse(point);
}

vec3 GTSphere::getSpecular(vec3 point) {
  return GTModel::getSpecular(point);
}


vec3 GTPlane::getNormal(vec3 surfPoint) {
  return glm::normalize(normal);
}

/*
 * Ray: X = E + t(R)        : Eye, Ray
 * Plane: X * N = P * N     : Normal, Point
 * t(R) * N = (P - E) * N
 */
float GTPlane::intersect(vec3 eye, vec3 ray, vec3 *hit) {
  float Right = glm::dot((position - eye), normal);
  float Left = glm::dot(ray, normal);
  if (Left > -GTCalc::precision && Left < GTCalc::precision) return -1.0f;

  float t = Right / Left;
  if (t < GTCalc::precision) return -1.0f;

  vec3 point = eye + t * ray;
  vec3 cast = point - position;

  if (fabsf(glm::dot(cast, xAxis)) > xLength / 2) return -1.0f;
  if (fabsf(glm::dot(cast, zAxis)) > zLength / 2) return -1.0f;

  *hit = point;
  return t;
}

vec3 GTPlane::getAmbient(vec3 point) {
  vec3 range = point - position;
  int x = (int) floor(glm::dot(range, xAxis));
  int z = (int) floor(glm::dot(range, zAxis));
  if ((x + z) % 2 == 0) return vec3(1.0, 1.0, 1.0);
  else return vec3(0.3, 0.3, 0.3);
}

vec3 GTPlane::getDiffuse(vec3 point) {
  vec3 range = point - position;
  int x = (int) floor(glm::dot(range, xAxis));
  int z = (int) floor(glm::dot(range, zAxis));
  if ((x + z) % 2 == 0) return glm::vec3(1.0, 1.0, 1.0);
  else return glm::vec3(0.0, 0.0, 0.0);
}

vec3 GTPlane::getSpecular(vec3 point) {
//  return GTModel::getSpecular(point);
  vec3 range = point - position;
  int x = (int) floor(glm::dot(range, xAxis));
  int z = (int) floor(glm::dot(range, zAxis));
  if ((x + z) % 2 == 0) return glm::vec3(1.0, 1.0, 1.0);
  else return glm::vec3(0.3, 0.3, 0.3);
}
