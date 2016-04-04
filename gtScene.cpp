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
  for (std::vector<GTModel *>::iterator it = modelList.begin(); it != modelList.end(); ++it) {
    delete (*it);
  }
  delete[] meshListCycle;
}

void GTScene::addLight(GTLight arg) {
  lightList.push_back(arg);
}

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
  if (refract) {
    sphere[0].isRefractObject = true;
    sphere[0].transmissivity = 1.0;
    sphere[0].refractiveIndex = 1.5;
  }
  // sphere 2
  sphere[1].position = vec3(-1.5, 0.0, -3.5);
  sphere[1].radius = 1.5;
  sphere[1].ambient = vec3(0.6, 0.6, 0.6);
  sphere[1].diffuse = vec3(1.0, 0.0, 0.25);
  sphere[1].specular = vec3(1.0, 1.0, 1.0);
  sphere[1].shineness = 6;
  sphere[1].reflectance = 0.3;
  if (refract) {
    sphere[1].isRefractObject = true;
    sphere[1].transmissivity = 1.0;
    sphere[1].refractiveIndex = 2.0;
  }
  // sphere 3
  sphere[2].position = vec3(-0.35, 1.75, -2.25);
  sphere[2].radius = 0.5;
  sphere[2].ambient = vec3(0.2, 0.2, 0.2);
  sphere[2].diffuse = vec3(0.0, 1.0, 0.25);
  sphere[2].specular = vec3(0.0, 1.0, 0.0);
  sphere[2].shineness = 30;
  sphere[2].reflectance = 0.3;
  if (refract) {
    sphere[2].isRefractObject = true;
    sphere[2].transmissivity = 0.5;
    sphere[2].refractiveIndex = 1.5;
  }
  for (int i = 0; i < 3; i++) {
    GTModel *model = &(sphere[i]);
    addModel(model);
  }

  //chessboard
  if (chessboard) {
    GTChessBoard *board = new GTChessBoard;
    board->position = vec3(0.0, -2.5, -3.5);
    board->normal = vec3(0, 1, 0);
    board->xLength = 8.0;
    board->zLength = 8.0;
    board->ambient = vec3(0.1, 0.1, 0.1);
    board->diffuse = vec3(0, 0, 0);
    board->specular = vec3(1.0, 1.0, 1.0);
    board->shineness = 40;
    board->reflectance = 1.0;
    if (refract) {
      board->isRefractObject = false;
    }
    GTModel *model = board;
    addModel(model);
  }
}

void GTScene::buildBonusScene(bool fastbonus, bool refract) {
  background_color = vec3(0.5, 0.05, 0.8);
  global_ambient = vec3(0.2, 0.2, 0.2);
  decay = vec3(0.5, 0.3, 0.0);

  GTLight light1;
  light1.position = vec3(-2.0, 5.0, 1.0);
  light1.intensity = vec3(1.0, 1.0, 1.0);
  // add light
  addLight(light1);

  if (!fastbonus) { // normal bonus
    FILE *fp1;
    FILE *fp2;
    char f1name[32], f2name[32];

    // chess1
    strcpy(f1name, "chess_pieces/bishop_hires.smf");
    printf("Opening %s\n", f1name);
    if (!(fp1 = fopen(f1name, "r"))) {
      printf("Unable to open file '%s'\n", f1name);
      return;
    }
    buildModelFromFile(fp1, refract, vec3(-1.0f, -1.0f, -2.0f), 20);
    fclose(fp1);

    // chess2
    strcpy(f2name, "chess_pieces/chess_hires.smf");
    printf("Opening %s\n", f2name);
    if (!(fp2 = fopen(f2name, "r"))) {
      printf("Unable to open file '%s'\n", f2name);
      return;
    }
    buildModelFromFile(fp2, refract, vec3(0.0f, -1.0f, -5.5f), 3);
    fclose(fp2);
  }
  else {  // fast bonus
    FILE *fp;
    char fname[32];
    strcpy(fname, "chess_pieces/chess_hires.smf");
    printf("Opening %s\n", fname);
    if (!(fp = fopen(fname, "r"))) {
      printf("Unable to open file '%s'\n", fname);
      return;
    }
    buildFastModelFromFile(fp, refract, vec3(0.0f, -1.5f, -5.5f), 3);
    fclose(fp);
  }

  //chessboard
  GTChessBoard *board = new GTChessBoard;
  board->position = vec3(0.0, -2.5, -3.5);
  board->normal = vec3(0, 1, 0);
  board->xLength = 32.0;
  board->zLength = 32.0;
  board->ambient = vec3(0.1, 0.1, 0.1);
  board->diffuse = vec3(0, 0, 0);
  board->specular = vec3(1.0, 1.0, 1.0);
  board->shineness = 40;
  board->reflectance = 1.0;
  if (refract) {
    board->isRefractObject = false;
  }
  GTModel *model = board;
  addModel(model);
}

/*********************************************************************
 * This function returns a pointer to the sphere object that the
 * ray intersects first; NULL if no intersection. You should decide
 * which arguments to use for the function. For example, note that you
 * should return the point of intersection to the calling function.
 **********************************************************************/
