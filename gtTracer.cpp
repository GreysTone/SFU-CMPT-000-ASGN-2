//
//  gtTracer.cpp
//  RayCast
//
//  Created by GreysTone on 2016-03-27.
//  Copyright © 2016 Danyang Song (Arthur). All rights reserved.
//

#include "gtTracer.hpp"

GTTracer::GTTracer() {
  userSceneActived = false;
  shadowActived = false;

  maxStep = 1;
  scene = new GTScene;

  win_width = WIN_WIDTH;
  win_height = WIN_HEIGHT;

  image_width = IMAGE_WIDTH;
  image_height = (float(WIN_HEIGHT) / float(WIN_WIDTH)) * IMAGE_WIDTH;
}

void GTTracer::setMaxStep(int arg) {
  maxStep = arg;
}

void GTTracer::setConfiguration(GTTracerSetting arg) {
  if (arg & 1) userSceneActived = true;
  else userSceneActived = false;

  if (arg & 2) shadowActived = true;
  else shadowActived = false;
}

void GTTracer::buildScene() {
  if (userSceneActived) scene->buildUserScene();
  else scene->buildDefaultScene();
}

inline float max(float a, float b) { return a>b?a:b; }

/*********************************************************************
* This function traverses all the pixels and cast rays. It calls the
* recursive ray tracer and assign return color to frame
*
* You should not need to change it except for the call to the recursive
* ray tracer. Feel free to change other parts of the function however,
* if you must.
*********************************************************************/
void GTTracer::traceRay() {
  int i, j;
  float x_grid_size = image_width / float(win_width);
  float y_grid_size = image_height / float(win_height);
  float x_start = -0.5 * image_width;
  float y_start = -0.5 * image_height;
  vec3 ret_color;
  vec3 cur_pixel_pos;
  vec3 ray;

  // ray is cast through center of pixel
  cur_pixel_pos.x = x_start + 0.5 * x_grid_size;
  cur_pixel_pos.y = y_start + 0.5 * y_grid_size;
  cur_pixel_pos.z = scene->image_plane;


  for (i = 0; i < win_height; i++) {
    for (j = 0; j < win_width; j++) {
      ray = cur_pixel_pos - scene->eye_pos;

      scene->intersectScene(scene->eye_pos, ray, i, j);

      if (scene->depthValue[i][j] > GTCalc::precision) {
        std::list<GTLight>::iterator it = scene->lightList.begin();
        GTLight &light = *(it);
        vec3 pointSurf = scene->depthPoint[i][j];
        std::list<GTSphere>::iterator obj = scene->depthObject[i][j];

        vec3 normLight = glm::normalize(light.position - pointSurf);
        float distance = glm::length(light.position - pointSurf);
        vec3 &decay = scene->decay;
        float decayCoefficient = 1 / (decay.x + decay.y * distance + decay.z * distance * distance);

        vec3 normSurf = glm::normalize(obj->normal(pointSurf));
        vec3 vecReflect = (2 * glm::dot(normLight, normSurf) * normSurf) - normLight;
        vec3 normReflect = glm::normalize(vecReflect);
        vec3 vecProject = scene->eye_pos - pointSurf;
        vec3 normProject = glm::normalize(vecProject);

        // calculate color
        vec3 Ambient = obj->ambient * scene->global_ambient;
        vec3 Diffuse = decayCoefficient  * obj->diffuse * max(glm::dot(normLight, normSurf), 0.0f);
        vec3 Specular = decayCoefficient * obj->specular *
            (float)pow(max(glm::dot(normReflect, normProject), 0.0f), obj->shineness);
        ret_color = Ambient + Diffuse + Specular;

      } else {
        ret_color = scene->background_color;
      }


      ////
      // You need to change this!!!

      // ret_color = recursive_ray_trace();
//        ret_color = background_clr; // just background for now
//      ret_color = scene->background_color;

      // Parallel rays can be cast instead using below
      //
      // ray.x = ray.y = 0;
      // ray.z = -1.0;
      // ret_color = recursive_ray_trace(cur_pixel_pos, ray, 1);

// Checkboard for testing
//        RGB_float clr = {float(i/32), 0, float(j/32)};
#ifdef GT_TEST
      vec3 clr = vec3(float(i / 32), 0, float(j / 32));
      ret_color = clr;
#endif

      frame[i][j][0] = GLfloat(ret_color.r);
      frame[i][j][1] = GLfloat(ret_color.g);
      frame[i][j][2] = GLfloat(ret_color.b);

      cur_pixel_pos.x += x_grid_size;
    }

    cur_pixel_pos.y += y_grid_size;
    cur_pixel_pos.x = x_start;
  }
}

