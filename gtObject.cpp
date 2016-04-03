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

float dot(vec3 v1, vec3 v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

bool lowerequal(vec3 v1, vec3 v2) {
  if (v1.x <= v2.x && v1.y <= v2.y && v1.z <= v2.z) return true;
  else return false;
}

bool greaterequal(vec3 v1, vec3 v2) {
  if (v1.x >= v2.x && v1.y >= v2.y && v1.z >= v2.z) return true;
  else return false;
}


} // namespace GTCalc

vec3 GTModel::getNormal(vec3 surfPoint) {
  vec3 empty = vec3(0.0, 0.0, 0.0);
  return empty;
}

float GTModel::intersect(vec3 eye, vec3 ray, vec3 *hit, bool far) {
  return 1E+126;
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

bool GTModel::refractRay(vec3 inRay, vec3 inPoint, vec3 *outRay) {
  return false;
}

float GTModel::refracted(vec3 inRay, vec3 inPoint, vec3 *outRay, vec3 *outPoint) {
  return 0;
}

bool GTModel::isInCubeRange(vec3 min, vec3 max) {
  return false;
}

void GTModel::getIntersectingObject(std::vector<GTModel *> &container) {
  return ;
}

vec3 GTPlane::getNormal(vec3 surfPoint) {
  return normal;
}

/*
 * Ray: X = E + t(R)        : Eye, Ray
 * Plane: X * N = P * N     : Normal, Point
 * t(R) * N = (P - E) * N
 */
float GTPlane::intersect(vec3 eye, vec3 ray, vec3 *hit, bool far) {
  float Right = glm::dot((position - eye), normal);
  float Left = glm::dot(ray, normal);
  if (Left > -GTCalc::precision && Left < GTCalc::precision) return -1.0f;

  float t = Right / Left;
  if (t < GTCalc::precision) return -1.0f;

  vec3 point = eye + t * ray;
  vec3 cast = point - position;

  if (fabsf(glm::dot(cast, vec3(1, 0, 0))) > xLength / 2) return -1.0f;
  if (fabsf(glm::dot(cast, vec3(0, 1, 0))) > yLength / 2) return -1.0f;
  if (fabsf(glm::dot(cast, vec3(0, 0, 1))) > zLength / 2) return -1.0f;

  *hit = point;
  return t;
}


vec3 GTTriangle::getNormal(vec3 surfPoint) {
  return normal;
}

float GTTriangle::intersect(vec3 eye, vec3 ray, vec3 *hit, bool far) {
  vec3 &E1 = vector[0];
  vec3 &E2 = vector[1];
  vec3 D = glm::normalize(ray);
  vec3 &O = eye;
  vec3 P = glm::cross(D, E2);

  // determinant
  float det = glm::dot(E1, P);

  // keep det > 0, modify T accordingly
  vec3 T;
  if (det > 0)T = O - vertex[0];
  else {
    T = vertex[0] - O;
    det = -det;
  }

  // If determinant is near zero, ray lies in plane of triangle
  if (det < GTCalc::precision) return -1.0f;

  // Calculate u and make sure u <= 1
  float u = glm::dot(T, P);
  if (u < 0.0f || u > det) return -1.0f;

  // Q
  vec3 Q = glm::cross(T, E1);
  // Calculate v and make sure u + v <= 1
  float v = glm::dot(D, Q);
  if (v < 0.0f || u + v > det) return -1.0f;

  // Calculate t, scale parameters, ray intersects triangle
  float t = glm::dot(E2, Q);

  float fInvDet = 1.0f / det;
  t *= fInvDet;
//  u *= fInvDet;
//  v *= fInvDet;

  *hit = (O + t * D);
  return t;
}

float GTTriangle::refracted(vec3 inRay, vec3 inPoint, vec3 *outRay, vec3 *outPoint) {
  vec3 mid, out;
  vec3 endPoint;

  if (!refractRay(inRay, inPoint, &mid)) return -1.0f;
//  float inLength = intersect(inPoint, mid, &endPoint, true);
  if (!groupIntersect(inPoint, mid, &endPoint)) return -1.0f;

  if (!refractRay(mid, endPoint, &out)) return -1.0f;
  *outRay = out;
  *outPoint = endPoint;
  return 1.0f;
}

bool GTTriangle::refractRay(vec3 inRay, vec3 inPoint, vec3 *outRay) {
  vec3 normal = getNormal(inPoint);
  vec3 in = glm::normalize(-inRay);
  vec3 out;

  float refraction_index;
  if (glm::dot(in, normal) > GTCalc::precision) {
    refraction_index = 1 / refractiveIndex;
  } else {
    normal = -normal;
    refraction_index = refractiveIndex;
  }

  float m = glm::dot(in, normal);
  float a_square = 1 - pow(m, 2);
  float b_square = a_square * pow(refraction_index, 2);
  float n_square = 1 - b_square;

  if (n_square < GTCalc::precision) return false;
  float n = sqrt(n_square);

  out = normal * (m * refraction_index - n) - refraction_index * in;
  *outRay = out;
  return true;
}

void GTTriangle::setReference(std::list<GTModel *> *ref) {
  modelGroup = ref;
}

float GTTriangle::groupIntersect(vec3 eye, vec3 ray, vec3 *hit) {
  float value, minValue;
  vec3 tmpPoint, minPoint = vec3(0.0, 0.0, 0.0);
  std::list<GTModel *>::iterator tmpIt, minIt = modelGroup->end();
  bool setMatch = false;
  for (std::list<GTModel *>::iterator it = modelGroup->begin(); it != modelGroup->end(); ++it) {
    GTModel *object = (*it);
    value = object->intersect(eye, ray, &(tmpPoint), false);
    tmpIt = it;
    if (value < GTCalc::precision) continue;
    if (tmpPoint == eye) continue; //TODO: ignore self, not sure
    if (!setMatch || value < minValue) {
      setMatch = true;
      minValue = value;
      minPoint = tmpPoint;
      minIt = tmpIt;
    }
  }

  *hit = minPoint;
  if (minIt == modelGroup->end()) return false;
  else return true;
}

bool GTTriangle::isInCubeRange(vec3 min, vec3 max) {
  for(int i = 0; i < 3; i++) {
    if (GTCalc::lowerequal(vertex[i], max) && GTCalc::greaterequal(vertex[i], min))
      return true;
  }
  return false;
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
float GTSphere::intersect(vec3 eye, vec3 ray, vec3 *hit, bool far) {
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
      if (far) {
        *hit = eye + t1 * ray;
        return t1;
      } else {
        *hit = eye + t2 * ray;
        return t2;
      }
    }
  }
}

