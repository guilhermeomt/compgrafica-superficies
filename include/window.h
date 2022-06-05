#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "GL/glut.h"
#include <string.h>
#include <math.h>

#include "matrix.h"
#include "transformation.h"

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

#include <GL/glut.h>

#define Linha -1
#define Solido -2 
#define Pontos -3

#define X 0
#define Y 1
#define Z 2
#define W 3

#define Escalar 4 // Redimensionar

#define RotarX 5
#define RotarY 6
#define RotarZ 7

#define TransladaX 8
#define TransladaY 9
#define TransladaZ 10

#define PtsControle 19
#define BEZIER      20
#define BSPLINE     21
#define CATMULLROM  22

#define VARIA 0.1f

#define sair 0

typedef float* f4d;

class Window {
private:
  int _height;
  int _width;
  const char* _title;
  int _argc;
  char** _argv;
  GLenum _doubleBuffer;
  int _gOperation;
  int _idColor;
  Matrix<f4d>** _pControl = nullptr;
  Matrix<f4d>** _pcPatch = nullptr;
  Matrix<f4d>** _ptsPatch = nullptr;
  f4d _matBase[4];

  int _viewType = GL_LINE_STRIP;

  void initGLUT();
  void args();
  void init();
  void createGLUTMenus();
  void drawSurface();
  void freeMatrix(Matrix<f4d>** matrix);
  void copyControlPointsPatch(int i0, int j0);
  void getSurfacePoints();
  void productVectorParamMatrixBase(float t, float* tt, float* vr);
  void productVectorMatrix(float* v, f4d* vr);
  int loadPoints(char* fileName);
  void setBaseMatrix(int supType);
  void showControlPoints();
  void clipVertex(int x, int y);

public:
  Window() {};
  Window(const char* title, int argc, char** argv);
  ~Window();
  void display();
  void reshape(int width, int height);
  void keyboard(unsigned char key, int x, int y);
  void motion(int x, int y);
  void mouse(int button, int state, int x, int y);
  void handleMenuEvents(int option);
  void handleColorEvents(int option);
  void render();

};

#endif // __WINDOW_H__