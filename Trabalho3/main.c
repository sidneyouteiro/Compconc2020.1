#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include "timer.h"

#define NTHREADS 3

FILE* file;
char file_name[10] = "array.bin";
int NBloco,MBuffer,buffer_index;
long long int tam_array,count_array=0;
int* buffer,bufferAux;
int count_barrier=NTHREADS,aux_barrier=0;

int High_sequence_start=0, High_sequence_value=-1, High_sequence_size=0;


pthread_mutex_t mutex;

sem_t bufferLivre,bufferOcupado,exclusaoMutua;

void* read_file(void* arg);
void* T1(void* arg);
void* T2(void* arg);
void* T3(void* arg);


struct Triple_sequence_info{
    int triple_number;
    int triples_count;
    int current_count;
};
void* find_triples(void* arg);

struct Full_sequence_info{
    int index;
    bool in_sequence;
    int count;
};
void* find_full_sequences(void* arg);


struct High_sequence_info{
    int start;
    int value;
    int size;
} High_sequence_info;

void* find_highest_sequence(void* arg);

void barreira();

int main(int argc, char const *argv[])
{
    double time_begin,time_end,time_delta;
    if(argc<3){
        printf("Execute a aplicação da seguinte forma:\n");
        printf("%s <Tamanho do Bloco(N)> <Tamanho do Buffer(M)>");
        return -1;
    }
    NBloco = atoi(argv[1]);
    MBuffer = atoi(argv[2]);

    buffer = (int*)malloc(sizeof(int)*MBuffer*NBloco);
    if(buffer==NULL){printf("erro no malloc");return -5;}
    
    bufferAux = (int*)malloc(sizeof(int)*MBuffer);
    if(bufferAux==NULL){printf("erro no malloc");return -5;}
    

    file = fopen(file_name,"rb");
    if(file==NULL){printf("erro no fopen");return -4;}
    fread(&tam_array,sizeof(long long int),1,file);
    







    pthread_t threads[NTHREADS];
    GET_TIME(&time_begin);
    if(pthread_create(&threads[0],NULL,read_file,NULL)){printf("erro pthread_create 0");return -2;}
    if(pthread_create(&threads[1],NULL,T1,NULL)){printf("erro pthread_create 1");return -2;}
    if(pthread_create(&threads[2],NULL,T2,NULL)){printf("erro pthread_create 2");return -2;}    
    if(pthread_create(&threads[3],NULL,T3,NULL)){printf("erro pthread_create 3");return -2;}

    for(register int i=0;i<NTHREADS;i++){
        if(pthread_join(threads[i],NULL)){printf("erro no pthread_join %d",i);return -3;}
    }
    GET_TIME(&time_end);
    time_delta=time_end-time_begin;

    printf("Temp: %lf",time_delta);


    return 0;
}


void* read_file(void* arg){
    int index=0;

    long long int num_read,num_rest;

    int aux_buffer[MBuffer];

    while(count_array<tam_array){
        sem_wait(&bufferLivre);
        int count_read = fread(aux_buffer,sizeof(int),NBloco,file);
        *(buffer+(NBloco*index))=aux_buffer;
        sem_wait(&exclusaoMutua);
        buffer_index++;
        count_array+=count_read;
        sem_post(&exclusaoMutua);
        sem_post(&bufferOcupado);
        
    }
    fclose(file);
    pthread_exit(NULL);
}

void* T1(void* arg){

    long long int index=0;
    int bloco_index=0,buffer_index=0;
    

    int aux_High_sequence_start,aux_High_sequence_value=-1, aux_High_sequence_size=0;
    
    int num=-2;
    while(1){
        
        for (register int i = 0; i < MBuffer; i++){
            num = *(buffer+(bloco_index+NBloco)+i);
            if(aux_High_sequence_value == num){
                aux_High_sequence_size++;
            } else{
                if(aux_High_sequence_size>High_sequence_size){
                    High_sequence_start=aux_High_sequence_start; 
                    High_sequence_value=aux_High_sequence_value; 
                    High_sequence_size=aux_High_sequence_size;
                }
                aux_High_sequence_value=num;
                aux_High_sequence_size=0;
                aux_High_sequence_start=(bloco_index+NBloco)+i;
            }
        }
        bloco_index = (bloco_index+1)%NBloco;
        if(bloco_index == NBloco-1){
            sem_post(&bufferLivre);
            break
        }

    }
}
void* T2(void* arg){

}
void* T3(void* arg);