bool GTSphere::refractRay(vec3 inRay, vec3 inPoint, vec3 *outRay) {
  vec3 normal = getNormal(inPoint);
  vec3 in = glm::normalize(-inRay);
  vec3 out;

  float refraction_index;
  if (glm::dot(in, normal) > GTCalc::precision) {
    refraction_index = 1 / refractiveIndex;
  } else {
    normal = -normal;
    refraction_index = refractiveIndex;
  }

  float m = glm::dot(in, normal);
  float a_square = 1 - pow(m, 2);
  float b_square = a_square * pow(refraction_index, 2);
  float n_square = 1 - b_square;

  if (n_square < GTCalc::precision) return false;
  float n = sqrt(n_square);

  out = normal * (m * refraction_index - n) - refraction_index * in;
  *outRay = out;
  return true;
//  float root = 1 - pow(refraction_index,2) * ( 1 - pow( glm::dot(normal,in), 2) );
//
//  if( root < GTCalc::precision ) return false;
//
//  *outRay = normal * ( refraction_index * glm::dot(normal,in) - sqrt(root) ) - refraction_index * in;
//  return true;
}

float GTSphere::refracted(vec3 inRay, vec3 inPoint, vec3 *outRay, vec3 *outPoint) {
  vec3 mid, out;
  vec3 endPoint;

  if (!refractRay(inRay, inPoint, &mid)) return -1.0f;
  float inLength = intersect(inPoint, mid, &endPoint, true);
  if (inLength < GTCalc::precision) return -1.0f;

  if (!refractRay(mid, endPoint, &out)) return -1.0f;
  *outRay = out;
  *outPoint = endPoint;
  return 1.0f;
}


vec3 GTChessBoard::getAmbient(vec3 point) {
  vec3 range = point - position;
  int x = (int) floor(glm::dot(range, vec3(1, 0, 0)));
  int z = (int) floor(glm::dot(range, vec3(0, 0, 1)));
  if ((x + z) % 2 != 0)
    return vec3(0.3, 0.3, 0.3);
  else
    return vec3(1.0, 1.0, 1.0);
}

vec3 GTChessBoard::getDiffuse(vec3 point) {
  vec3 range = point - position;
  int x = (int) floor(glm::dot(range, vec3(1, 0, 0)));
  int z = (int) floor(glm::dot(range, vec3(0, 0, 1)));
  if ((x + z) % 2 != 0)
    return vec3(0.3, 0.3, 0.3);
  else
    return vec3(1.0, 1.0, 1.0);
}

