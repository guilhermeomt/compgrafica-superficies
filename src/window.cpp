#include "../include/window.h"
#include "../include/window_callback.h"

Window::Window(const char* title, int argc, char** argv) : _title(title), _argc(argc), _argv(argv)
{
  _width = WINDOW_WIDTH;
  _height = WINDOW_HEIGHT;
}

Window::~Window()
{

}

void Window::initGLUT()
{
  GLenum type;

  glutInit(&_argc, _argv);
  args();


  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(_width, _height);
  glutCreateWindow(_title);

}

void Window::args()
{
  GLint i;

  _doubleBuffer = GL_FALSE;

  for (i = 1; i < _argc; i++)
  {
    if (strcmp(_argv[i], "-sb") == 0)
    {
      _doubleBuffer = GL_FALSE;
    }
    else if (strcmp(_argv[i], "-db") == 0)
    {
      _doubleBuffer = GL_TRUE;
    }
  }
}

void Window::init()
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MAP2_VERTEX_3);
  glEnable(GL_AUTO_NORMAL);
  glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
}

void Window::display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  if (_pControl)
  {
    showControlPoints();
    drawSurface();
  }

  glutSwapBuffers();
}

void Window::freeMatrix(Matrix<f4d>** matrix)
{
  if (*matrix)
  {
    for (int i = 0; i < matrix[0]->rows(); i++)
    {
      auto px = matrix[X]->get(i, 0);
      auto py = matrix[Y]->get(i, 0);
      auto pz = matrix[Z]->get(i, 0);
      auto p = matrix[3]->get(i, 0);
      delete[] px;
      delete[] py;
      delete[] pz;
      delete[] p;
    }

  }

  delete matrix[0];
  delete matrix[1];
  delete matrix[2];
  delete matrix[3];

  delete* matrix;
  matrix = nullptr;
}

void Window::drawSurface() {
  int i, j, nn, mm;

  nn = _pControl[0]->rows() - 3;

  for (i = 0; i < nn; i++)
  {
    mm = _pControl[0]->cols() - 3;
    for (j = 0; j < mm; j++)
    {
      //copyControlPointsPatch(i, j);
      // getSurfacePoints();
      // MostrarUmPatch();
    }
  }
}

void Window::productVectorParamMatrixBase(float t, float* tt, float* vr)
{
  int i, j;

  tt[0] = pow(t, 3);
  tt[1] = pow(t, 2);
  tt[2] = t;
  tt[3] = 1.0;

  for (i = 0; i < 4; i++)
  {
    vr[i] = 0.0f;
    for (j = 0; j < 4; j++)
      vr[i] += _matBase[j][i] * tt[j];
  }
}

void Window::productVectorMatrix(float* v, f4d* vr)
{
  int i, j;

  for (i = 0; i < 4; i++)
  {
    vr[i][0] = vr[i][1] = vr[i][2] = 0.0;
    for (j = 0; j < 4; j++)
    {
      auto px = *_pcPatch[X]->get(i, j);
      printf("px: %f\n", px[0]);
      auto py = *_pcPatch[Y]->get(i, j);
      auto pz = *_pcPatch[Z]->get(i, j);
      vr[i][0] += v[j] * px[0];
      vr[i][1] += v[j] * py[0];
      vr[i][2] += v[j] * pz[0];
    }
  }
}

