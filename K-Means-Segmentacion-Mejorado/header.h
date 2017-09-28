//
//  header.h
//  K-Means-Segmentacion-Mejorado
//
//  Created by Miguel Garcia on 9/25/17.
//  Copyright © 2017 Miguel Garcia. All rights reserved.
//

#ifndef header_h
#define header_h

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <ctime>

using namespace cv;

//Variables
int flagChange;
int cambioCentroide;
int globalIter=0;

int mejoraFlag=1;
int iteracionMejora=100;

Mat image, centroidsPrincipales,centroidsFlags,clusters,clustersFlags,newCentroids;

//Funciones
Mat readFileCentroids(int clusterN,char *path);
Mat readCentroids(Mat image,int clusterN , char *path);
void clusterAssignation(Mat image, int clusterN);
void centroidCalculation(Mat image, int clusterN);

void centroideEstable(int k);

Mat readFileCentroids(int clusterN,char *path){
    static char mydirIn[150] = "";// centroid path
    strcat(mydirIn,path);
    
    FileStorage file(mydirIn, FileStorage::READ);
    
    Mat centroids;// = Mat::zeros(clusterN,1, CV_8UC(3));
    
    file["centroids"] >> centroids;
    
    int renglones = centroids.rows;
    int columnas = centroids.cols;
    // printf("renglones: %d - columnas: %d \n", renglones, columnas);
    
    unsigned char r,g,b;
    for(int i = 0; i < renglones; i++)
    {
        b=centroids.at<Vec3b>(i, 0)[0];
        g=centroids.at<Vec3b>(i, 0)[1];
        r=centroids.at<Vec3b>(i, 0)[2];
        
//        centroids.at<Vec3b>(i,0)[0]=r;
//        centroids.at<Vec3b>(i,0)[1]=g;
//        centroids.at<Vec3b>(i,0)[2]=b;
        //Imprimimos en pantalla
//        printf ("R: %d, G: %d, B: %d \n", r, g, b);
        
    }
    
//    for (int i=0; i<10; i++) {
//        printf("0:%d, 1:%d, 2:%d\n",centroids.at<Vec3b>(i,0)[0],centroids.at<Vec3b>(i,0)[1],centroids.at<Vec3b>(i,0)[2]);
//    }
    
    file.release();
    
    return centroids;
}

Mat readCentroids(Mat image, int clusterN, char *path){
    Mat centroids = Mat::zeros(clusterN,1, CV_8UC(3)); // initilizing centroids in ceros
    
    if( !image.data )
    {
        printf( " La imagen no tiene datos \n " );
        return centroids;
    }
    
    int renglones = image.rows;
    int columnas = image.cols;
    //printf("renglones: %d - columnas: %d\n", renglones, columnas);
    
    //recorrer toda la imagen
    //las coordenadas inician en 0,0
    //int ren,col;//es lo mismo que x,y
    //los colores de los pixeles tienen rojo-r verde-g y azul-b
    //los colores con los vectores de caracteristicas de los pixeles
    //las coordenasdas tambien se pueden utilizar para clasificar los pixeles
    srand(time(NULL));
    unsigned char r,g,b;
    for(int i = 0; i < clusterN; i++)
    {
        int ren = 1 + rand() % renglones;// RANDOM NUMBER between 1-renglones
        int col = 1 + rand() % columnas; // random number between 1-columnas
        //opencv proporciona el rgb en el orden de bgr por eso primero leemos el azul-b
        //Este codigo es para la lectura de los colores de los pixeles
        b=image.at<Vec3b>(ren, col)[0];
        g=image.at<Vec3b>(ren, col)[1];
        r=image.at<Vec3b>(ren, col)[2];
        //Imprimimos en pantalla
        //printf("Coordenadas: x: %d, y: %d - ", ren, col);
        //printf ("R: %d, G: %d, B: %d \n", r, g, b);
        //si se desea alterar el color de un pixel para cada canal se debe hacer lo siguiente
        //image.at<Vec3b>(ren, col)[0]=nuevo valor;
        centroids.at<Vec3b>(i,0)[0] =b;
        centroids.at<Vec3b>(i,0)[1] =g;
        centroids.at<Vec3b>(i,0)[2] =r;
        
    }
    
    //Almacenar cenrtoides dentro de un archivo
    static char mydirOut[150] = ""; //centroid path
    strcat(mydirOut,path);
    
    FileStorage file(mydirOut, FileStorage::WRITE);
    
    file << "centroids" << centroids;
    
    file.release();
    
    //Fin del almacenamiento
    
    
    return centroids;
}


