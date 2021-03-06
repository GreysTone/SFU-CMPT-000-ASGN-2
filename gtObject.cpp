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

void GTModel::getIntersectingObject(vec3 eye, vec3 ray, std::vector<GTModel *> *container) {
  return;
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
  // fast detected
  for (int i = 0; i < 3; i++) {
    if (GTCalc::lowerequal(vertex[i], max) && GTCalc::greaterequal(vertex[i], min))
      return true;
  }
//  return false;

  GTBoundary testCube;
  float xl = max.x - min.x;
  float yl = max.y - min.y;
  float zl = max.z - min.z;
  float cx = (min.x + max.x) / 2;
  float cy = (min.y + max.y) / 2;
  float cz = (min.z + max.z) / 2;
  vec3 xaxis(1, 0, 0), yaxis(0, 1, 0), zaxis(0, 0, 1);

  testCube.box[0].position = vec3(cx, cy, max.z);
  testCube.box[0].normal = zaxis;
  testCube.box[1].position = vec3(cx, cy, min.z);
  testCube.box[1].normal = -zaxis;
  testCube.box[2].position = vec3(max.x, cy, cz);
  testCube.box[2].normal = xaxis;
  testCube.box[3].position = vec3(min.x, cy, cz);
  testCube.box[3].normal = -xaxis;
  testCube.box[4].position = vec3(cx, max.y, cz);
  testCube.box[4].normal = yaxis;
  testCube.box[5].position = vec3(cx, min.y, cz);
  testCube.box[5].normal = -yaxis;

  for(int i = 0; i < 6; i++) {
    testCube.box[i].xLength = xl;
    testCube.box[i].yLength = yl;
    testCube.box[i].zLength = zl;
  }

  vec3 ray[6], eye[6];
  ray[0] = vertex[1] - vertex[0];  eye[0] = vertex[0];
  ray[1] = vertex[2] - vertex[0];  eye[1] = vertex[0];
  ray[2] = vertex[0] - vertex[1];  eye[2] = vertex[1];
  ray[3] = vertex[2] - vertex[1];  eye[3] = vertex[1];
  ray[4] = vertex[0] - vertex[2];  eye[4] = vertex[2];
  ray[5] = vertex[1] - vertex[2];  eye[5] = vertex[2];

  for(int i = 0; i < 6; i++) {
    if(testCube.isRayIntersected(eye[i], ray[i]))
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


void GTBoundary::getIntersectingObject(vec3 eye, vec3 ray, std::vector<GTModel *> *container) {
  // InBox & OutBox Point;
  std::vector<vec3> hitSerial;
  std::map<int, float> hitFaceContainer;
  for (int i = 0; i < 6; i++) {
    vec3 hit;
    float t = box[i].intersect(eye, ray, &hit, 0);
    if (t > GTCalc::precision) {
      hitFaceContainer.insert(std::pair<int, float>(i, t));
      hitSerial.push_back(hit);
    }
  }

  if(hitSerial.empty()) return ;
  vec3 midPoint = hitSerial[1] - hitSerial[0];
  midPoint *= 0.5;
  midPoint += hitSerial[0];

  // Only consider the first OCT space structure
  for (int i = 0; i < 8; i++) {
    if (root.space[i].containPoint(hitSerial[0])
        || root.space[i].containPoint(midPoint)
        || root.space[i].containPoint(hitSerial[1])) {
      for (int j = 0; j < (int) root.space[i].content.size(); j++)
        container->push_back(root.space[i].content[j]);
    }
  }

  //TODO: this routine contains a problem
  /*
//  std::map<int, float> &hFC = hitFaceContainer;
//  if (hFC.empty()) return;
//  for (std::map<int, float>::iterator it = hFC.begin(); it != hFC.end(); it++) {
//    vec3 normal = box[it->first].normal;
//    // z-axis
//    if (normal == vec3(0, 0, 1) || normal == vec3(0, 0, -1)) {
//      float t = it->second;
//      int block = pow(2, OCTTREE_SPLIT_STEP);
//      float interval = (root.zmax - root.zmin) / block;
//      vec3 nD = glm::normalize(ray);
////        vec3 inter = (normal == vec3(0, 0, 1))?vec3(0, 0, root.zmin):vec3(0, 0, root.zmax);
//      for (int i = 0; i <= block; i++) {
////          if(normal == vec3(0,0,1)) inter += vec3(0, 0, i * interval);
////          else  inter += vec3(0, 0, -i * interval);
//        vec3 inter = vec3(0, 0, root.zmin + i * interval);
//        float offset = -(glm::dot(inter, normal) / glm::dot(nD, normal));
//        vec3 newPoint = eye + (t + offset) * nD;
//        if (root.isInRange(newPoint))
////          hitSerial.insert(std::pair<vec3, float>(newPoint, (t + offset)));
//          hitSerial.push_back(newPoint);
//      }
//    }
//    // x-axis
//    if (normal == vec3(1, 0, 0) || normal == vec3(-1, 0, 0)) {
//      float t = it->second;
//      int block = pow(2, OCTTREE_SPLIT_STEP);
//      float interval = (root.xmax - root.xmin) / block;
//      vec3 nD = glm::normalize(ray);
//      for (int i = 0; i <= block; i++) {
//        vec3 inter = vec3(root.xmin + i * interval, 0, 0);
//        float offset = -(glm::dot(inter, normal) / glm::dot(nD, normal));
//        vec3 newPoint = eye + (t + offset) * nD;
//        if (root.isInRange(newPoint))
//          hitSerial.push_back(newPoint);
//      }
//    }
//    // y-axis
//    if (normal == vec3(0, 1, 0) || normal == vec3(0, -1, 0)) {
//      float t = it->second;
//      int block = pow(2, OCTTREE_SPLIT_STEP);
//      float interval = (root.ymax - root.ymin) / block;
//      vec3 nD = glm::normalize(ray);
//      for (int i = 0; i <= block; i++) {
//        vec3 inter = vec3(0, root.ymin + i * interval, 0);
//        float offset = -(glm::dot(inter, normal) / glm::dot(nD, normal));
//        vec3 newPoint = eye + (t + offset) * nD;
//        if (root.isInRange(newPoint))
//          hitSerial.push_back(newPoint);
//      }
//    }
//  }

//  if(hitSerial.size() > 1)
//    std::cout << hitSerial.size() << std::endl;

// 2 locate tree
// 3 adding object to container
//  for(std::vector<vec3>::iterator it = hitSerial.begin(); it != hitSerial.end(); ++it)
//  {
//    GTOctTree *subTree = locateTree(&root, (*it));
//    for (int i = 0; i < (int)subTree->content.size(); i++)
//      container->push_back(subTree->content[i]);
//  }
*/
}

GTOctTree *GTBoundary::locateTree(GTOctTree *tree, vec3 position) {
  if (tree->isLeaf) return tree;
  for (int i = 0; i < 8; i++) {
    if (tree->space[i].containPoint(position))
      return locateTree(&(tree->space[i]), position);
  }
  return NULL;
}

bool GTBoundary::isRayIntersected(vec3 eye, vec3 ray) {
  vec3 hit;
  for (int i = 0; i < 6; i++) {
    float t = box[i].intersect(eye, ray, &hit, 0);
    if (t > GTCalc::precision) {
      return true;
    }
  }
  return false;
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
  if (step < 1) return;
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
      vec3 loose = vec3(0.1, 0.1, 0.1);
      if (((*it)->isInCubeRange(space[i].cubeMin-loose, space[i].cubeMax+loose))) {
        space[i].content.push_back((*it));
      }
    }
  }
#ifdef OCT_OUTPUT
  std::cout << "transfer " << content.size() << " node(s) into sub space\n";
#endif
#ifdef OCT_REDUCE_MEM
  content.clear();
#endif

  // recursive split space
  for (int i = 0; i < 8; i++) {
#ifdef OCT_OUTPUT
    std::cout << "  " << space[i].content.size() << " node(s) in tree with index: " << i << std::endl;
#endif
    space[i].splitSpace(step - 1);
  }
}

void GTOctTree::addObject(GTModel *obj) {
  content.push_back(obj);
}

bool GTOctTree::containPoint(vec3 point) {
  vec3 min = vec3(xmin, ymin, zmin);
  vec3 max = vec3(xmax, ymax, zmax);
  if (GTCalc::lowerequal(point, max) && GTCalc::greaterequal(point, min))
    return true;
  else return false;
}