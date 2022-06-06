/*=====================================
UENF-CCT-LCMAT-Ciência da Computação
Aluno: Guilherme Oliveira M. Tavares
Data: 04/06/2022
Disciplina: Computação Gráfica
Professor: Luis Antonio Rivera Escriba
Trabalho 02 - Superfícies
=======================================*/

void scale(int key) {
  if (key == GLUT_KEY_LEFT || key == GLUT_KEY_DOWN) {
    matTransf[0][0] = 0.99;
    matTransf[1][1] = 0.99;
    matTransf[2][2] = 0.99;
  }
  else
    if (key == GLUT_KEY_RIGHT || key == GLUT_KEY_UP) {
      matTransf[0][0] = 1.01;
      matTransf[1][1] = 1.01;
      matTransf[2][2] = 1.01;
    }
}

void translateX(int key, float x) {
  if (key == GLUT_KEY_LEFT)
    matTransf[3][0] = -x;
  else
    if (key == GLUT_KEY_RIGHT)
      matTransf[3][0] = x;
}

void translateY(int key, float y) {
  if (key == GLUT_KEY_UP)
    matTransf[3][1] = y;
  else
    if (key == GLUT_KEY_DOWN)
      matTransf[3][1] = -y;
}

void translateZ(int key) {
  if (key == GLUT_KEY_LEFT || key == GLUT_KEY_DOWN) {
    matTransf[0][0] = 0.99;
    matTransf[1][1] = 0.99;
    matTransf[2][2] = 0.99;
  }
  else
    if (key == GLUT_KEY_RIGHT || key == GLUT_KEY_UP) {
      matTransf[0][0] = 1.01;
      matTransf[1][1] = 1.01;
      matTransf[2][2] = 1.01;
    }
}

void rotateX(int key) {
  if (key == GLUT_KEY_LEFT)
  {
    matTransf[1][1] = cos(-0.01);
    matTransf[1][2] = sin(-0.01);
    matTransf[2][1] = -sin(-0.01);
    matTransf[2][2] = cos(-0.01);
  }
  else
    if (key == GLUT_KEY_RIGHT)
    {
      matTransf[1][1] = cos(0.01);
      matTransf[1][2] = sin(0.01);
      matTransf[2][1] = -sin(0.01);
      matTransf[2][2] = cos(0.01);
    }
}

void rotateY(int key) {
  if (key == GLUT_KEY_UP)
  {
    matTransf[0][0] = cos(-0.01);
    matTransf[0][2] = sin(-0.01);
    matTransf[2][0] = -sin(-0.01);
    matTransf[2][2] = cos(-0.01);
  }
  else
    if (key == GLUT_KEY_DOWN)
    {
      matTransf[0][0] = cos(0.01);
      matTransf[0][2] = sin(0.01);
      matTransf[2][0] = -sin(0.01);
      matTransf[2][2] = cos(0.01);
    }
}

void rotateZ(int key) {
  if (key == GLUT_KEY_LEFT)
  {
    matTransf[0][0] = cos(-0.01);
    matTransf[0][1] = sin(-0.01);
    matTransf[1][0] = -sin(-0.01);
    matTransf[1][1] = cos(-0.01);
  }
  else
    if (key == GLUT_KEY_RIGHT)
    {
      matTransf[0][0] = cos(0.01);
      matTransf[0][1] = sin(0.01);
      matTransf[1][0] = -sin(0.01);
      matTransf[1][1] = cos(0.01);
    }
}