bool GTScene::intersectScene(vec3 eye, vec3 ray, Match *result, std::vector<GTModel *>::iterator ignore) {
  Match tmpMatch, minMatch;
  minMatch.itor = modelList.end();  // set exit boundary
  bool setMatch = false;
  for (std::vector<GTModel *>::iterator it = modelList.begin(); it != modelList.end(); ++it) {
    if (it == ignore) continue;
    GTModel *object = (*it);
    tmpMatch.value = object->intersect(eye, ray, &(tmpMatch.point), false);
    tmpMatch.itor = it;
    if (tmpMatch.value < GTCalc::precision) continue;
    if (!setMatch || tmpMatch.value < minMatch.value) {
      setMatch = true;
      minMatch = tmpMatch;
    }
  }

  *result = minMatch;

  if (minMatch.itor == modelList.end()) return false;
  else return true;
}

bool GTScene::intersectFastScene(vec3 eye, vec3 ray, Match *result, GTModel *ignore) {
  // build actual intersect object list
  std::vector<GTModel *> actualModelList;
  for(std::vector<GTModel *>::iterator it = modelList.begin(); it != modelList.end(); ++it) {
    if(!((*it)->isVirtual)) actualModelList.push_back((*it));
    else (*it)->getIntersectingObject(eye, ray, &(actualModelList));
  }

  Match tmpMatch, minMatch;
  minMatch.itor = actualModelList.end();  // set exit boundary
  bool setMatch = false;
  for (std::vector<GTModel *>::iterator it = actualModelList.begin(); it != actualModelList.end(); ++it) {
    GTModel *object = (*it);
    if (object == ignore) continue;
    tmpMatch.value = object->intersect(eye, ray, &(tmpMatch.point), false);
    tmpMatch.intersectedObject = object;
    tmpMatch.itor = it;
    if (tmpMatch.value < GTCalc::precision) continue;
    if (!setMatch || tmpMatch.value < minMatch.value) {
      setMatch = true;
      minMatch = tmpMatch;
    }
  }

  *result = minMatch;

  if (minMatch.itor == actualModelList.end()) return false;
  else return true;
}

void GTScene::buildModelFromFile(FILE *fp, bool refract, vec3 translation, float scale) {
  char flag;
  int vertex_count, mesh_count, ret;
  ret = fscanf(fp, "%c%d%d\n", &flag, &vertex_count, &mesh_count);
//  std::cout << vertex_count << " " << mesh_count << std::endl;

  vec3 *vertexList = new vec3[vertex_count];
  for (int i = 0; i < vertex_count; i++) {
    ret = fscanf(fp, "%c%f%f%f\n", &flag, &(vertexList[i].x), &(vertexList[i].y), &(vertexList[i].z));
//    GTCalc::printVector(vertexList[i]);
    vertexList[i] *= scale;
    vertexList[i] += translation;
//    GTCalc::printVector(vertexList[i]);
  }
  int v[3];
  GTTriangle *meshList = new GTTriangle[mesh_count];
  //TODO: recycle list;
  std::list<GTModel *> *objectGroup = new std::list<GTModel *>;
  for (int i = 0; i < mesh_count; i++) {
    ret = fscanf(fp, "%c%d%d%d\n", &flag, &(v[0]), &(v[1]), &(v[2]));
    meshList[i].vertex[0] = vertexList[v[0] - 1];
    meshList[i].vertex[1] = vertexList[v[1] - 1];
    meshList[i].vertex[2] = vertexList[v[2] - 1];
    meshList[i].vector[0] = meshList[i].vertex[1] - meshList[i].vertex[0];
    meshList[i].vector[1] = meshList[i].vertex[2] - meshList[i].vertex[0];
    meshList[i].normal = glm::normalize(glm::cross(meshList[i].vector[0], meshList[i].vector[1]));
    meshList[i].ambient = vec3(0.7, 0.7, 0.7);
    meshList[i].diffuse = vec3(0.1, 0.5, 0.8);
    meshList[i].specular = vec3(1.0, 1.0, 1.0);
    meshList[i].shineness = 30;
    meshList[i].reflectance = 0.3;
    if (refract) {
      meshList[i].isRefractObject = true;
      meshList[i].transmissivity = 0.5;
      meshList[i].refractiveIndex = 1.5;
    }
    meshList[i].setReference(objectGroup);
  }
  for (int i = 0; i < mesh_count; i++) {
    GTModel *model = &(meshList[i]);
    addModel(model);
    objectGroup->push_back(model);
  }
  if (!ret) std::cout << "release ret\n";
  delete[] vertexList;
  //TODO: recycle meshList
  meshListCycle = meshList;
}

