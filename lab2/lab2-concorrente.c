#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

float *mat;
float *vet;
float *saida;

#define nThread 4

typedef struct{
    int id;
    int dim;
    int indexInicial;
    int indexFinal;
} tArgs;

void * tarefa(void *arg){
    tArgs *args = (tArgs*) arg;

    for (int i = (args->indexInicial); i < (args->indexFinal); i++)    
        for (int j = 0; j < (args->dim); j++)
                saida[i]+= mat[i*(args->dim)+j]*vet[j];
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    if (argc<2){
        printf("Digite: %s <dimensao da matriz>\n",argv[0]);
        return 1;
    }

    int dim = atoi(argv[1]);
    pthread_t *tid;
    tArgs *args;    

    //alocação
    mat = (float*) malloc(sizeof(float)* dim * dim);
    if (mat == NULL){printf("Erro no malloc\n"); return 2;}

    vet =  (float*) malloc(sizeof(float)* dim);
    if (vet == NULL){printf("Erro no malloc\n"); return 2;}

    saida = (float*) malloc(sizeof(float)* dim);
    if (saida == NULL){printf("Erro no malloc\n"); return 2;}

    //inicialização
    double startInicializacao,finishInicializacao;
    GET_TIME(startInicializacao);
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            mat[i*dim+j]=1;
        }
        vet[i]=1;
        saida[i]=0;
    }
    GET_TIME(finishInicializacao);
    double timeInicializacao = finishInicializacao - startInicializacao;

    //multiplicaçao
        //alocação
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nThread);
    if (tid == NULL){printf("Erro no malloc\n"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nThread);
    if (args == NULL){printf("Erro no malloc\n"); return 2;}

        //criação das threads
    int intervalo = dim/nThread;

    double startMultiplicacao, finishMultiplicacao;

    GET_TIME(startMultiplicacao);
    for (int i = 0; i < nThread; i++){
        (args+i)->dim = dim;
        (args+i)->indexInicial = i*intervalo;
        (args+i)->indexFinal = (i+1)*intervalo;        
        if(pthread_create(tid+i,NULL,tarefa, (void*) (args+i))){
            puts("erro no pthread create");return 3;
        }
    }
        //termino das threads
    for (int i = 0; i < nThread; i++){
        pthread_join(*(tid+i),NULL);
    }
    GET_TIME(finishMultiplicacao);

    double timeMultiplicacao = finishMultiplicacao - startMultiplicacao;
    


    //exibição
    /*puts("Matriz de entrada:");
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            printf("%.lf ",mat[i*dim+j]);
        }
        puts(""); 
    }

    puts("Vetor de entrada:");
    for (int i = 0; i < dim; i++){
        printf("%.lf ",vet[i]);
    }
    puts("");*/
    puts("Vetor de saida:");
    for (int i = 0; i < dim; i++){
        printf("%.lf ",saida[i]);
    }
    puts(""); 

    printf("Tempo na Inicialização: %.lf nanossegundos\n",timeInicializacao);
    printf("Tempo na Multiplicação: %.lf nanossegundos\n",timeMultiplicacao);
    printf("Tempo Total: %.lf nanossegundos\n",timeInicializacao+timeMultiplicacao);

    free(mat);
    free(vet);
    free(saida);
    free(args);
    free(tid);

    return 0;
}
