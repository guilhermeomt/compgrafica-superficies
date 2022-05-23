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
    //   DisenaSuperficie();
  }
  glutSwapBuffers();
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
      glVertex3fv(Matrix::to_array(*_pControl[i]));
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (j = 0; j < _pControl[0]->cols(); j++)
      glVertex3fv(Matrix::to_array(*_pControl[i]));
    glEnd();
  }

  for (i = 0; i < _pControl[0]->cols(); i++)
  {
    glBegin(GL_LINE_STRIP);
    for (j = 0; j < _pControl[0]->rows(); j++)
      glVertex3fv(Matrix::to_array(*_pControl[i]));
    glEnd();
  }
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

int Window::LoadPoints(char* fileName)
{
  FILE* fobj;
  char token[40];
  float px, py, pz;
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

  // if (pControle) pControle = liberaMatriz(pControle);

  _pControl = new Matrix * [n];

  for (i = 0; i < n; i++)
  {
    _pControl[i] = new Matrix(n, m);
  }

  fscanf(fobj, "%s", token);  // leitura da linha 0

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < m; j++)
    {
      fscanf(fobj, "%s %f %f %f", token, &px, &py, &pz);

      _pControl[0]->set(i, j, px * local_scale);
      _pControl[1]->set(i, j, py * local_scale);
      _pControl[2]->set(i, j, pz * local_scale);
      _pControl[3]->set(i, j, 1.0);
      printf("%f %f %f \n", _pControl[0]->get(i, j), _pControl[1]->get(i, j), _pControl[2]->get(i, j));
    }
    printf("\n");
    fscanf(fobj, "%s", token);  // leitura da linha j+1;
  }

  // // espaco de matriz para um patch
  // if (pcPatch) pcPatch = liberaMatriz(pcPatch);
  // pcPatch = AlocaMatriz(4, 4);
}

void Window::handleMenuEvents(int option)
{
  if (option == PtsControle)
    LoadPoints("ptosControleSuperficie4x4.txt");
  else if (option == Pontos)
    _viewType = GL_POINTS;
  else if (option == Linha)
    _viewType = GL_LINE_STRIP;
  else if (option == Solido)
    _viewType = GL_QUADS;
  else if (option == sair)
    exit(0);
  // comando = option;

 // if (option == BEZIER || option == BSPLINE || option == CATMULLROM)  // OBSERVACAO: considerar cado de CATMULLROM
 // {
 //   MontaMatrizBase(option);
 // }
 // glutPostRedisplay();
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




