/*=====================================
UENF-CCT-LCMAT-Ciência da Computação
Aluno: Guilherme Oliveira M. Tavares
Data: 04/06/2022
Disciplina: Computação Gráfica
Professor: Luis Antonio Rivera Escriba
Trabalho 02 - Superfícies
=======================================*/

#define LINE -1
#define SOLID -2 
#define POINTS -3

#define X 0
#define Y 1
#define Z 2
#define W 3

#define SCALE 4 

#define VARIATION 0.1f  // variacao do parametros Ex. t = j * VARIA para j = 0, ....< n

#define ROTATE_X 5
#define ROTATE_Y 6
#define ROTATE_Z 7

#define TRANSLATE_X 8
#define TRANSLATE_Y 9
#define TRANSLATE_Z 10

#define CONTROL_POINTS_4 17
#define CONTROL_POINTS_5 18
#define CONTROL_POINTS_RANDOM 19
#define BEZIER      20
#define BSPLINE     21
#define CATMULLROM  22

#define QUIT 0

typedef float f4d[4];   // O tipo de dado f4d � um vetor de 4 elementos

typedef struct st_matrix
{
  int n, m;
  f4d** values;
} matrix;

int command = ROTATE_X;

int viewType = GL_LINE_STRIP;

float local_scale = 0.22f;   // Valor de escala para apresentar no CANVAS


f4d matTransf[4];   // matrix transposta 4 x 4

f4d matBase[4];   // matrix de base 4x4

f4d pView = { 10.0, 10.0, -20.0, 0.0 };

int colorId;

f4d vcolor[5] = { {1.0, 0.0, 0.0, 0.0},
         {0.0, 1.0, 0.0, 0.0},
         {0.0, 0.0, 1.0, 0.0},
         {1.0, 1.0, 0.0, 0.0},
         {0.0, 1.0, 1.0, 0.0}
};

int windW, windH;
int gIndVert = -1;

int x_initial;
int y_initial;
int xf;
int yf;

matrix* pControl = NULL;  // matrix de valuess de controle  LIDOS

matrix* pcPatch = NULL;    // matrix de valuess para um patch 

matrix* ptsPatch = NULL;   // matrix de PONTOS na superficie PATCH calclulados

// matrix.h
void setBaseMatrix(int sufType);
matrix* freeMatrix(matrix* suf);
matrix* allocateMatrix(int n, int m);
void productVectorParamMatBase(float t, float* tt, float* vr);
void productVecMatrix(float* v, f4d** pc, f4d* vr);

// surface.h
void getSurfacePoints(matrix* pcPatch);
void createPatch(int cc);
void createTriangle(int i, int j, int i1, int j1, int i2, int j2, int cc);
void showControlPointsPolygon(matrix* suf);
void copyControlPointsPatch(int i0, int j0, matrix* pcPat);
void drawSurface(void);
int clipVertex(int x, int y);
int loadPoints(char* arch);
int generatePoints();

// transformation.h
void scale(int key);
void translateX(int key, float x);
void translateY(int key, float y);
void translateZ(int key);
void rotateX(int key);
void rotateY(int key);
void rotateZ(int key);

// window.h
static void init(void);
void display(void);
void reshape(int w, int h);
void motion(int x, int y);
void mouse(int button, int state, int x, int y);
void keyboard(int key, int x, int y);
void processMenuEvents(int option);
void processColorEvents(int option);
void createGLUTMenus();

