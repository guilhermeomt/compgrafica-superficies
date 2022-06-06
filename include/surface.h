/*=====================================
UENF-CCT-LCMAT-Ciência da Computação
Aluno: Guilherme Oliveira M. Tavares
Data: 04/06/2022
Disciplina: Computação Gráfica
Professor: Luis Antonio Rivera Escriba
Trabalho 02 - Superfícies
=======================================*/

void getSurfacePoints(matrix* pcPatch)
{
  int i, j, h, n, m;
  float t, s;
  float tmp[4], vsm[4], vtm[4];
  f4d va[4];

  if (!pcPatch) return;

  n = 0;

  for (s = 0; s <= 1.01; s += VARIATION) n += 1;

  m = n;

  if (ptsPatch) ptsPatch = freeMatrix(ptsPatch);

  ptsPatch = allocateMatrix(n, m);

  s = 0.0f;
  for (i = 0; i < ptsPatch->n; i++)
  {
    t = 0.0f;
    for (j = 0; j < ptsPatch->m; j++)
    {
      productVectorParamMatBase(s, tmp, vsm);    // vsm = S G
      productVectorParamMatBase(t, tmp, vtm);    // vtm = G^t T

      productVecMatrix(vsm, pcPatch->values, va);    // va = S G P = vsm P

      ptsPatch->values[i][j][0] = 0.0f;
      ptsPatch->values[i][j][1] = 0.0f;
      ptsPatch->values[i][j][2] = 0.0f;

      for (h = 0; h < 4; h++)						// p = S G P G^t T = va vtm
      {
        ptsPatch->values[i][j][0] += va[h][0] * vtm[h];
        ptsPatch->values[i][j][1] += va[h][1] * vtm[h];
        ptsPatch->values[i][j][2] += va[h][2] * vtm[h];
      }
      t += VARIATION;
    }
    s += VARIATION;
  }
}

void createPatch(int cc)
{
  int i, j;
  float t, v, s;
  f4d a, b, n, l, cx, cy, c;

  if (!ptsPatch)  return;

  switch (viewType)
  {
  case GL_POINTS:
    glColor3f(0.0f, 0.0f, 0.7f);
    glPointSize(1.0);
    for (i = 0; i < ptsPatch->n; i++)
    {
      glBegin(viewType);
      for (j = 0; j < ptsPatch->m; j++)
        glVertex3fv(ptsPatch->values[i][j]);
      glEnd();
    }
    break;

  case GL_LINE_STRIP:
    glColor3f(0.0f, 0.0f, 0.7f);
    for (i = 0; i < ptsPatch->n; i++)
    {
      glBegin(viewType);
      for (j = 0; j < ptsPatch->m; j++)
        glVertex3fv(ptsPatch->values[i][j]);
      glEnd();
    }

    for (j = 0; j < ptsPatch->n; j++)
    {
      glBegin(viewType);
      for (i = 0; i < ptsPatch->m; i++)
        glVertex3fv(ptsPatch->values[i][j]);
      glEnd();
    }
    break;

  case GL_QUADS:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (i = 0; i < ptsPatch->n - 1; i++)
    {
      for (j = 0; j < ptsPatch->m - 1; j++)
      {

        // criando 1o triangulo
        createTriangle(i, j, i, j + 1, i, j, cc);

        // criando 2o triangulo
        createTriangle(i, j, i + 1, j, i, j, cc);

        // criando 3o triangulo
        createTriangle(i, j, i + 1, j + 1, i + 1, j, cc);

        // criando 4o triangulo
        createTriangle(i, j, i + 1, j + 1, i, j + 1, cc);
      }
    }
    break;
  }

}

void createTriangle(int i, int j, int i1, int j1, int i2, int j2, int cc) {
  float t, v, s;
  f4d a, b, n, l, cx, cy, c;

  a[X] = ptsPatch->values[i + 1][j][X] - ptsPatch->values[i][j][X];
  a[Y] = ptsPatch->values[i + 1][j][Y] - ptsPatch->values[i][j][Y];
  a[Z] = ptsPatch->values[i + 1][j][Z] - ptsPatch->values[i][j][Z];

  b[X] = ptsPatch->values[i + 1][j + 1][X] - ptsPatch->values[i][j][X];
  b[Y] = ptsPatch->values[i + 1][j + 1][Y] - ptsPatch->values[i][j][Y];
  b[Z] = ptsPatch->values[i + 1][j + 1][Z] - ptsPatch->values[i][j][Z];

  c[X] = (ptsPatch->values[i + 1][j + 1][X] - ptsPatch->values[i][j][X]) / 2 + ptsPatch->values[i][j][X];
  c[Y] = (ptsPatch->values[i + 1][j + 1][Y] - ptsPatch->values[i][j][Y]) / 2 + ptsPatch->values[i][j][Y];
  c[Z] = (ptsPatch->values[i + 1][j + 1][Z] - ptsPatch->values[i][j][Z]) / 2 + ptsPatch->values[i][j][Z];

  n[X] = a[Y] * b[Z] - a[Z] * b[Y];
  n[Y] = a[Z] * b[X] - a[X] * b[Z];
  n[Z] = a[X] * b[Y] - a[Y] * b[X];

  s = sqrt(n[X] * n[X] + n[Y] * n[Y] + n[Z] * n[Z]);

  n[X] /= s; n[Y] /= s; n[Z] /= s;

  l[X] = pView[X] - ptsPatch->values[i1][j1][X];
  l[Y] = pView[Y] - ptsPatch->values[i1][j1][Y];
  l[Z] = pView[Z] - ptsPatch->values[i1][j1][Z];

  s = n[X] * l[X] + n[Y] * l[Y] + n[Z] * l[Z];

  v = sqrt(l[X] * l[X] + l[Y] * l[Y] + l[Z] * l[Z]);
  t = s / v;

  if (t < 0.0f)
    t *= -1.00f;

  glBegin(GL_POLYGON);
  glColor3f(t * vcolor[cc][X], t * vcolor[cc][Y], t * vcolor[cc][Z]);
  glNormal3fv(n);
  glVertex3fv(ptsPatch->values[i2][j2]);
  glVertex3fv(c);
  glVertex3fv(ptsPatch->values[i1][j1]);
  glEnd();
}