void GTScene::buildFastModelFromFile(FILE *fp, bool refract, vec3 translation, float scale) {
  char flag;
  int vertex_count, mesh_count, ret;
  ret = fscanf(fp, "%c%d%d\n", &flag, &vertex_count, &mesh_count);

  vec3 *vertexList = new vec3[vertex_count];
  for (int i = 0; i < vertex_count; i++) {
    ret = fscanf(fp, "%c%f%f%f\n", &flag, &(vertexList[i].x), &(vertexList[i].y), &(vertexList[i].z));
    vertexList[i] *= scale;
    vertexList[i] += translation;
  }
  int v[3];
  GTTriangle *meshList = new GTTriangle[mesh_count];
  std::list<GTModel *> *objectGroup = new std::list<GTModel *>;
  float minX, maxX, minY, maxY, minZ, maxZ;
  bool six, sax, siy, say, siz, saz;
  six = sax = siy = say = siz = saz = false;
  for (int i = 0; i < mesh_count; i++) {
    ret = fscanf(fp, "%c%d%d%d\n", &flag, &(v[0]), &(v[1]), &(v[2]));
    meshList[i].vertex[0] = vertexList[v[0] - 1];
    meshList[i].vertex[1] = vertexList[v[1] - 1];
    meshList[i].vertex[2] = vertexList[v[2] - 1];
    for(int j = 0; j < 3; j++) {
      if(!six || meshList[i].vertex[j].x < minX) {six = true;  minX = meshList[i].vertex[j].x;}
      if(!sax || meshList[i].vertex[j].x > maxX) {sax = true;  maxX = meshList[i].vertex[j].x;}
      if(!siy || meshList[i].vertex[j].y < minY) {siy = true;  minY = meshList[i].vertex[j].y;}
      if(!say || meshList[i].vertex[j].y > maxY) {say = true;  maxY = meshList[i].vertex[j].y;}
      if(!siz || meshList[i].vertex[j].z < minZ) {siz = true;  minZ = meshList[i].vertex[j].z;}
      if(!saz || meshList[i].vertex[j].z > maxZ) {saz = true;  maxZ = meshList[i].vertex[j].z;}
    }
    meshList[i].vector[0] = meshList[i].vertex[1] - meshList[i].vertex[0];
    meshList[i].vector[1] = meshList[i].vertex[2] - meshList[i].vertex[0];
    meshList[i].normal = glm::normalize(glm::cross(meshList[i].vector[0], meshList[i].vector[1]));
    meshList[i].ambient = vec3(0.7, 0.7, 0.7);
    meshList[i].diffuse = vec3(0.1, 0.5, 0.8);
    meshList[i].specular = vec3(1.0, 1.0, 1.0);
    meshList[i].shineness = 30;
    meshList[i].reflectance = 0.3;
    if (refract) {
      meshList[i].isRefractObject = true;
      meshList[i].transmissivity = 0.5;
      meshList[i].refractiveIndex = 1.5;
    }
    meshList[i].setReference(objectGroup);
  }
//  for (int i = 0; i < mesh_count; i++) {
//    GTModel *model = &(meshList[i]);
//    addModel(model);
//    objectGroup->push_back(model);
//  }
  GTBoundary *bound = buildBoundaryRange(minX, maxX, minY, maxY, minZ, maxZ);
  for (int i = 0; i < mesh_count; i++) {
    bound->root.addObject(&(meshList[i]));
  }
  bound->root.splitSpace(OCTTREE_SPLIT_STEP);

  GTModel *model = bound;
  addModel(model);

  if (!ret) std::cout << "release ret\n";
  delete[] vertexList;
  meshListCycle = meshList;
}

GTBoundary *GTScene::buildBoundaryRange(float minx, float maxx, float miny, float maxy, float minz, float maxz) {
  float xl = maxx - minx;
  float yl = maxy - miny;
  float zl = maxz - minz;
  float cx = (minx + maxx) / 2;
  float cy = (miny + maxy) / 2;
  float cz = (minz + maxz) / 2;
  vec3 xaxis(1, 0, 0), yaxis(0, 1, 0), zaxis(0, 0, 1);
  vec3 diffuse(0.7, 0.7, 0.7);

//  GTPlane *box = new GTPlane[6];
  GTBoundary *b = new GTBoundary;
  b->box[0].position = vec3(cx, cy, maxz);
  b->box[0].normal = zaxis;
  b->box[1].position = vec3(cx, cy, minz);
  b->box[1].normal = -zaxis;
  b->box[2].position = vec3(maxx, cy, cz);
  b->box[2].normal = xaxis;
  b->box[3].position = vec3(minx, cy, cz);
  b->box[3].normal = -xaxis;
  b->box[4].position = vec3(cx, maxy, cz);
  b->box[4].normal = yaxis;
  b->box[5].position = vec3(cx, miny, cz);
  b->box[5].normal = -yaxis;

  for(int i = 0; i < 6; i++) {
    b->box[i].xLength = xl;
    b->box[i].yLength = yl;
    b->box[i].zLength = zl;
    b->box[i].diffuse = diffuse;
    b->box[i].specular = vec3(1.0, 1.0, 1.0);
    b->box[i].shineness = 40;
    b->box[i].reflectance = 1.0;
    b->box[i].isRefractObject = false;
  }
  b->root.setRange(minx, maxx, miny, maxy, minz, maxz);

//  for(int i = 2; i < 4; i++)
//    addModel(&(b->box[i]));

  return b;
}

