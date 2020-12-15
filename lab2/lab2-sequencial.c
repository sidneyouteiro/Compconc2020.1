#include <stdio.h>
#include <stdlib.h> 

float *mat;
#include <sys/time.h>

float *vet;
float *saida;

#define BILLION 1000000000L

double get_time(){
    struct timespec time; 
    clock_gettime(1, &time); 
    return (BILLION* time.tv_sec) + time.tv_nsec;
}

int main(int argc, char const *argv[])
{
    
    double start, finish;
    if (argc<2){
        printf("Digite: %s <dimensao da matriz>\n",argv[0]);
        return 1;
    }
    start = get_time();
    int dim = atoi(argv[1]);
    
    //alocação
    mat = (float*) malloc(sizeof(float)* dim * dim);
    if (mat == NULL){printf("Erro no malloc\n"); return 2;}

    vet =  (float*) malloc(sizeof(float)* dim);
    if (vet == NULL){printf("Erro no malloc\n"); return 2;}

    saida = (float*) malloc(sizeof(float)* dim);
    if (saida == NULL){printf("Erro no malloc\n"); return 2;}

    //inicialização

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            mat[i*dim+j]=1;
        }
        vet[i]=1;
        saida[i]=0;
    }
    //multiplicaçao
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            saida[i]+= mat[i*(dim)+j] * vet[j];       
    
    

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


    free(mat);
    free(vet);
    free(saida);

    finish = get_time();
    printf("Tempo Total: %.lf nanossegundos\n",finish-start);
    return 0;
}
