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


vec3 GTSphere::normal(vec3 surfPoint) {
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
//  GTCalc::printVector(position);
//  GTCalc::printVector(eye);
//  vec3 a = this->position - eye;
//  float a_square = glm::dot(a, a);
//  std::cout<<a_square<<std::endl;
//  vec3 normal_ray = glm::normalize(ray);
//  GTCalc::printVector(normal_ray);
//  float l = glm::dot(normal_ray, a);
//  std::cout<<l<<std::endl;
//  float l_square = l * l;
//  float r_square = this->radius * this->radius;
////  std::cout << a_square << " " << l << " " << r_square << "\n";
//  if(a_square > r_square && l < 0.0) {
//    *hit = vec3(0.0, 0.0, 0.0);
//    *value = -1.0f;
//    return false;
//  }
//
//  float m_square = a_square - l_square;
//  if(m_square > r_square) {
//    *hit = vec3(0.0, 0.0, 0.0);
//    *value = -1.0f;
//    return false;
//  }
//
//  float q_square = r_square - m_square;
//  float q = sqrt(q_square);
//  float t;
//  if(a_square > r_square) t = l - q;
//  else t = l + q;
////  std::cout << "l:" << l << " q:" << q << " t:" << t << std::endl;
//  vec3 offset = t * normal_ray + eye;
////  GTCalc::printVector(offset);
//  *hit = offset;
////  GTCalc::printVector(offset);
//  *value = glm::dot(offset, offset);
////  std::cout << *value << std::endl;
//  return true;
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
//      return GTCalc::dot(*hit, *hit);
      return t;
    }
  } else { // two root
    float t1 = ((-b) + sqrt(delta)) / (2 * a);
    float t2 = ((-b) - sqrt(delta)) / (2 * a);
    if (t1 < -GTCalc::precision) {
      return -1.0f;
    } else if (t2 < -GTCalc::precision) {
      *hit = eye + t1 * ray;
//      return GTCalc::dot(*hit, *hit);
      return t1;
    } else {
      *hit = eye + t2 * ray;
//      return GTCalc::dot(*hit, *hit);
      return t2;
    }
  }
}



