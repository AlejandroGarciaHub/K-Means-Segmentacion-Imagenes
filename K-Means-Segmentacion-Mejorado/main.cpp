//
//  main.cpp
//  K-Means-Segmentacion-Mejorado
//
//  Created by Miguel Garcia on 9/25/17.
//  Copyright © 2017 Miguel Garcia. All rights reserved.
//



#include "header.h"



int main(int argc, char * argv[]) {
    // insert code here...
    
    int clusterN, flag=0, iter=0;
    double error;
    char *imageName = (char *)malloc(sizeof(char)*20), *centroidPath = (char *)malloc(sizeof(char)*20), *imageOut = (char *)malloc(sizeof(char)*20), *resultsPath = (char *)malloc(sizeof(char)*20); // image name
    imageName = argv[1]; // first agument for image name
    clusterN = atoi(argv[2]);
    centroidPath = argv[3];
    imageOut = argv[4];
    resultsPath= argv[5];
    
    mejoraFlag=atoi(argv[6]);
    iteracionMejora=atoi(argv[7]);
    
    FILE *Results;

    struct timeval t_ini, t_fin;
    double secs, tiempo;
    /*******Inicia tiempo*******/
    gettimeofday(&t_ini, NULL);
    /***************************/
    
    image=readImage(imageName);
    
    centroidsPrincipales=readFileCentroids(clusterN, centroidPath);
//     centroidsPrincipales=readCentroids(image,clusterN,centroidPath);
//
//    return 0;
    
   // centroids.convertTo(centroidsPrincipales, CV_64FC(3));
    

    clustersFlags=Mat::zeros(image.rows, image.cols, CV_8S);

    centroidsFlags=Mat::zeros(clusterN, 1, CV_8S);

    
    clusters = Mat::zeros(image.rows, image.cols, CV_32S);

    do {
        clusterAssignation(image, clusterN);
        centroidCalculation(image, clusterN);
        iter++;
//        printf("Calcula\n");
        globalIter=iter;

    } while (flagChange==1);
    
    /*******Termina tiempo*******/
    gettimeofday(&t_fin, NULL);
    /****************************/
    secs = timeval_diff(&t_fin, &t_ini);
    tiempo = (secs * 1000.0); //Tiempo en milisegundos
    //printf("%.16g milliseconds\n", tiempo); //Imprime en pantalla el tiempo de ejecucion total desde inicio hasta fin en segundos
    
    error= SSE(image, clusters, centroidsPrincipales, clusterN);
    //printf("\n bandera %d error: %f iteraciones %d\n",flag,error,iter);
    Results = fopen(resultsPath,"a");
    fprintf(Results,"%.16g\t %f\t %d\n", tiempo, error, iter); //Imprime en el archivo [Tiempo Error Iteracion]
    fclose(Results);
    display(image, clusters, centroidsPrincipales, clusterN, imageOut);

        printf("Termino\n");
    return 0;
}
