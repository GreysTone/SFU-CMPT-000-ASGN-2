/***********************************************************
 * CMPT 361, Spring 2016
 *
 *  raycast.cpp
 *      
 *  Render a simple scene using ray tracing
 * 
 *  NAME: Danyang Song (Arthur)
 *  SFU ID: 301295765
 *
 *  Template code for drawing a scene using raycasting.
 *  Some portions of the code was originally written by 
 *  M. vandePanne - and then modified by R. Zhang & H. Li
***********************************************************/

//#include "include/Angel.h"
#ifdef __APPLE__  // include Mac OS X version of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
//#  include <GL/glut.h>
#  include <GL/freeglut.h>
#endif  // __APPLE__


#include <iostream>
#include <string.h>

#include "global.h"
#include "include/shader.h"

#include "gtTracer.hpp"

//#include "trace.h"
//#include "sphere.h"
//#include "image_util.h"
//#include "scene.h"

//
// Global variables
//
// Here we avoid dynamic memory allocation as a convenience. You can
// change the resolution of your rendered image by changing the values
// of WIN_X_SIZE and WIN_Y_SIZE in "global.h", along with other
// global variables
//
//
//int win_width = WIN_WIDTH;
//int win_height = WIN_HEIGHT;

//GLfloat frame[WIN_HEIGHT][WIN_WIDTH][3];
// array for the final image
// This gets displayed in glut window via texture mapping,
// you can also save a copy as bitmap by pressing 's'
//
//float image_width = IMAGE_WIDTH;
//float image_height = (float(WIN_HEIGHT) / float(WIN_WIDTH)) * IMAGE_WIDTH;

// some colors
//RGB_float background_clr; // background color
//RGB_float null_clr = {0.0, 0.0, 0.0};   // NULL color

//
// these view parameters should be fixed
//
//Point eye_pos = {0.0, 0.0, 0.0};  // eye position
//float image_plane = -1.5;           // image plane position

// list of spheres in the scene
//Spheres *scene = NULL;

// light 1 position and color
//Point light1;
//float light1_intensity[3];

// global ambient term
//float global_ambient[3];

// light decay parameters
//float decay_a;
//float decay_b;
//float decay_c;

// maximum level of recursions; you can use to control whether reflection
// is implemented and for how many levels
//int step_max = 1;

// You can put your flags here
// a flag to indicate whether you want to have shadows
//int shadow_on = 0;


// OpenGL
const int NumPoints = 6;

//gtArgument *ArgumentHandler = NULL;
GTTracer *tracer = NULL;
//----------------------------------------------------------------------------

void init() {
  // Vertices of a square
//  double ext = 1.0;
//  vec4 points[NumPoints] = {
//      vec4(-ext, -ext, 0, 1.0), //v1
//      vec4(ext, -ext, 0, 1.0), //v2
//      vec4(-ext, ext, 0, 1.0), //v3
//      vec4(-ext, ext, 0, 1.0), //v3
//      vec4(ext, -ext, 0, 1.0), //v2
//      vec4(ext, ext, 0, 1.0)  //v4
//  };
//
//  // Texture coordinates
//  vec2 tex_coords[NumPoints] = {
//      vec2(0.0, 0.0),
//      vec2(1.0, 0.0),
//      vec2(0.0, 1.0),
//      vec2(0.0, 1.0),
//      vec2(1.0, 0.0),
//      vec2(1.0, 1.0)
//  };

  float ext = 1.0;
  float points[NumPoints * 4] = {
      -ext, -ext, 0, 1.0, //v1
      ext, -ext, 0, 1.0, //v2
      -ext, ext, 0, 1.0, //v3
      -ext, ext, 0, 1.0, //v3
      ext, -ext, 0, 1.0, //v2
      ext, ext, 0, 1.0  //v4
  };

  // Texture coordinates
  float tex_coords[NumPoints * 2] = {
      0.0, 0.0,
      1.0, 0.0,
      0.0, 1.0,
      0.0, 1.0,
      1.0, 0.0,
      1.0, 1.0
  };

  // Initialize texture objects
  GLuint texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIN_WIDTH, WIN_HEIGHT, 0,
               GL_RGB, GL_FLOAT, tracer->frame);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glActiveTexture(GL_TEXTURE0);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
  GLintptr offset = 0;
  glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(points), points);
  offset += sizeof(points);
  glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(tex_coords), tex_coords);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader.glsl", "fshader.glsl");
  glUseProgram(program);

  // set up vertex arrays
  offset = 0;
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(offset));

  offset += sizeof(points);
  GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
  glEnableVertexAttribArray(vTexCoord);
  glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(offset));

  glUniform1i(glGetUniformLocation(program, "texture"), 0);

  glClearColor(1.0, 1.0, 1.0, 1.0);
}

/*********************************************************
 * This is the OpenGL display function. It is called by
 * the event handler to draw the scene after you have
 * rendered the image using ray tracing. Remember that
 * the pointer to the image memory is stored in 'frame'.
 *
 * There is no need to change this.
 **********************************************************/

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);

  glDrawArrays(GL_TRIANGLES, 0, NumPoints);

  glutSwapBuffers();
}

/*********************************************************
 * This function handles keypresses
 *
 *   s - save image
 *   q - quit
 *
 * DO NOT CHANGE
 *********************************************************/
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'q':
    case 'Q':
//      free(scene);
      exit(0);
    case 's':
    case 'S':
      tracer->saveImage();
      glutPostRedisplay();
      break;
    default:
      break;
  }
}



//----------------------------------------------------------------------------

int main(int argc, char **argv) {

  if (argc < 3) {
    std::cout << "Missing arguments ... use:\n";
    std::cout << "./raycast [-u | -d] step_max <options>\n";
    return false;
  }

  tracer = new GTTracer;
  int options = NOUN;

  // user defined scene
  if (strcmp(argv[1], "-u") == 0) options |= USER_SCENE;
//    set_up_user_scene();
//  else
//    set_up_default_scene();
  //TODO: enforce using default scene
//  set_up_default_scene();

  // maximum level of recursions
  tracer->setMaxStep(atoi(argv[2]));

  // Optional arguments
  for (int i = 3; i < argc; i++) {
    if (strcmp(argv[i], "-s") == 0) options |= SHADOW;
  }

  tracer->setConfiguration((GTTracerSetting)options);
  tracer->buildScene();

  //
  // ray trace the scene now
  //
  // we have used so many global variables and this function is
  // happy to carry no parameters
  //
  printf("Rendering scene using my fantastic ray tracer ...\n");
//  ray_trace();
  tracer->traceRay();

  // we want to make sure that intensity values are normalized
  tracer->normalizeHistogram();

  // Show the result in glut via texture mapping
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutCreateWindow("Ray tracing");
  glewInit();
  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
