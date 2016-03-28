//
//  gtObject.cpp
//  RayCast
//
//  Created by GreysTone on 2016-03-27.
//  Copyright © 2016 Danyang Song (Arthur). All rights reserved.
//

#include "gtObject.hpp"

namespace GTClac {
float precision = 1E-6;

void printVector(vec3 v) {
  float length = glm::dot(v, v);
  std::cout << "\tX(R):" << v.x
            << "\tY(G):" << v.y
            << "\tZ(B):" << v.z
            << "\tLEN:" << length << std::endl;
}


}

bool GTModel::intersect(vec3 eye, vec3 ray, vec3 *hit) {
  return false;
}


vec3 GTSphere::normal(vec3 surfPoint) {
  vec3 rc = surfPoint - this->position;
  GTClac::printVector(rc);
  rc = glm::normalize(rc);
  return rc;
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
bool GTSphere::intersect(vec3 eye, vec3 ray, vec3 *hit, float *value) {
  GTClac::printVector(position);
  GTClac::printVector(eye);
  vec3 a = this->position - eye;
  float a_square = glm::dot(a, a);
  vec3 normal_ray = glm::normalize(ray);
  float l = glm::dot(normal_ray, a);
  float l_square = l * l;
  float r_square = this->radius * this->radius;
  std::cout << a_square << " " << l << " " << r_square << "\n";
  if(a_square > r_square && l < 0.0) {
    *hit = vec3(0.0, 0.0, 0.0);
    *value = -1.0f;
    return false;
  }

  float m_square = a_square -  l_square;
  if(m_square > r_square) {
    *hit = vec3(0.0, 0.0, 0.0);
    *value = -1.0f;
    return false;
  }

  float q_square = r_square - m_square;
  float q = sqrt(q_square);
  float t;
  if(a_square > r_square) t = l - q;
  else t = l + q;
  vec3 offset = t * normal_ray + eye;
//  GTClac::printVector(offset);
  *hit = offset;
  *value = glm::dot(offset, offset);
  return true;
}