/*********************************************************
 * This function saves the current image to a ppm file
 *
 * DO NOT CHANGE
 *********************************************************/
void GTTracer::saveImage() {
  int w = win_width;
  int h = win_height;

  unsigned char *bImg = new unsigned char[w * h * 3];

  int index = 0;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {

      float r = frame[y][x][0];
      float g = frame[y][x][1];
      float b = frame[y][x][2];

      bImg[index] = (b > 1.f) ? 255 : (unsigned char) (b * 255);
      index++;
      bImg[index] = (g > 1.f) ? 255 : (unsigned char) (g * 255);
      index++;
      bImg[index] = (r > 1.f) ? 255 : (unsigned char) (r * 255);
      index++;
    }
  }

  unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
  unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
  unsigned char bmppad[3] = {0, 0, 0};

  int filesize = 54 + 3 * w * h;

  bmpfileheader[2] = (unsigned char) (filesize);
  bmpfileheader[3] = (unsigned char) (filesize >> 8);
  bmpfileheader[4] = (unsigned char) (filesize >> 16);
  bmpfileheader[5] = (unsigned char) (filesize >> 24);

  bmpinfoheader[4] = (unsigned char) (w);
  bmpinfoheader[5] = (unsigned char) (w >> 8);
  bmpinfoheader[6] = (unsigned char) (w >> 16);
  bmpinfoheader[7] = (unsigned char) (w >> 24);
  bmpinfoheader[8] = (unsigned char) (h);
  bmpinfoheader[9] = (unsigned char) (h >> 8);
  bmpinfoheader[10] = (unsigned char) (h >> 16);
  bmpinfoheader[11] = (unsigned char) (h >> 24);


  FILE *fp;
  char fname[32];

  strcpy(fname, "scene.bmp");
  printf("Saving image %s: %d x %d\n", fname, w, h);
  fp = fopen(fname, "wb");
  if (!fp) {
    printf("Unable to open file '%s'\n", fname);
    return;
  }


  fwrite(bmpfileheader, 1, 14, fp);
  fwrite(bmpinfoheader, 1, 40, fp);

  for (int y = h - 1; y >= 0; y--) {
    int offset = w * (h - 1 - y) * 3;
    fwrite(bImg + offset, 3, w, fp);
    fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, fp);
  }


  fclose(fp);
}

/**************************************************************
 * This function normalizes the frame resulting from ray
 * tracing so that the maximum R, G, or B value is 1.0
 *
 * DO NOT CHANGE
 **************************************************************/
void GTTracer::normalizeHistogram() {
  GLfloat max_val = 0.0;
  int i, j;

  for (i = 0; i < win_height; i++)
    for (j = 0; j < win_width; j++) {
      if (frame[i][j][0] > max_val) max_val = frame[i][j][0];
      if (frame[i][j][1] > max_val) max_val = frame[i][j][1];
      if (frame[i][j][2] > max_val) max_val = frame[i][j][2];
    }

  for (i = 0; i < win_height; i++)
    for (j = 0; j < win_width; j++) {
      frame[i][j][0] /= max_val;
      frame[i][j][1] /= max_val;
      frame[i][j][2] /= max_val;
    }
}