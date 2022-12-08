#include <bits/stdc++.h>
#include <math.h>
#define MAX_TREE_H 10000
#define MAXLENGTH 100
#define MAXVALUE  255
#define pi 3.14159265358979311600


using namespace std;

unsigned char **pgmRead(char *fileName,int *rows,int *cols)
{
      FILE *filePointer;   
      char line[MAXLENGTH]; /* for character input from file */
      int maximumValue = 0; /* max value from header */
      int binary;           /* flag to indicate if file is binary (P5) or ascii (P2)*/
      long numberRead = 0;  /* counter for number of pixels read */
      long i,j;             /* (i,j) for loops */
      int test,temp;        /* for detecting EOF(test) and temp storage */
      unsigned char *tempPtr, **image;

      /* Open the file, return an error if necessary. */
      if ((filePointer = fopen(fileName,"r")) == NULL) {
	   printf ("ERROR: cannot open file\n\n");
	   fclose (filePointer);
	   return (0);
      }
    
      *cols = *rows =0;

      /* Check the file signature ("Magic Numbers" P2 and P5); skip comments
         and blank lines (CR with no spaces before it).*/
      fgets (line,MAXLENGTH,filePointer);
      while (line[0]=='#' || line[0]=='\n') 
           fgets (line,MAXLENGTH,filePointer);

      if (line[0]=='P' && (line[1]=='2')) {
	   binary = 0;
	 /*   printf ("\nFile Format: P2\n"); */
      }
      else if (line[0]=='P' && (line[1]=='5')) {
	   binary = 1;
	  /*  printf ("\nFORMAT: P5\n"); */
      }
      else {
	   printf ("ERROR: incorrect file format\n\n");
	   fclose (filePointer);
	   return (0);
      } 

      /* Input the width, height and maximum value, skip comments and blank
       * lines. */
      fgets (line,MAXLENGTH,filePointer);
      while (line[0]=='#' || line[0]=='\n') 
         fgets (line,MAXLENGTH,filePointer);
      sscanf (line,"%d %d",cols,rows);

      fgets (line,MAXLENGTH,filePointer);
      while (line[0]=='#' || line[0]=='\n') 
         fgets(line,MAXLENGTH,filePointer);
      sscanf (line,"%d",&maximumValue);

      if ((*cols)<1 ||(*rows)<1 || maximumValue<0 || maximumValue>MAXVALUE){
	   printf ("ERROR: invalid file specifications (cols/rows/max value)\n\n");
	   fclose (filePointer);
	   return (0);
      }

      // creating memory for the input image
      image = (unsigned char **)malloc((*rows) * sizeof(unsigned char *));
      if (image == NULL)
      {
         puts("\nFailure to allocate room for pointers");
         return(NULL);
      }

      tempPtr = (unsigned char *) malloc((*rows)*(*cols)*sizeof(unsigned char));
      if (tempPtr == NULL)
      {
         puts("\nFailure to allocate room for the array");
         return(NULL);
      }
      for (i = 0; i < *rows; i++)
      {
        image[i] = tempPtr + ( i * (*cols) );
      }


      /* Read in the data for binary (P5) or ascii (P2) PGM formats   */
      if (binary) {
	   for (i = 0; i < (*rows); i++) {
	        numberRead += fread((void *)&(image[i][0]),
		  sizeof(unsigned char),(*cols),filePointer); 
		if (feof(filePointer)) break;
	   }
      }
      else {
	   for (i= 0; i < (*rows); i++) {
	        for (j =0; j < (*cols); j++) { 
	             test = fscanf (filePointer,"%d",&temp);
		     if (test == EOF) break;
		     image[i][j] = (unsigned char)temp;

		     numberRead++;
		}
		if (test == EOF) break;
	   }
      }

      /* close the file and return 1 indicating success */
      fclose (filePointer);
      return (image);
}

/* INPUT: a filename (char*), the dimensions of the pixmap (rows,cols of
 *   type long), and a pointer to a 2D array (MAXROWS x MAXCOLS) in row
 *   major order.
 * OUTPUT: an integer is returned indicating if the desired file was written
 *   (in P5 PGM format (binary)).  A 1 is returned if the write was completed
 *   and 0 if it was not.  An error message is returned if the file is not
 *   properly opened.  
 */ 
int pgmWrite(char* filename, int rows,int cols,unsigned char **image,char* comment_string)
{
      FILE* file;        /* pointer to the file buffer */
      int maxval;        /* maximum value in the image array */
      long nwritten = 0; /* counter for the number of pixels written */
      long i,j;          /* for loop counters */

      /* open the file; write header and comments specified by the user. */
      if ((file = fopen(filename, "w")) == NULL)	{
           printf("ERROR: file open failed\n");
	   return(0);
      }
      fprintf(file,"P5\n");

      if (comment_string != NULL) fprintf(file,"# %s \n", comment_string);
    
      /* write the dimensions of the image */	
      fprintf(file,"%d %d \n", cols, rows);

      /* NOTE: MAXIMUM VALUE IS WHITE; COLOURS ARE SCALED FROM 0 - */
      /* MAXVALUE IN A .PGM FILE. */
      
      /* WRITE MAXIMUM VALUE TO FILE */
      fprintf(file, "%d\n", (int)255);

      /* Write data */
      for (i = 0;i < rows;i++) {
          nwritten += fwrite((void*)image[i],sizeof(unsigned char),cols,file);
      }      	
      fclose(file);
      return(1);
}


