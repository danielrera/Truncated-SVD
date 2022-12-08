Este programa comprime una imagen en escala de grises (formato pgm) mediante el método de SVD.

Entrada:el nombre de la imagen en formato pgm, el nombre de la imagen procesada, el número k de valores singulares a considerar.
Salida: En pantalla imprimimos el tiempo de ejecución, la tasa de compresión y el MSE. Además, se crea un archivo .pgm que contiene la imagen procesada.

Comando para compilar:
$ g++ svdTruncated.cpp -o svdTruncated -fopenmp -L. library.a

Comando para ejecutar:
$ ./svdTruncated ex1.pgm outImg.pgm 40