void clusterAssignation(Mat image, int clusterN){
    int i,j,k,group;
    double distancia;
//    printf("Inicia\n");
//    for (int i=0; i<clusterN; i++) {
//        printf("R:%d, G:%d, B:%d\n",centroidsPrincipales.at<Vec3b>(i,0)[0],centroidsPrincipales.at<Vec3b>(i,0)[1],centroidsPrincipales.at<Vec3b>(i,0)[2]);
//    }
//    printf("Termina\n");


        for (i=0; i<image.rows; i++) {
            for (j=0; j<image.cols&&clustersFlags.at<int>(i,j)!=1; j++) {
                distancia=0;
                double min=1e12;
                int ind=1e12;
                for (k=0; k<clusterN&&centroidsFlags.at<int>(k,0)!=1; k++) {
//                    printf("Entro asignar");
  //                  printf("Valor centroid canal %d\n",centroidsPrincipales.at<Vec3b>(k, 0)[0]);
                    distancia=sqrt(pow(image.at<Vec3b>(i, j)[0] - centroidsPrincipales.at<Vec3b>(k, 0)[0],2) + pow(image.at<Vec3b>(i, j)[1] - centroidsPrincipales.at<Vec3b>(k, 0)[1],2) + pow(image.at<Vec3b>(i, j)[2] - centroidsPrincipales.at<Vec3b>(k, 0)[2],2));
//                    printf("Distancia: %lf",distancia);
                    if (distancia<min) {
                        min=distancia;
                        ind=k;
                    }
                        clusters.at<int>(i, j)=ind;
                }
            }
        }

}

void centroidCalculation(Mat image, int clusterN){
    double sum; //Variable que almacena la suma de las caracteristicas en cada columna para un grupo
    int size;   //Numero de ocasiones que se repitio ese grupo
    flagChange=0;   //Bandera de cambio en los centroides
    int i,j,k;
    double sumR,sumG,sumB;
    int cont;
    

    
    for (k=0; k<clusterN&&centroidsFlags.at<int>(k,0)!=1; k++) {
        sumR=0;
        sumG=0;
        sumB=0;
        cont=0;
        for (i=0; i<image.rows; i++) {
            for (j=0; j<image.cols; j++) {

                if (clusters.at<int>(i,j)==k) {
                    cont++;
                    sumR+=image.at<Vec3b>(i,j)[0];
                    sumG+=image.at<Vec3b>(i,j)[1];
                    sumB+=image.at<Vec3b>(i,j)[2];
                }
            }
        }
//        printf("Centroides flags\n");
//        for (int i=0; i<clusterN; i++) {
//            printf("%d\n",centroidsFlags.at<int>(i,0));
//        }
//        printf("Centroids flags end %d\n",globalIter);
        if (centroidsPrincipales.at<Vec3b>(k,0)[0]==int(sumR/cont)&&centroidsPrincipales.at<Vec3b>(k,0)[1]==int(sumG/cont)&&centroidsPrincipales.at<Vec3b>(k,0)[2]==int(sumB/cont)) {
//            printf("Es igual");
            if (globalIter>iteracionMejora&&mejoraFlag) {
                centroideEstable(k);
                centroidsFlags.at<int>(k,0)=1;
//                printf("Centroides flags\n");
//                for (int i=0; i<clusterN; i++) {
//                    printf("%d\n",centroidsFlags.at<int>(i,0));
//                }
//                printf("Centroids flags end %d\n",globalIter);
            }
        }
        else{
//            printf("Es diferente %d",int(sumR/cont));
            flagChange=1;
            centroidsPrincipales.at<Vec3b>(k,0)[0]=int(sumR/cont);
            centroidsPrincipales.at<Vec3b>(k,0)[1]=int(sumG/cont);
            centroidsPrincipales.at<Vec3b>(k,0)[2]=int(sumB/cont);
        }

    }
//    printf("Inicia2\n");
//    for (int i=0; i<clusterN; i++) {
//        printf("R:%d, G:%d, B:%d\n",centroidsPrincipales.at<Vec3b>(i,0)[0],centroidsPrincipales.at<Vec3b>(i,0)[1],centroidsPrincipales.at<Vec3b>(i,0)[2]);
//    }
//    printf("Termina2\n");
}

