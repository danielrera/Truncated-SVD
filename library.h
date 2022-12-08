#include "library.cpp"

double ** create_matrix(int n, int m);
int pgmWrite(char* filename, int rows,int cols,unsigned char **image,char* comment_string);
unsigned char **pgmRead(char *fileName,int *rows,int *cols);
void printMatrix(double **A, int m, int n);
void deleteMatrix(double **A);
double** matrixProd(double **A, double **B, int m, int n, int l);
double** read_matrix(char *name, int *n, int *m);
double normaEucl(double *v, int n);
double prod_vec_vec_Parale(double *v0, double *v1, int n);
void normalizav(double *v, int n);
void proyeccion(double *v0,double *v1, double *p, int n);
void sumaVec_Parale(double *v0,double *v1, double *sum,int n);
void restaVec_Parale(double *v0,double *v1, double *rest, int n);
void prod_mat_vec(double **A, double *v0, double *v1, int n);
double approx_error(double *v0, double *v1, int n);
double **power_method_gen(double **A, double *valp, int n, int m, double tol, int MAX);
double **transposeMat(double **A, int m, int n);
