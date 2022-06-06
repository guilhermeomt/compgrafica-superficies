/*=====================================
UENF-CCT-LCMAT-Ciência da Computação
Aluno: Guilherme Oliveira M. Tavares
Data: 04/06/2022
Disciplina: Computação Gráfica
Professor: Luis Antonio Rivera Escriba
Trabalho 02 - Superfícies
=======================================*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <GL/glut.h>

#include "global.h"
#include "transformation.h"
#include "matrix.h"
#include "surface.h"
#include "window.h"

int main(int argc, char** argv)
{
  srand(time(NULL));
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(700, 700);
  glutCreateWindow("Superficies");

  init();

  glutReshapeFunc(reshape);
  glutSpecialFunc(keyboard);
  glutMotionFunc(motion);
  glutMouseFunc(mouse);
  glutDisplayFunc(display);
  createGLUTMenus();

  glutMainLoop();
  return 0;
}

