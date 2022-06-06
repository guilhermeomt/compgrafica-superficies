/*=====================================
UENF-CCT-LCMAT-Ciência da Computação
Aluno: Guilherme Oliveira M. Tavares
Data: 04/06/2022
Disciplina: Computação Gráfica
Professor: Luis Antonio Rivera Escriba
Trabalho 02 - Superfícies
=======================================*/

void setBaseMatrix(int sufType)
{
  if (sufType == BEZIER)
  {
    matBase[0][0] = -1.0f; matBase[0][1] = 3.0f;  matBase[0][2] = -3.0f; matBase[0][3] = 1.0f;
    matBase[1][0] = 3.0f; matBase[1][1] = -6.0f; matBase[1][2] = 3.0f; matBase[1][3] = 0.0f;
    matBase[2][0] = -3.0f; matBase[2][1] = 3.0f;  matBase[2][2] = 0.0f; matBase[2][3] = 0.0f;
    matBase[3][0] = 1.0f; matBase[3][1] = 0.0f;  matBase[3][2] = 0.0f; matBase[3][3] = 0.0f;
  }

  if (sufType == BSPLINE)
  {
    matBase[0][0] = -1.0f / 6.0;  matBase[0][1] = 3.0f / 6.0;  matBase[0][2] = -3.0f / 6.0;  matBase[0][3] = 1.0f / 6.0;
    matBase[1][0] = 3.0f / 6.0; matBase[1][1] = -6.0f / 6.0;   matBase[1][2] = 3.0f / 6.0;   matBase[1][3] = 0.0f;
    matBase[2][0] = -3.0f / 6.0;  matBase[2][1] = 0.0f / 6.0;  matBase[2][2] = 3.0f / 6.0;  matBase[2][3] = 0.0f;
    matBase[3][0] = 1.0f / 6.0; matBase[3][1] = 4.0f / 6.0;  matBase[3][2] = 1.0f / 6.0; matBase[3][3] = 0.0;
  }

  if (sufType == CATMULLROM)
  {
    matBase[0][0] = -1.0f / 2.0; matBase[0][1] = 3.0f / 2.0;  matBase[0][2] = -3.0f / 2.0; matBase[0][3] = 1.0f / 2.0;
    matBase[1][0] = 2.0f / 2.0; matBase[1][1] = -5.0f / 2.0; matBase[1][2] = 4.0f / 2.0; matBase[1][3] = -1.0f / 2.0;
    matBase[2][0] = -1.0f / 2.0; matBase[2][1] = 0.0f / 2.0;  matBase[2][2] = 1.0f / 2.0; matBase[2][3] = 0.0f / 2.0;
    matBase[3][0] = 0.0f / 2.0; matBase[3][1] = 2.0f / 2.0;  matBase[3][2] = 0.0f / 2.0; matBase[3][3] = 0.0f / 2.0;
  }
}

matrix* freeMatrix(matrix* suf)
{
  int i;

  if (suf)
  {
    for (i = 0; i < suf->n; i++)
      free(suf->values[i]);

    free(suf->values);
    free(suf);
  }
  return NULL;
}

matrix* allocateMatrix(int n, int m)
{
  matrix* matTemp;
  int j;

  if ((matTemp = (matrix*)malloc(sizeof(matrix))) == NULL)
  {
    printf("\n Error en alocacion de memoria para uma matrix");
    return 0;
  }

  matTemp->n = n;
  matTemp->m = m;
  matTemp->values = (f4d**)calloc(n, sizeof(f4d*));

  for (j = 0; j < matTemp->n; j++)
    matTemp->values[j] = (f4d*)calloc(m, sizeof(f4d));

  return matTemp;
}

void identity()
{
  int a, b;
  for (a = 0; a < 4; a++)
  {
    for (b = 0; b < 4; b++)
    {
      if (a == b)
        matTransf[a][b] = 1;
      else
        matTransf[a][b] = 0;
    }
  }
}

void multMatrix()
{
  int j, k;
  f4d aux;
  for (j = 0; j < pControl->n; j++)
  {
    for (k = 0; k < pControl->m; k++)
    {
      aux[X] = pControl->values[j][k][X];
      aux[Y] = pControl->values[j][k][Y];
      aux[Z] = pControl->values[j][k][Z];
      aux[W] = 1.0; //pControl->values[j][k][W];

      //  Pj = MatTransf4x4 . Pj  <--- transformada homogenea 

      pControl->values[j][k][X] = matTransf[X][X] * aux[X] +
        matTransf[Y][X] * aux[Y] +
        matTransf[Z][X] * aux[Z] +
        matTransf[W][X] * aux[W];

      pControl->values[j][k][Y] = matTransf[X][Y] * aux[X] +
        matTransf[Y][Y] * aux[Y] +
        matTransf[Z][Y] * aux[Z] +
        matTransf[W][Y] * aux[W];

      pControl->values[j][k][Z] = matTransf[X][Z] * aux[X] +
        matTransf[Y][Z] * aux[Y] +
        matTransf[Z][Z] * aux[Z] +
        matTransf[W][Z] * aux[W];

      pControl->values[j][k][W] = matTransf[X][W] * aux[X] +
        matTransf[Y][W] * aux[Y] +
        matTransf[Z][W] * aux[Z] +
        matTransf[W][W] * aux[W];
    }
  }
}

void productVectorParamMatBase(float t, float* tt, float* vr)
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
      vr[i] += matBase[j][i] * tt[j];
  }
}

void productVecMatrix(float* v, f4d** pc, f4d* vr)
{
  int i, j;

  // Vr = V . P   <----- combinacao linear
  // --------------------------------------
  for (i = 0; i < 4; i++)
  {
    vr[i][0] = vr[i][1] = vr[i][2] = 0.0;
    for (j = 0; j < 4; j++)
    {
      vr[i][0] += v[j] * pc[j][i][0];
      vr[i][1] += v[j] * pc[j][i][1];
      vr[i][2] += v[j] * pc[j][i][2];
    }
  }
}