void centroideEstable(int k){
    for (int i=0; i<image.rows; i++) {
        for (int j=0; j<image.cols; j++) {
            if (clusters.at<int>(i,j)==k) {
                clustersFlags.at<int>(i,j)=1;
            }
        }
    }
}

/* Image loading */
Mat readImage(char *path){
    
    Mat image = imread( path, 1 );
    
    if( !image.data )
    {
        printf( " La imagen no tiene datos \n " );
        return image;
    }
    
    int renglones = image.rows;
    int columnas = image. cols;
    
    // printf("renglones: %d - columnas: %d\n", renglones, columnas);
    
    //recorrer toda la imagen
    //las coordenadas inician en 0,0
    //int ren,col;//es lo mismo que x,y
    //los colores de los pixeles tienen rojo-r verde-g y azul-b
    //los colores con los vectores de caracteristicas de los pixeles
    //las coordenasdas tambien se pueden utilizar para clasificar los pixeles
    unsigned char r,g,b;
    for(int ren = 0; ren < renglones; ren++)
    {
        for(int col = 0; col < columnas; col++)
        {
            //opencv proporciona el rgb en el orden de bgr por eso primero leemos el azul-b
            //Este codigo es para la lectura de los colores de los pixeles
            b=image.at<Vec3b>(ren, col)[0];
            g=image.at<Vec3b>(ren, col)[1];
            r=image.at<Vec3b>(ren, col)[2];
            //Imprimimos en pantalla
            /*printf("Coordenadas: x: %d, y: %d - ", ren, col);
             printf ("R: %d, G: %d, B: %d \n", r, g, b);*/
            
            //si se desea alterar el color de un pixel para cada canal se debe hacer lo siguiente
            //image.at<Vec3b>(ren, col)[0]=nuevo valor;
        }
    }
    return (image);
    
}

/* Retorna "a - b" en segundos */
double timeval_diff(struct timeval *a, struct timeval *b){
    return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

double SSE(Mat image, Mat clusters, Mat centroids, int clusterN){
    int i,k;
    double error = 0;
    
    for (i = 0; i < clusterN; i++){ //Numero de objetos
        
        for (int ren = 0; ren < image.rows; ren++){ //Numero de centroides
            for (int col =0 ; col < image.cols; col++){
                int sum = 0;
                
                if(clusters.at<int>(ren,col) == i){
                    
                    for (k = 0; k < 3; k++){ //Numero de caracteristicas
                        sum+= pow( int(centroids.at<Vec3b>(i,0)[k]) - int(image.at<Vec3b>(ren,col)[k]) , 2);
                    }
                    error += sqrt(sum);
                }
            }
        }
    }
    return error;
    
}

void display(Mat image, Mat clusters, Mat centroids, int clusterN, char *path){
    int i,k;
    
    for (i = 0; i < clusterN; i++){ //cluster number
        
        for (int ren = 0; ren < image.rows; ren++){ //rows
            for (int col =0 ; col < image.cols; col++){ //colums
                
                if(clusters.at<int>(ren,col) == i){
                    
                    for (k = 0; k < 3; k++){ //Numero de caracteristicas
                        image.at<Vec3b>(ren,col)[k]=centroids.at<Vec3b>(i,0)[k];
                    }
                }
            }
        }
    }
    //namedWindow( "final", CV_WINDOW_AUTOSIZE );
    //imshow("final", image );
    imwrite( path, image );
}




#endif /* header_h */
