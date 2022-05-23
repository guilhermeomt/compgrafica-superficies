// #include "../include/polygon.h"
// #include "../include/transformation.h"

// float* calculateCenter(struct Polygon(&pvertex)[MAXVERTEXS])
// {
//   int i;
//   float* gCen = new float[3];

//   gCen[0] = gCen[1] = 0.0f;
//   for (i = 0; i < pvertex->n_vertex; i++) {
//     gCen[0] += pvertex[i].v[0];
//     gCen[1] += pvertex[i].v[1];
//   }
//   gCen[0] /= pvertex->n_vertex;
//   gCen[1] /= pvertex->n_vertex;

//   return gCen;
// }

// void Transformation::translate(struct Polygon(&pvertex)[MAXVERTEXS], float dx, float dy)
// {
//   Matrix m1 = Matrix::identity(3, 3);

//   m1.set(0, 2, dx);
//   m1.set(1, 2, dy);

//   for (int i = 0; i < pvertex->n_vertex; i++) {
//     Matrix m2 = Matrix(pvertex[i].v);
//     Matrix m3 = m1 * m2;
//     std::memcpy(&pvertex[i].v, Matrix::to_array(m3), sizeof(pvertex[i].v));
//   }

// }

// void Transformation::rotate(struct Polygon(&pvertex)[MAXVERTEXS], int gVert, float dx, float dy)
// {
//   int i;
//   float oo, theta, xy[3], * gCen;

//   gCen = calculateCenter(pvertex);
//   translate(pvertex, -1 * gCen[0], -1 * gCen[1]);

//   oo = pvertex[gVert].v[1] * dx - pvertex[gVert].v[0] * dy;

//   float	gAng = (2.0f * PHI) / 180.0f;
//   theta = gAng;
//   if (oo > 0.0f)
//     theta = -1.0f * gAng;

//   Matrix m1 = Matrix::identity(3, 3);

//   m1.set(0, 0, cos(theta));
//   m1.set(0, 1, -1 * sin(theta));
//   m1.set(1, 0, sin(theta));
//   m1.set(1, 1, cos(theta));

//   for (i = 0; i < pvertex->n_vertex; i++) {
//     Matrix m2 = Matrix(pvertex[i].v);
//     Matrix m3 = m1 * m2;
//     std::memcpy(&pvertex[i].v, Matrix::to_array(m3), sizeof(pvertex[i].v));
//   }

//   translate(pvertex, gCen[0], gCen[1]);

//   delete gCen;
// }

// void Transformation::scale(struct Polygon(&pvertex)[MAXVERTEXS], int gVert, float dx, float dy)
// {
//   int i;
//   float sx, sy, * gCen;

//   gCen = calculateCenter(pvertex);
//   translate(pvertex, -1 * gCen[0], -1 * gCen[1]);

//   sx = sy = 1.0f;
//   if (fabs(pvertex[gVert].v[0]) > 0.01f)
//     sx = 1.0f + dx / pvertex[gVert].v[0];
//   if (fabs(pvertex[gVert].v[1]) > 0.01f)
//     sy = 1.0f + dy / pvertex[gVert].v[1];

//   Matrix m1 = Matrix::identity(3, 3);
//   m1.set(0, 0, sx);
//   m1.set(1, 1, sy);

//   for (i = 0; i < pvertex->n_vertex; i++) {
//     Matrix m2 = Matrix(pvertex[i].v);
//     Matrix m3 = m1 * m2;
//     std::memcpy(&pvertex[i].v, Matrix::to_array(m3), sizeof(pvertex[i].v));
//   }

//   translate(pvertex, gCen[0], gCen[1]);

//   delete gCen;
// }

// void Transformation::shear(struct Polygon(&pvertex)[MAXVERTEXS], int gVert, float dx, float dy)
// {
//   int i;
//   float sx, sy, xy[3], * gCen;

//   if (dx > dy)
//   {
//     if (fabs(pvertex[gVert].v[0]) > 0.1f)
//       sx = dx / pvertex[gVert].v[1];
//   }
//   else
//   {
//     if (fabs(pvertex[gVert].v[1]) > 0.1f)
//       sy = dy / pvertex[gVert].v[0];
//   }

//   gCen = calculateCenter(pvertex);
//   translate(pvertex, -1 * gCen[0], -1 * gCen[1]);

//   Matrix m1 = Matrix::identity(3, 3);

//   m1.set(0, 1, sx);
//   m1.set(1, 0, sy);

//   for (i = 0; i < pvertex->n_vertex; i++) {
//     Matrix m2 = Matrix(pvertex[i].v);
//     Matrix m3 = m1 * m2;
//     std::memcpy(&pvertex[i].v, Matrix::to_array(m3), sizeof(pvertex[i].v));
//   }

//   translate(pvertex, gCen[0], gCen[1]);

//   delete gCen;
// }

// void Transformation::reflect(struct Polygon(&pvertex)[MAXVERTEXS], float dx, float dy)
// {
//   int i;

//   Matrix m1 = Matrix::identity(3, 3);

//   if (dx > 100.0f || dx < -100.0f)
//   {
//     m1.set(0, 0, -1.0f);
//     m1.set(1, 1, 1.0f);
//   }

//   if (dy > 100.0f || dy < -100.0f)
//   {
//     m1.set(0, 0, 1.0f);
//     m1.set(1, 1, -1.0f);
//   }

//   for (i = 0; i < pvertex->n_vertex; i++) {
//     Matrix m2 = Matrix(pvertex[i].v);
//     Matrix m3 = m1 * m2;
//     std::memcpy(&pvertex[i].v, Matrix::to_array(m3), sizeof(pvertex[i].v));
//   }
// }