void Window::getSurfacePoints() {
  int i, j, h, n, m;
  float t, s;
  float tmp[4], vsm[4], vtm[4];
  f4d va[4];

  for (i = 0; i < 4; i++)
  {
    va[i] = new float[4];
  }

  if (!_pcPatch) return;

  n = 0;

  for (s = 0; s <= 1.01; s += VARIA) n += 1;

  m = n;

  _ptsPatch = new Matrix<f4d> *[4];

  for (i = 0; i < n; i++)
  {
    _ptsPatch[i] = new Matrix<f4d>(n, m);
  }

  s = 0.0f;
  for (i = 0; i < _ptsPatch[0]->rows(); i++)
  {
    t = 0.0f;
    for (j = 0; j < _ptsPatch[0]->cols(); j++)
    {
      productVectorParamMatrixBase(s, tmp, vsm);
      productVectorParamMatrixBase(t, tmp, vtm);

      productVectorMatrix(vsm, va);

      auto p1 = new float[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

      _ptsPatch[X]->set(i, j, &p1[0]);
      _ptsPatch[Y]->set(i, j, &p1[1]);
      _ptsPatch[Z]->set(i, j, &p1[2]);

      for (h = 0; h < 4; h++)
      {

        auto p2 = new float[3]{ va[h][0] * vtm[h],
                                va[h][1] * vtm[h],
                                va[h][2] * vtm[h] };

        _ptsPatch[X]->set(i, j, &p2[0]);
        _ptsPatch[Y]->set(i, j, &p2[1]);
        _ptsPatch[Z]->set(i, j, &p2[2]);
      }
      t += VARIA;
    }
    s += VARIA;
  }
}

void Window::setBaseMatrix(int supType)
{
  if (supType == BEZIER)
  {
    _matBase[0][0] = -1.0f; _matBase[0][1] = 3.0f;  _matBase[0][2] = -3.0f; _matBase[0][3] = 1.0f;
    _matBase[1][0] = 3.0f; _matBase[1][1] = -6.0f; _matBase[1][2] = 3.0f; _matBase[1][3] = 0.0f;
    _matBase[2][0] = -3.0f; _matBase[2][1] = 3.0f;  _matBase[2][2] = 0.0f; _matBase[2][3] = 0.0f;
    _matBase[3][0] = 1.0f; _matBase[3][1] = 0.0f;  _matBase[3][2] = 0.0f; _matBase[3][3] = 0.0f;
  }

  if (supType == BSPLINE)
  {
    _matBase[0][0] = -1.0f / 6.0;  _matBase[0][1] = 3.0f / 6.0;  _matBase[0][2] = -3.0f / 6.0;  _matBase[0][3] = 1.0f / 6.0;
    _matBase[1][0] = 3.0f / 6.0; _matBase[1][1] = -6.0f / 6.0;   _matBase[1][2] = 3.0f / 6.0;   _matBase[1][3] = 0.0f;
    _matBase[2][0] = -3.0f / 6.0;  _matBase[2][1] = 0.0f / 6.0;  _matBase[2][2] = 3.0f / 6.0;  _matBase[2][3] = 0.0f;
    _matBase[3][0] = 1.0f / 6.0; _matBase[3][1] = 4.0f / 6.0;  _matBase[3][2] = 1.0f / 6.0; _matBase[3][3] = 0.0;
  }

  if (supType == CATMULLROM)
  {
    _matBase[0][0] = -1.0f / 2.0; _matBase[0][1] = 3.0f / 2.0;  _matBase[0][2] = -3.0f / 2.0; _matBase[0][3] = 1.0f / 2.0;
    _matBase[1][0] = 2.0f / 2.0; _matBase[1][1] = -5.0f / 2.0; _matBase[1][2] = 4.0f / 2.0; _matBase[1][3] = -1.0f / 2.0;
    _matBase[2][0] = -1.0f / 2.0; _matBase[2][1] = 0.0f / 2.0;  _matBase[2][2] = 1.0f / 2.0; _matBase[2][3] = 0.0f / 2.0;
    _matBase[3][0] = 0.0f / 2.0; _matBase[3][1] = 2.0f / 2.0;  _matBase[3][2] = 0.0f / 2.0; _matBase[3][3] = 0.0f / 2.0;
  }
}

void Window::copyControlPointsPatch(int i0, int j0) {
  int i, j, jj, ii;

  for (i = 0; i < _pcPatch[0]->rows(); i++)
  {
    ii = i0 + i;
    for (j = 0; j < _pcPatch[0]->cols(); j++)
    {
      jj = j0 + j;
      _pcPatch[X]->set(i, j, *_pControl[X]->get(ii, jj));
      _pcPatch[Y]->set(i, j, *_pControl[Y]->get(ii, jj));
      _pcPatch[Z]->set(i, j, *_pControl[Z]->get(ii, jj));
      _pcPatch[3]->set(i, j, *_pControl[3]->get(ii, jj));
    }
  }
}

void Window::showControlPoints() {
  int i, j;

  glColor3f(0.0f, 0.8f, 0.0f);
  glPolygonMode(GL_FRONT_AND_BACK, _viewType);
  glPointSize(7.0);
  for (i = 0; i < _pControl[0]->rows(); i++)
  {
    glBegin(GL_POINTS);
    for (j = 0; j < _pControl[0]->cols(); j++)
    {
      glVertex3fv(*_pControl[X]->get(i, j));
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (j = 0; j < _pControl[0]->cols(); j++)
      glVertex3fv(*_pControl[X]->get(i, j));
    glEnd();
  }

  for (i = 0; i < _pControl[0]->cols(); i++)
  {
    glBegin(GL_LINE_STRIP);
    for (j = 0; j < _pControl[0]->rows(); j++)
      glVertex3fv(*_pControl[X]->get(j, i));
    glEnd();
  }
}

int Window::loadPoints(char* fileName)
{
  FILE* fobj;
  char token[40];
  float px, py, pz;
  float* p;
  int i, j, n, m;
  float local_scale = 0.22f;

  printf(" \n ler  %s  \n", fileName);

  if ((fobj = fopen(fileName, "rt")) == NULL)
  {
    printf("Error en la apertura del archivo %s \n", fileName);
    return 0;
  }

  fgets(token, 40, fobj);
  fscanf(fobj, "%s %d %d", token, &n, &m);

  if (_pControl) freeMatrix(_pControl);

  _pControl = new Matrix<f4d> *[n];

  for (i = 0; i < n; i++)
  {
    _pControl[i] = new Matrix<f4d>(n, m);
  }

  fscanf(fobj, "%s", token);  // leitura da linha 0

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < m; j++)
    {
      fscanf(fobj, "%s %f %f %f", token, &px, &py, &pz);

      // p = (float*)malloc(sizeof(float) * 4);
      // p[X] = px * local_scale;
      // p[Y] = py * local_scale;
      // p[Z] = pz * local_scale;
      // p[3] = 1.0f;
      p = new float[4]{ px * local_scale, py * local_scale, pz * local_scale, 1.0f };

      _pControl[0]->set(i, j, &p[0]);
      _pControl[1]->set(i, j, &p[1]);
      _pControl[2]->set(i, j, &p[2]);
      _pControl[3]->set(i, j, &p[3]);

    }
    fscanf(fobj, "%s", token);  // leitura da linha j+1;
  }

  if (_pcPatch) freeMatrix(_pcPatch);

  _pcPatch = new Matrix<f4d> *[4];

  for (i = 0; i < n; i++)
  {
    _pcPatch[i] = new Matrix<f4d>(n, m);
  }

  for (i = 0; i < 4; i++)
  {
    _matBase[i] = new float[4];
  }

  setBaseMatrix(BEZIER);
}

