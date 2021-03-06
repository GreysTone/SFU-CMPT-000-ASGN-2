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
// multi-core header
#include <omp.h>

#include "global.h"
#include "include/shader.h"

#include "gtTracer.hpp"



// OpenGL
const int NumPoints = 6;

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
    std::cout << "./raycast [-u | -d | bonus | fbonus] step_max <options>\n";
    return false;
  }

  tracer = new GTTracer;
  int options = NOUN;

  // user defined scene
  if (strcmp(argv[1], "-u") == 0) options |= USER_SCENE;
  if (strcmp(argv[1], "bonus") == 0) {
    options = USER_SCENE | SHADOW | REFLECTION | CHESSBOARD | REFRACTION | STOCHASTIC_DIFFUSE | ANTIALIAS;
    options |= BONUS;
  }
  if (strcmp(argv[1], "fbonus") == 0) {
    options = USER_SCENE | SHADOW | REFLECTION | CHESSBOARD | REFRACTION | STOCHASTIC_DIFFUSE | ANTIALIAS;
    options |= FAST_BONUS;
  }

  // maximum level of recursions
  tracer->setMaxStep(atoi(argv[2]));

  // optional arguments
  for (int i = 3; i < argc; i++) {
    if (strcmp(argv[i], "+s") == 0) options |= SHADOW;
    if (strcmp(argv[i], "+l") == 0) options |= REFLECTION;
    if (strcmp(argv[i], "+c") == 0) options |= CHESSBOARD;
    if (strcmp(argv[i], "+r") == 0) options |= REFRACTION;
    if (strcmp(argv[i], "+f") == 0) options |= STOCHASTIC_DIFFUSE;
    if (strcmp(argv[i], "+p") == 0) options |= ANTIALIAS;
  }

//  std::cout << options << std::endl;

  tracer->setConfiguration((GTTracerSetting)options);
  tracer->buildScene();

  // ray trace the scene
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
