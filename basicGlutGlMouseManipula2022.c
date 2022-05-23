#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "GL/glut.h" 

#define MAXVERTEXS 30  // numero maximo de v�rtices do poligono (espa�o disponivel)
#define PHI 3.141572
#define ZERO 0.00001;

GLenum doubleBuffer;

struct polygon
{
  float v[3];
} pvertex[MAXVERTEXS];

int windW, windH;    // comprimento da janela: Width  e Height
int gOpera = 0;      // tipo de operacao: O: gera poligono; 1: transalacao; 2:...
int tipoPoligono;    // poligo por vertices ou por arestas
int gIndVert = -1;   // indice do v�rtice selecionado
int gNumVert = 0;    // numero de v�rtices do poligono definido
float gCen[3];       // centroido do poligono
float gAng = 0.0f;    // angulo para rotacao 

void circulo(float r, float ang, float pp[3])
{
  pp[0] = (float)(r * cos(ang));
  pp[1] = (float)(r * sin(ang));
  pp[2] = (float)0.0;
}


int clipVertex(int x, int y)
{
  int i;
  float d;
  gIndVert = -1;
  printf("x=%d y=%d\n", x, y);

  for (i = 0; i < gNumVert; i++) {
    d = sqrt(pow((pvertex[i].v[0] - x), 2.0) + pow((pvertex[i].v[1] - y), 2.0));

    if (i == 0) {
      printf("p=(%f,%f) d=%f\n", pvertex[i].v[0], pvertex[i].v[1], d);
    }


    if (d < 3.0) {
      gIndVert = i;
      break;
    }
  }
  return gIndVert;
}

void init(void)
{
  int i;
  float ang;
  tipoPoligono = GL_LINE;

  for (i = 0; i < gNumVert; i++)
  {
    ang = (float)(i) * (2.0 * PHI) / ((float)(gNumVert));
    circulo(70.0, ang, pvertex[i].v);
  }

  // calcula o angulo b�sico de rotacao 
  gAng = (2.0f * PHI) / 180.0f;
}

static void Reshape(int width, int height)
{
  windW = width / 2;
  windH = height / 2;

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //    gluPerspective(60.0, 1.0, 0.1, 1000.0);
  gluOrtho2D(-windW, windW, -windH, windH);

  glMatrixMode(GL_MODELVIEW);
}

static void Key(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27:
    exit(0);
  }
}

void coord_line(void)
{
  glLineWidth(1);

  glColor3f(1.0, 0.0, 0.0);

  // vertical line

  glBegin(GL_LINE_STRIP);
  glVertex2f(-windW, 0);
  glVertex2f(windW, 0);
  glEnd();

  // horizontal line 

  glBegin(GL_LINE_STRIP);
  glVertex2f(0, -windH);
  glVertex2f(0, windH);
  glEnd();
}

void PolygonDraw(void)
{
  int i;

  glColor3f(0.0, 0.0, 0.0);

  glPolygonMode(GL_FRONT_AND_BACK, tipoPoligono);

  glBegin(GL_POLYGON);
  for (i = 0; i < gNumVert; i++)
  {
    glVertex2fv(pvertex[i].v);
  }
  glEnd();
}

static void Draw(void)
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  coord_line();

  PolygonDraw();

  if (gIndVert > -1) {
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex2fv(pvertex[gIndVert].v);
    glEnd();
  }

  if (doubleBuffer)
  {
    glutSwapBuffers();
  }
  else {
    glFlush();
  }
}

static void Args(int argc, char** argv)
{
  GLint i;

  doubleBuffer = GL_FALSE;

  for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-sb") == 0)
    {
      doubleBuffer = GL_FALSE;
    }
    else if (strcmp(argv[i], "-db") == 0)
    {
      doubleBuffer = GL_TRUE;
    }
  }
}


void procegVertMenuEvents(int option)
{
  switch (option)
  {
  case 1:
    if (tipoPoligono == GL_LINE)
      tipoPoligono = GL_FILL;
    else
      tipoPoligono = GL_LINE;
    break;
  }
  glutPostRedisplay();
}

void subMenu1Events(int option)
{
  // option:  1 - poligono;  2 - triangulo
  switch (option) {
  case 0:  // livre
    printf("\n gerar vertices com mouse \n");
    gNumVert = 0;
    break;
  case 1:  // pent�gono
    gNumVert = 3;
    break;
  case 2:  // triangulo
    gNumVert = 5;
    break;
  }
  init();
  glutPostRedisplay();
}

void subMenu2Events(int option)
{
  //  option: 
  //            1: transla��o
  //            2: Rota��o
  //            3: Scalamento
  //            4: Cisalha
  gOpera = option;
  glutPostRedisplay();
}