vec3 GTChessBoard::getSpecular(vec3 point) {
  vec3 range = point - position;
  int x = (int) floor(glm::dot(range, vec3(1, 0, 0)));
  int z = (int) floor(glm::dot(range, vec3(0, 0, 1)));
  if ((x + z) % 2 != 0)
    return vec3(0.3, 0.3, 0.3);
  else
    return vec3(1.0, 1.0, 1.0);
}

float GTChessBoard::intersect(vec3 eye, vec3 ray, vec3 *hit, bool far) {
  float Right = glm::dot((position - eye), normal);
  float Left = glm::dot(ray, normal);
  if (Left > -GTCalc::precision && Left < GTCalc::precision) return -1.0f;

  float t = Right / Left;
  if (t < GTCalc::precision) return -1.0f;

  vec3 point = eye + t * ray;
  vec3 cast = point - position;

  if (fabsf(glm::dot(cast, vec3(1, 0, 0))) > xLength / 2) return -1.0f;
  if (fabsf(glm::dot(cast, vec3(0, 0, 1))) > zLength / 2) return -1.0f;

  *hit = point;
  return t;
}



void GTBoundary::getIntersectingObject(std::vector<GTModel *> &container) {
//  std::cout << "Hi Getting \n";
}


GTOctTree::GTOctTree() {
  isLeaf = true;
  space = NULL;
  xmin = xmax = ymin = ymax = zmin = zmax = 0;
}

GTOctTree::~GTOctTree() {
  if (space) delete[] space;
  for (std::vector<GTModel *>::iterator it; it != content.end(); it++) {
    delete (*it);
  }
  space = NULL;
}

void GTOctTree::setRange(float xi, float xa, float yi, float ya, float zi, float za) {
  xmin = xi;
  xmax = xa;
  ymin = yi;
  ymax = ya;
  zmin = zi;
  zmax = za;
  cubeMin = vec3(xi, yi, zi);
  cubeMax = vec3(xa, ya, za);
}

void GTOctTree::splitSpace(int step) {
  if(step < 1) return;
  space = new GTOctTree[8];
  isLeaf = false;

  float xmid = (xmin + xmax) / 2;
  float ymid = (ymin + ymax) / 2;
  float zmid = (zmin + zmax) / 2;
#ifdef OCT_OUTPUT
  std::cout << "(" << step << ")new split on X: " << xmid << std::endl;
  std::cout << "(" << step << ")new split on Y: " << ymid << std::endl;
  std::cout << "(" << step << ")new split on Z: " << zmid << std::endl;
#endif

  space[0].setRange(xmin, xmid, ymin, ymid, zmin, zmid);
  space[1].setRange(xmin, xmid, ymin, ymid, zmid, zmax);
  space[2].setRange(xmin, xmid, ymid, ymax, zmin, zmid);
  space[3].setRange(xmin, xmid, ymid, ymax, zmid, zmax);
  space[4].setRange(xmid, xmax, ymin, ymid, zmin, zmid);
  space[5].setRange(xmid, xmax, ymin, ymid, zmid, zmax);
  space[6].setRange(xmid, xmax, ymid, ymax, zmin, zmid);
  space[7].setRange(xmid, xmax, ymid, ymax, zmid, zmax);

  for (std::vector<GTModel *>::iterator it = content.begin(); it != content.end(); ++it) {
    for (int i = 0; i < 8; i++) {
      if (((*it)->isInCubeRange(space[i].cubeMin, space[i].cubeMax)))
      {
        space[i].content.push_back((*it));
      }
    }
  }
#ifdef OCT_OUTPUT
  std::cout << "transfer " << content.size() << " node(s) into sub space\n";
#endif
  content.clear();

  // recursive split space
  for(int i = 0; i < 8; i++) {
#ifdef OCT_OUTPUT
    std::cout << "  " << space[i].content.size() << " node(s) in tree with index: " << i << std::endl;
#endif
    space[i].splitSpace(step - 1);
  }
}

void GTOctTree::addObject(GTModel *obj) {
  content.push_back(obj);
}

bool GTOctTree::isInRange(vec3 point) {
//  if(pos.x >= xmin && pos.x <= xmax &&
//      pos.y >= ymin && pos.y <= ymax &&
//      pos.z <= zmin && pos.y <= zmin)
//    return true;
//  else return false;
  vec3 min = vec3(xmin, ymin, zmin);
  vec3 max = vec3(xmax, ymax, zmax);
  if (GTCalc::lowerequal(point, max) && GTCalc::greaterequal(point, min))
    return true;
  else return false;
}