void showControlPointsPolygon(matrix* suf)
{
  int i, j;

  glColor3f(0.0f, 0.8f, 0.0f);
  glPolygonMode(GL_FRONT_AND_BACK, viewType);
  glPointSize(7.0);
  for (i = 0; i < suf->n; i++)
  {
    glBegin(GL_POINTS);
    for (j = 0; j < suf->m; j++)
    {
      glVertex3fv(suf->values[i][j]);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (j = 0; j < suf->m; j++)
      glVertex3fv(suf->values[i][j]);
    glEnd();
  }

  for (i = 0; i < suf->m; i++)
  {
    glBegin(GL_LINE_STRIP);
    for (j = 0; j < suf->n; j++)
      glVertex3fv(suf->values[j][i]);
    glEnd();
  }
}

void copyControlPointsPatch(int i0, int j0, matrix* pcPat)
{
  int i, j, jj, ii;

  // copiar n x m valuess desde (i0, j0) apartir da matrix pControl
  for (i = 0; i < pcPat->n; i++)
  {
    ii = i0 + i;
    for (j = 0; j < pcPat->m; j++)
    {
      jj = j0 + j;
      pcPat->values[i][j][0] = pControl->values[ii][jj][0];
      pcPat->values[i][j][1] = pControl->values[ii][jj][1];
      pcPat->values[i][j][2] = pControl->values[ii][jj][2];
      pcPat->values[i][j][3] = pControl->values[ii][jj][3];
    }
  }
}

void drawSurface(void)
{
  int i, j, nn, mm;

  nn = pControl->n - 3;   // numero de descolamentos (patchs)

  for (i = 0; i < nn; i++)
  {
    mm = pControl->m - 3;
    for (j = 0; j < mm; j++)
    {
      copyControlPointsPatch(i, j, pcPatch);  // copiar ptos de controle em matrix 4 x 4
      getSurfacePoints(pcPatch);        // calculos valuess do PATCH com os ptos de Contrle em pcPatch 
      createPatch(colorId);
    }
  }
}

int clipVertex(int x, int y)
{
  int i, j;
  double d;
  gIndVert = -1;
  // para cada vértice do values de controle
  printf("----- clipVertex -----\n");
  for (i = 0; i < pControl->n; i++)
  {
    for (j = 0; j < pControl->m; j++)
    {
      float valuesX = (double)pControl->values[i][j][0];
      float valuesY = (double)pControl->values[i][j][1];

      float dx = valuesX - x;
      dx *= local_scale;
      float dy = valuesY - y;
      dy *= local_scale;
      d = sqrt(pow((valuesX - dx), 2.0) + pow((valuesY - dy), 2.0));

      printf("%f %f %f\n", dx, dy, d);
      // distancia do values (x, y) a cada vértice do poligono
      // se a distancia d é bem proxima ( d < 3 pixel)

      if (d < 5.0)
      {
        gIndVert = i;    // achou o indice do vértice
        break;
      }
    }
  }
  return gIndVert;
}

int loadPoints(char* arch)
{
  FILE* fobj;
  char token[40];
  float px, py, pz;
  int i, j, n, m;

  printf(" \n ler  %s  \n", arch);

  if ((fobj = fopen(arch, "rt")) == NULL)
  {
    printf("Error en la apertura del archivo %s \n", arch);
    return 0;
  }

  fgets(token, 40, fobj);
  fscanf(fobj, "%s %d %d", token, &n, &m);

  if (pControl) pControl = freeMatrix(pControl);

  pControl = allocateMatrix(n, m);

  fscanf(fobj, "%s", token);  // leitura da linha 0

  for (j = 0; j < pControl->n; j++)
  {
    for (i = 0; i < pControl->m; i++)
    {
      fscanf(fobj, "%s %f %f %f", token, &px, &py, &pz);

      pControl->values[j][i][0] = px * local_scale;
      pControl->values[j][i][1] = py * local_scale;
      pControl->values[j][i][2] = pz * local_scale;
      pControl->values[j][i][3] = 1.0f;

    }
    fscanf(fobj, "%s", token);  // leitura da linha j+1;
  }

  // espaco de matrix para um patch
  if (pcPatch) pcPatch = freeMatrix(pcPatch);
  pcPatch = allocateMatrix(4, 4);

}