void createGLUTMenus()
{
  int menu, submenu1, submenu2;

  submenu1 = glutCreateMenu(subMenu1Events);
  glutAddMenuEntry("Free (mouse)", 0);
  glutAddMenuEntry("Triangle", 1);
  glutAddMenuEntry("Pentagon", 2);

  submenu2 = glutCreateMenu(subMenu2Events);
  glutAddMenuEntry("Translation", 1);
  glutAddMenuEntry("Rotation", 2);
  glutAddMenuEntry("Scale", 3);
  glutAddMenuEntry("Shear", 4);

  menu = glutCreateMenu(procegVertMenuEvents);
  glutAddMenuEntry("Wire/Solid", 1);
  glutAddSubMenu("Polygon Type", submenu1);
  glutAddSubMenu("Transformation", submenu2);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void translate(float dx, float dy)
{
  int i;
  for (i = 0; i < gNumVert; i++) {
    pvertex[i].v[0] += dx;
    pvertex[i].v[1] += dy;
  }
}

void calCentro(void)
{
  int i;
  // computando o centroide
  gCen[0] = gCen[1] = 0.0f;
  for (i = 0; i < gNumVert; i++) {
    gCen[0] += pvertex[i].v[0];
    gCen[1] += pvertex[i].v[1];
  }
  gCen[0] /= gNumVert;
  gCen[1] /= gNumVert;
}

void translaCentro(int t)
{
  int i;
  // translada para centro
  for (i = 0; i < gNumVert; i++) {
    pvertex[i].v[0] += (t * gCen[0]);
    pvertex[i].v[1] += (t * gCen[1]);
  }
}

void rotate(float dx, float dy)
{
  int i;
  float oo, teta, xy[3];

  // calculo do angulo 
  // seja vetor do centro para o vertice: vv 
  // dd = (dx, dy) � o vetor deslocalmento do mouse
  // o = vv x dd  (produto vetorial)
  // se o positivo ==> rota antihorario;  
  // se o negativo ==> rota horario

  calCentro();
  translaCentro(-1);

  // determinando o angulo
  oo = pvertex[gIndVert].v[1] * dx - pvertex[gIndVert].v[0] * dy;

  teta = gAng;
  if (oo > 0.0f)
    teta = -1.0f * gAng;

  // rota em teta para lado oo         
  for (i = 0; i < gNumVert; i++) {
    xy[0] = pvertex[i].v[0];
    xy[1] = pvertex[i].v[1];
    pvertex[i].v[0] = xy[0] * cos(teta) - xy[1] * sin(teta);
    pvertex[i].v[1] = xy[0] * sin(teta) + xy[1] * cos(teta);
  }
  translaCentro(1);
}

void scale(float dx, float dy)
{
  int i;
  float sx, sy;

  calCentro();
  translaCentro(-1);
  // scalando...

  sx = sy = 1.0f;
  if (fabs(pvertex[gIndVert].v[0]) > 0.01f)
    sx = 1.0f + dx / pvertex[gIndVert].v[0];
  if (fabs(pvertex[gIndVert].v[1]) > 0.01f)
    sy = 1.0f + dy / pvertex[gIndVert].v[1];
  for (i = 0; i < gNumVert; i++) {
    pvertex[i].v[0] *= sx;
    pvertex[i].v[1] *= sy;
  }

  translaCentro(1);
}

void shear(float dx, float dy)
{
  int i;
  float sx, sy, xy[3];

  sx = 0.001f * dx;
  sy = 0.001f * dy;
  if (dx > dy)
  {
    if (fabs(pvertex[gIndVert].v[0]) > 0.1f)
      sx = dx / pvertex[gIndVert].v[0];
  }
  else
  {
    if (fabs(pvertex[gIndVert].v[1]) > 0.1f)
      sy = dy / pvertex[gIndVert].v[1];
  }

  calCentro();
  translaCentro(-1);

  // rota em teta para lado oo         
  for (i = 0; i < gNumVert; i++) {
    xy[0] = pvertex[i].v[0];
    xy[1] = pvertex[i].v[1];
    pvertex[i].v[0] = xy[0] + xy[1] * sx;
    pvertex[i].v[1] = xy[0] * sy + xy[1];
  }

  translaCentro(1);
}

void motion(int x, int y)
{
  int i;
  float dx, dy;
  if (gIndVert > -1) {
    x = x - windW; y = windH - y;
    dx = x - pvertex[gIndVert].v[0];
    dy = y - pvertex[gIndVert].v[1];
    switch (gOpera)
    {
    case 1: translate(dx, dy); break;
    case 2: rotate(dx, dy); break;
    case 3: scale(dx, dy); break;
    case 4: shear(dx, dy); break;
    }
    Draw();
  }
}

void mouse(int button, int state, int x, int y)
{
  gIndVert = -1;

  if (button == GLUT_LEFT)
  {
    if (state == GLUT_DOWN)
    {
      x = x - windW; y = windH - y;
      gIndVert = clipVertex(x, y);
    }
  }
}

int main(int argc, char** argv)
{
  GLenum type;

  glutInit(&argc, argv);
  Args(argc, argv);

  type = GLUT_RGB;
  type |= (doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE;

  glutInitDisplayMode(type);
  glutInitWindowSize(600, 500);
  glutCreateWindow("Basic Program Using Glut and Gl");

  init();

  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutDisplayFunc(Draw);

  glutMotionFunc(motion);
  glutMouseFunc(mouse);

  createGLUTMenus();

  glutMainLoop();

  return (0);
}