void Window::reshape(int width, int height)
{
  double windW = _width / 2;
  double windH = _height / 2;
  glViewport(0, 0, (GLsizei)_width, (GLsizei)_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (_width <= _height)
    glOrtho(-10.0, 10.0, -10.0 * (GLfloat)_height / (GLfloat)_width,
      10.0 * (GLfloat)_height / (GLfloat)_width, -10.0, 30.0);
  else
    glOrtho(-10.0 * (GLfloat)_width / (GLfloat)_height,
      10.0 * (GLfloat)_width / (GLfloat)_height, -10.0, 10.0, -10.0, 30.0);
  glMatrixMode(GL_MODELVIEW);
}

void Window::keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27:
    exit(0);
  }
}

void Window::motion(int x, int y) {

}

void Window::mouse(int button, int state, int x, int y)
{

}

void Window::handleMenuEvents(int option)
{
  if (option == PtsControle)
    loadPoints("ptosControleSuperficie4x4.txt");
  else if (option == Pontos)
    _viewType = GL_POINTS;
  else if (option == Linha)
    _viewType = GL_LINE_STRIP;
  else if (option == Solido)
    _viewType = GL_QUADS;
  else if (option == sair)
    exit(0);

  if (option == BEZIER || option == BSPLINE || option == CATMULLROM)
    setBaseMatrix(option);

  glutPostRedisplay();
}

