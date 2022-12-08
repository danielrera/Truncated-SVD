#include "library.h"
#include <chrono>
using namespace std::chrono;

//función que calcula la facorización SVD truncada para los k valores singulares más grandes de una matriz A de mxn
void truncatedSVD(double **A, int m, int n, double ** U, double **S, double **V, int k, double tol, int MAX){
    double *s = new double[k];
    double **At = transposeMat(A,m,n);
    double ** AtA = matrixProd(At,A,n,m,n);
    double **Vt = power_method_gen(AtA,s,n,k,tol,MAX);
    for(int i=0;i<n;i++){
        for(int j=0;j<k;j++){
            V[i][j] = Vt[j][i];
        }
    }
    double **temp = matrixProd(A,V,m,n,k);
    for(int i=0;i<m;i++){
        for(int j=0;j<k;j++){
            if(s[j]!=0){
                U[i][j] = temp[i][j]/sqrt(s[j]);
            }
            else
                U[i][j] = 0;
        }
    }
    for(int i=0;i<k;i++){
        for(int j=0;j<k;j++){
            if(i==j)
               S[i][j] = sqrt(s[i]);
            else
               S[i][j] = 0;
        }
    }
    deleteMatrix(At);
    deleteMatrix(AtA);
    deleteMatrix(Vt);
    deleteMatrix(temp);
    delete s;
}

//función que comprime una imagen mediante el método SVD truncado para k valores singulares
void compressImage(char* inName, char *outName, int k){
    int m,n; //siempre vamos a suponer que m>n
    unsigned char **img = pgmRead(inName,&m,&n);
    double **S = create_matrix(k,k), **U = create_matrix(m,k), ** V=create_matrix(n,k);
    double **A = create_matrix(m,n);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            A[i][j] = (double)img[i][j];
        }
    }
    //medimos el tiempo de ejecución
    auto start = high_resolution_clock::now();
    truncatedSVD(A,m,n,U,S,V,k,10e-8,100000);    
    auto stop = high_resolution_clock::now();
    
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Tiempo de ejecución: "<< duration.count() << " seconds" << endl;
    cout << "Tasa de compresión = " << (double)(m*n)/(k*(m+n+1)) << endl;
    //vamos a calcular el producto USV^T 
    double **Vt = transposeMat(V,n,k);
    double** prod = matrixProd(U,S,m,k,k);
    double** Ak = matrixProd(prod,Vt,m,k,n);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            img[i][j] = (int)Ak[i][j];
        }
    } 
    //reconstruimos la imagen después de haber sido procesada 
    pgmWrite(outName,m,n,img,NULL);   
    //calculamos el error cuadrático medio
    double MSE = 0;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            MSE += (A[i][j]-Ak[i][j])*(A[i][j]-Ak[i][j]);
        }
    }
    MSE = MSE/(double)(m*n);
    cout << "Error cuadratico medio (MSE) = " << MSE << endl;

    delete(img[0]);
    delete(img);
    deleteMatrix(S);
    deleteMatrix(U);
    deleteMatrix(V);
    deleteMatrix(A);
    deleteMatrix(Vt);
    deleteMatrix(prod);
    deleteMatrix(Ak);
}

int main(int argc, char* argv[]){
    int k = atoi(argv[3]);
    compressImage(argv[1],argv[2],k);
    return 0;
}