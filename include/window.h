/*=====================================
UENF-CCT-LCMAT-Ciência da Computação
Aluno: Guilherme Oliveira M. Tavares
Data: 04/06/2022
Disciplina: Computação Gráfica
Professor: Luis Antonio Rivera Escriba
Trabalho 02 - Superfícies
=======================================*/

static void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MAP2_VERTEX_3);
  glEnable(GL_AUTO_NORMAL);
  glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  if (pControl)
  {
    showControlPointsPolygon(pControl);  // mostrando valuess de controle
    drawSurface();                   // disenhando un objeto
  }
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  windW = w / 2;
  windH = h / 2;
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w <= h)
    glOrtho(-10.0, 10.0, -10.0 * (GLfloat)h / (GLfloat)w,
      10.0 * (GLfloat)h / (GLfloat)w, -10.0, 30.0);
  else
    glOrtho(-10.0 * (GLfloat)w / (GLfloat)h,
      10.0 * (GLfloat)w / (GLfloat)h, -10.0, 10.0, -10.0, 30.0);
  glMatrixMode(GL_MODELVIEW);
}

void motion(int x, int y)
{
  int i;
  float dx, dy;
  x = x - windW; y = windH - y;

  dx = x - x_initial;
  dy = y - y_initial;

  int sideX = 0, sideY = 0;

  switch (command) {
  case SCALE:
    if (dy > 0) {
      sideY = GLUT_KEY_UP;
    }
    else {
      sideY = GLUT_KEY_DOWN;
    }

    scale(sideY);
    break;
  case ROTATE_X:
    if (dx > 0) {
      sideX = GLUT_KEY_RIGHT;
    }
    else {
      sideX = GLUT_KEY_LEFT;
    }

    rotateX(sideX);
    break;
  case ROTATE_Y:
    if (dy > 0) {
      sideY = GLUT_KEY_UP;
    }
    else {
      sideY = GLUT_KEY_DOWN;
    }

    rotateY(sideY);
    break;
  case ROTATE_Z:
    if (dx > 0) {
      sideX = GLUT_KEY_RIGHT;
    }
    else {
      sideX = GLUT_KEY_LEFT;
    }

    rotateZ(sideX);
    break;
  case TRANSLATE_X:
    if (dx > 0) {
      sideX = GLUT_KEY_RIGHT;
    }
    else {
      sideX = GLUT_KEY_LEFT;
    }

    translateX(sideX, 0.1);
    break;
  case TRANSLATE_Y:
    if (dy > 0) {
      sideY = GLUT_KEY_UP;
    }
    else {
      sideY = GLUT_KEY_DOWN;
    }

    translateY(sideY, 0.1);
    break;
  case TRANSLATE_Z:
    if (dy > 0) {
      sideY = GLUT_KEY_UP;
    }
    else {
      sideY = GLUT_KEY_DOWN;
    }

    translateZ(sideY);
    break;
  }

  x_initial = dx;
  y_initial = dy;

  if (pControl) {
    multMatrix();
    glutPostRedisplay();
  }
}

void mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN)       //  botão SOLTO
  {
    if (button == GLUT_LEFT_BUTTON)
    {
      x_initial = x - windW;
      y_initial = windH - y;
    }
  }

  if (state == GLUT_UP)
  {
    if (button == GLUT_LEFT_BUTTON)
    {
      xf = x - windW;
      yf = windH - y;
    }
  }

  glutPostRedisplay();
}

void keyboard(int key, int x, int y)
{
  identity();  // identidade em matTransf : matrix de transforma
  switch (command)
  {
  case SCALE:
    scale(key);
    break;
  case ROTATE_X:
    rotateX(key);
    break;
  case ROTATE_Y:
    rotateY(key);
    break;
  case ROTATE_Z:
    rotateZ(key);
    break;
  case TRANSLATE_X:
    translateX(key, 0.1);
    break;
  case TRANSLATE_Y:
    translateY(key, 0.1);
    break;
  case TRANSLATE_Z:
    translateZ(key);
    break;
  }

  if (pControl) {
    multMatrix();
    glutPostRedisplay();
  }
}

void processMenuEvents(int option)
{
  identity();
  if (option == CONTROL_POINTS)
    loadPoints("ptosControleSuperficie4x4.txt");  // ptosControleArbitra4x4   ptosControleSuperficieTrab ptosControleSuperficie4x4
  else if (option == POINTS)
    viewType = GL_POINTS;
  else if (option == LINE)
    viewType = GL_LINE_STRIP;
  else if (option == SOLID)
    viewType = GL_QUADS;
  else if (option == QUIT)
    exit(0);
  else
    command = option;

  if (option == BEZIER || option == BSPLINE || option == CATMULLROM)  // OBSERVACAO: considerar cado de CATMULLROM
  {
    setBaseMatrix(option);
  }
  glutPostRedisplay();
}

void processColorEvents(int option) {
  switch (option)
  {
  case 0:
    colorId = 0;
    break;
  case 1:
    colorId = 1;
    break;
  case 2:
    colorId = 2;
    break;
  case 3:
    colorId = 3;
    break;
  case 4:
    colorId = 4;
    break;
  default:
    colorId = 0;
    break;
  }
}

void createGLUTMenus()
{
  int menu, submenu, SubMenuTranslate, SubMenuRotate, SubMenuSurface, SubMenuPaint, SubMenuColors;

  SubMenuSurface = glutCreateMenu(processMenuEvents);
  glutAddMenuEntry("Bezier", BEZIER);
  glutAddMenuEntry("B-Spline", BSPLINE);
  glutAddMenuEntry("CatmullRom", CATMULLROM);

  SubMenuRotate = glutCreateMenu(processMenuEvents);
  glutAddMenuEntry("EixoX", ROTATE_X);
  glutAddMenuEntry("EixoY", ROTATE_Y);
  glutAddMenuEntry("EixoZ", ROTATE_Z);

  SubMenuTranslate = glutCreateMenu(processMenuEvents);
  glutAddMenuEntry("EixoX", TRANSLATE_X);
  glutAddMenuEntry("EixoY", TRANSLATE_Y);
  glutAddMenuEntry("EixoZ", TRANSLATE_Z);

  SubMenuPaint = glutCreateMenu(processMenuEvents);
  glutAddMenuEntry("Pontos", POINTS);
  glutAddMenuEntry("Malha", LINE);
  glutAddMenuEntry("Preenchido", SOLID);

  SubMenuColors = glutCreateMenu(processColorEvents);
  glutAddMenuEntry("Vermelho", 0);
  glutAddMenuEntry("Verde", 1);
  glutAddMenuEntry("Azul", 2);
  glutAddMenuEntry("Amarelo", 3);
  glutAddMenuEntry("Ciano", 4);

  menu = glutCreateMenu(processMenuEvents);
  glutAddMenuEntry("Pontos de Controle...", CONTROL_POINTS);
  glutAddSubMenu("Superficies", SubMenuSurface);
  glutAddSubMenu("Visao do Objeto", SubMenuPaint);
  glutAddSubMenu("Cores", SubMenuColors);
  glutAddMenuEntry("Escala", SCALE);
  glutAddSubMenu("Rotacao", SubMenuRotate);
  glutAddSubMenu("Transalacao", SubMenuTranslate);
  glutAddMenuEntry("Sair", QUIT);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}