void Window::handleColorEvents(int option) {
  switch (option)
  {
  case 0:
    _idColor = 0;
    break;
  case 1:
    _idColor = 1;
    break;
  case 2:
    _idColor = 2;
    break;
  case 3:
    _idColor = 3;
    break;
  case 4:
    _idColor = 4;
    break;
  default:
    _idColor = 0;
    break;
  }
}

void Window::createGLUTMenus()
{
  int menu, submenu, SUBmenuTransladar, SUBmenuGirar, SUBmenuSuperficie, SUBmenuPintar, SUBmenuCores;

  SUBmenuSuperficie = glutCreateMenu(handleMenuEventsCallback);
  glutAddMenuEntry("Bezier", BEZIER);
  glutAddMenuEntry("B-Spline", BSPLINE);
  glutAddMenuEntry("CatmullRom", CATMULLROM);

  SUBmenuGirar = glutCreateMenu(handleMenuEventsCallback);
  glutAddMenuEntry("EixoX", RotarX);
  glutAddMenuEntry("EixoY", RotarY);
  glutAddMenuEntry("EixoZ", RotarZ);


  SUBmenuTransladar = glutCreateMenu(handleMenuEventsCallback);
  glutAddMenuEntry("EixoX", TransladaX);
  glutAddMenuEntry("EixoY", TransladaY);
  // ----------------------------------------------------
  // OBSERVACAO 7:
  // Considerar opcao de Translacao em Y e Z
  // ------------------------------------------------------

  SUBmenuPintar = glutCreateMenu(handleMenuEventsCallback);
  glutAddMenuEntry("Pontos", Pontos);
  glutAddMenuEntry("Malha", Linha);
  glutAddMenuEntry("Preenchido", Solido);

  SUBmenuCores = glutCreateMenu(processColorEventsCallback);
  glutAddMenuEntry("Vermelho", 0);
  glutAddMenuEntry("Verde", 1);
  glutAddMenuEntry("Azul", 2);
  glutAddMenuEntry("Amarelo", 3);
  glutAddMenuEntry("Ciano", 4);

  menu = glutCreateMenu(handleMenuEventsCallback);
  glutAddMenuEntry("Pontos de Controle...", PtsControle);
  glutAddSubMenu("Superficies", SUBmenuSuperficie);
  glutAddSubMenu("Objeto", SUBmenuPintar);
  glutAddSubMenu("Cores", SUBmenuCores);
  glutAddMenuEntry("Escala", Escalar);
  glutAddSubMenu("Rotacao", SUBmenuGirar);
  glutAddSubMenu("Translacao", SUBmenuTransladar);
  glutAddMenuEntry("Sair", sair);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Window::clipVertex(int x, int y)
{

}

void Window::render()
{
  initGLUT();
  init();
  g_CurrentInstance = this;
  glutReshapeFunc(reshapeCallback);
  glutDisplayFunc(drawCallback);
  glutKeyboardFunc(keyboardCallback);

  glutMotionFunc(motionCallback);
  glutMouseFunc(mouseCallback);

  createGLUTMenus();

  glutMainLoop();
}