//función que crea una matriz de tamaño mxn con memoria secuencial 
double ** create_matrix(int m, int n){
   double *ptr = new double[m*n];
   double **A = new double*[m];
   for(int i=0;i<m;i++)
      A[i] = ptr + (i*n);
   return A;
}


void printMatrix(double **A, int m, int n){
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
}

//elimina una matriz con memoria secuencial
void deleteMatrix(double **A){
   delete A[0];
   delete A;
}

double** matrixProd(double **A, double **B, int m, int n, int l){
   double **prod = create_matrix(m,l);
   for(int i=0;i<m;i++){
      for(int j=0;j<l;j++){
         double suma = 0;
         #pragma omp parallel for reduction(+:suma)
         for(int k=0;k<n;k++){
            suma += A[i][k]*B[k][j];
         }
         prod[i][j] = suma;
      }
   }
   return prod;
}

//función que lee una matriz de un archivo de texto
double** read_matrix(char *name, int *n, int *m){
   ifstream file(name);
   if(!file) {
      cerr << "Error: el archivo no pudo ser abierto" << endl;
      exit(1);
   }
   string mystring;
   file >> mystring;
   *n = atoi(mystring.c_str());
   file >> mystring;
   *m = atoi(mystring.c_str());

   double ** M = create_matrix(*n,*m);

   for(int i=0; i<*n;i++){
     for(int j=0; j<*m;j++){
       file >> mystring;
       M[i][j]=atof(mystring.c_str());
     }
   }
   file.close();
   return M;
}

//calcula la norma euclidiana
double normaEucl(double *v, int n){
    double sum =0;
    for(int i=0;i<n;i++){
        sum+=v[i]*v[i];
    }
    sum = sqrt(sum);
    return sum;
}

double prod_vec_vec_Parale(double *v0, double *v1, int n){
    double sum =0;
    #pragma omp parallel for reduction(+:sum) 
    for(int i=0;i<n;i++){
        sum+=v0[i]*v1[i];
    }
    return sum;
}


//Normaliza un vector
void normalizav(double *v, int n){
    double norm = normaEucl(v,n);
    for(int i=0;i<n;i++){
        v[i]=v[i]/norm;
    }
}

//función que proyecta el vector v0 sobre v1
void proyeccion(double *v0,double *v1, double *p, int n){
    double a = prod_vec_vec_Parale(v1,v0,n);
    for(int i=0;i<n;i++){
        p[i] = a*v1[i]; 
    }
}

void sumaVec_Parale(double *v0,double *v1, double *sum,int n){
    #pragma omp parallel for
    for(int i=0;i<n;i++){
        sum[i] = v0[i]+v1[i];
    }
}

void restaVec_Parale(double *v0,double *v1, double *rest, int n){
    #pragma omp parallel for
    for(int i=0;i<n;i++){
        rest[i] = v0[i]-v1[i];
    }
}

//Realiza el producto entre una matriz y un vector
void prod_mat_vec(double **A, double *v0, double *v1, int n){
    for(int i=0; i<n;i++){
        v1[i] = 0;
    }
    for(int i=0;i<n; i++){
        for(int j=0;j<n;j++){
            v1[i] += A[i][j]*v0[j];
        }
    }
}

//Calcula el error de aproximacion entre v0 y v1
double approx_error(double *v0, double *v1, int n){
    double *subs = new double[n];
    double *sum = new double[n];
    for(int i=0;i<n;i++){
        subs[i] = v0[i]-v1[i];
        sum[i] = v0[i]+v1[i];
    }
    double error1 = normaEucl(subs,n), error2 = normaEucl(sum,n);
    delete subs;
    delete sum;
    return min(error1,error2);
}


//método de la potencia generalizado para calcular los m valores propios más grandes de una matriz A de nxn
double **power_method_gen(double **A, double *valp, int n, int m, double tol, int MAX){
    srand(time(NULL));
    double error;
    double ** phi = create_matrix(m,n);
    double *v0 = new double[n], *v1 = new double[n], *suma = new double[n], *p = new double[n];
    //Vamos a repetir el proceso de la Potencia para los m valores propios
    for(int i=0;i<m;i++){
        //Generamos un vector inicial v0
        for(int j=0;j<n;j++){
            v0[j] = rand();
        }
        normalizav(v0,n);
        int k = 0;
        //iteramos hasta converger a un valor y vector propio
        while(k<MAX){
            //Primero a v0 le quitamos las contribuciones de los anteriores valores y vectores propios
            if(i>0){
                for(int j=0;j<n;j++){
                    suma[j] = 0;
                }
                for(int j=0;j<i;j++){
                    proyeccion(v0,phi[j],p,n);
                    sumaVec_Parale(suma,p,suma,n);
                }
                restaVec_Parale(v0,suma,v0,n);
                normalizav(v0,n);
            }

            prod_mat_vec(A,v0,v1,n);
            valp[i] = prod_vec_vec_Parale(v1,v0,n);
            normalizav(v1,n);
            error = approx_error(v0,v1,n); 
            if(error < tol){
                break;
            }
            for(int j=0;j<n;j++){
                v0[j] = v1[j];
            }
            k++;
        }
        for(int j=0;j<n;j++){
            phi[i][j] = v1[j]; 
        }
    }
    delete v0;
    delete v1;
    delete suma;
    delete p;
    return phi; 
}

double **transposeMat(double **A, int m, int n){
    double ** At = create_matrix(n,m);
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            At[i][j] = A[j][i];
        }
    }
    return At;
}
