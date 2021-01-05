#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

#define inFuncaoReta x >= y
#define inFuncaoQuadratica pow(x,2) >= y
#define inCircunferencia sqrt(pow(x,2)+pow(y,2)) <= 1
#define inNoPrimitiva sin(pow(x,2)) >= y

int* vetorPontosDentro;
int nPontos,nThreads;

void zeraVetorPontosDentro(){
    for(register int i=0;i<nPontos;i++){
        vetorPontosDentro[i]=0;
    }
}

int main(int argc, char const *argv[]){
    if (argc<3){printf("Insira 2 argumentos. Ex: %s <nº de pontos> <nº de threads>",argv[0]);return 1;}
    nPontos = atoi(argv[1]);
    nThreads = atoi(argv[2]);
    
    int qtdDentro=0;
    double x, y,Aproximacao;
    
    
    if(nThreads <=1){
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
    }
    
    
    return 0;
}
