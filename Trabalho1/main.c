#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

#define inFuncaoReta x >= y
#define inFuncaoQuadratica pow(x,2) >= y
#define inCircunferencia sqrt(pow(x,2)+pow(y,2)) <= 1
#define inNoPrimitiva sin(pow(x,2)) >= y

typedef struct{
    int resultParcialReta;
    int resultParcialQuadratica;
    int resultParcialCircunferencia;
    int resultParcialNoPrimitiva;
}ResultParciais;


ResultParciais* vetorResult;
int nPontos,nThreads;

void* MontCarlo(void* i){
    double x,y;
    long int numThread = (long int)i;
    int qtdPontos = nPontos/nThreads;
    if(numThread == nThreads-1 && (nPontos%nThreads)!=0){qtdPontos=nPontos%nThreads;}
    for (register int i = 0; i < qtdPontos; i++){
        x = drand48();
        y = drand48();
        if(inFuncaoReta){vetorResult[numThread].resultParcialReta++;}
        if(inFuncaoQuadratica){vetorResult[numThread].resultParcialQuadratica++;}
        if(inCircunferencia){vetorResult[numThread].resultParcialCircunferencia++;}
        if(inNoPrimitiva){vetorResult[numThread].resultParcialNoPrimitiva++;}
        
    }
    
}

int main(int argc, char const *argv[]){
    if (argc<3){printf("Insira 2 argumentos. Ex: %s <nº de pontos> <nº de threads>",argv[0]);return 1;}
    nPontos = atoi(argv[1]);
    nThreads = atoi(argv[2]);
    
    int qtdDentro=0;
    double x, y,Aproximacao;
    double start,finish;
    
    if(nThreads <=1){
        ResultParciais resultTot;
        GET_TIME(start);
        //area da função y = x
        for (register int i = 0; i < nPontos; i++){
           x = drand48();
           y = drand48();
           if(inFuncaoReta){qtdDentro++;}
        }
        Aproximacao = ((double)qtdDentro)/((double)nPontos);
        printf("A aproximação da area da função y = x delimitada entre [0,1] é: %lf\n",Aproximacao);
        qtdDentro=0;

        //area de 1/4 de uma circunferencia
        for (register int i = 0; i < nPontos; i++){
           x = drand48();
           y = drand48();
           if(inCircunferencia){qtdDentro++;}
        }
        Aproximacao = ((double)qtdDentro)/((double)nPontos);
        printf("A aproximação da area do primeiro quadrante de uma circunferencia é: %lf\n",Aproximacao);
        qtdDentro=0;

        //area da função x²
        for (register int i = 0; i < nPontos; i++){
           x = drand48();
           y = drand48();
           if(inFuncaoQuadratica){qtdDentro++;}
        }
        Aproximacao = ((double)qtdDentro)/((double)nPontos);
        printf("A aproximação da area da função y = x² delimitada entre [0,1] é: %lf\n",Aproximacao);
        qtdDentro=0;

        //area da função e^(x^2) M_E
        for (register int i = 0; i < nPontos; i++){
           x = drand48();
           y = drand48();
           if(inNoPrimitiva){
               qtdDentro++;
           }
        }
        Aproximacao = ((double)qtdDentro)/((double)nPontos);
        printf("A aproximação da area da função y = seno(x^2) delimitada entre [0,1] é: %lf\n",Aproximacao);
        GET_TIME(finish);
        printf("Tempo de execução (sequencial): %lf",finish-start);
    }
    else{
        pthread_t tid_array[nThreads];
        vetorResult = malloc(sizeof(ResultParciais)*nThreads);
        for (register int i = 0; i < nThreads; i++){
            vetorResult[i].resultParcialReta=0;
            vetorResult[i].resultParcialQuadratica=0;
            vetorResult[i].resultParcialCircunferencia=0;
            vetorResult[i].resultParcialNoPrimitiva=0;
        }        
        GET_TIME(start);
        for (register long int i = 0; i < nThreads; i++){
            if(pthread_create(&tid_array[i],NULL,MontCarlo,(void*)i)){printf("PTHREAD_CREATE -- ERRO");return 2;}
        }

        for (register int i = 0; i < nThreads; i++){
            if(pthread_join(tid_array[i],NULL)){printf("PTHREAD_JOIN -- ERRO");return 3;}
        }
        GET_TIME(finish);
        int qtdDentroReta=0, qtdDentroQuadratica=0, qtdDentroCircunferencia=0, qtdDentroNoPrimitiva=0;
        double totPontos= (double)nPontos;
        double totReta,totQuadratica,totCircunferencia,totNoPrimitiva;
        for (register int i = 0; i < nThreads; i++){
            qtdDentroReta+=vetorResult[i].resultParcialReta;
            qtdDentroQuadratica+=vetorResult[i].resultParcialQuadratica;
            qtdDentroCircunferencia+=vetorResult[i].resultParcialCircunferencia;
            qtdDentroNoPrimitiva+=vetorResult[i].resultParcialNoPrimitiva;
        }
        totReta=(double)qtdDentroReta/totPontos;
        totQuadratica=(double)qtdDentroQuadratica/totPontos;
        totCircunferencia=(double)qtdDentroCircunferencia/totPontos;
        totNoPrimitiva=(double)qtdDentroNoPrimitiva/totPontos;

        printf("A aproximação da area da função y = x delimitada entre [0,1] é: %lf\n",totReta);
        printf("A aproximação da area do primeiro quadrante de uma circunferencia é: %lf\n",totCircunferencia);
        printf("A aproximação da area da função y = x² delimitada entre [0,1] é: %lf\n",totQuadratica);
        printf("A aproximação da area da função y = seno(x^2) delimitada entre [0,1] é: %lf\n",totNoPrimitiva);
        printf("Tempo de execução das threads (concorrente): %lf \n ",finish-start);

        free(vetorResult);
    }
    return 0;
}
