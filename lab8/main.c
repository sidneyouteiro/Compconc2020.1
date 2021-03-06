#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

sem_t semaforo_em, escrita, prioridade;
int t_leitores=0,t_escritores=0, tot_threads,tam_arr;

typedef struct {
    int tid;
    int* arr;
} threadAgs;


void *Leitor(void *args){
    while(1) {
        threadAgs *arg = (threadAgs*) args;
        sem_wait(&semaforo_em);
        while(t_escritores>0) {
            sem_post(&semaforo_em);
            sem_wait(&prioridade);
            sem_post(&prioridade);
            sem_wait(&semaforo_em);
        }
        t_leitores += 1;
        if(t_leitores == 1){sem_wait(&escrita);}
        sem_post(&semaforo_em);
        
        printf("Thread Leitor %d# iniciando",arg->tid);

        for (register int i = t_escritores; i < tot_threads; i++){printf("array[%d] = %d", i,*(arg->arr + i));}
        printf("\n");
        printf("Thread Leitor %d# finalizando",arg->tid);

        t_leitores -= 1;
        if(t_leitores == 0){sem_post(&escrita);}
        sem_post(&semaforo_em);
    }    
}

void *Escritor(void *args){
    while(1) {
        threadAgs *arg = (threadAgs*) args;
        sem_wait(&semaforo_em);
        t_escritores += 1;        
        if(t_escritores == 1){sem_wait(&prioridade);}
        sem_post(&semaforo_em);
        sem_wait(&escrita);
        
        printf("Thread Leitor %d# iniciando",arg->tid);
        for(register int i = 0; i < t_escritores; i++){*(arg->arr + i) = arg->tid;}
        printf("Thread Leitor %d# finalizando",arg->tid);
        
        sem_post(&escrita);
        sem_wait(&semaforo_em);
        t_escritores -= 1;
        if(t_escritores == 0){sem_post(&prioridade);}
        sem_post(&semaforo_em);
    }   
}

int main(int argc, char const *argv[])
{
    if(argc < 3){printf("Execute o programa usando o seguinte formato:\n%s <Quantidade de Leitores> <Quantidade de Escritores> <Tamanho do Array>",argv[0]);return -1;}
    //inicializar os semaforos
    sem_init(&semaforo_em,0,1);
    sem_init(&escrita,0,1);
    sem_init(&prioridade,0,1);
    
    //recebendo argumentos
    t_leitores = atoi(argv[1]);
    t_escritores = atoi(argv[2]);
    tam_arr = atoi(argv[3]);
    tot_threads = t_leitores + t_escritores;
    pthread_t *threads = (pthread_t*) malloc((tot_threads)*sizeof(pthread_t));

    int *arr = (int*)malloc(tam_arr*sizeof(int));

    threadAgs* t_args = (threadAgs*)malloc(tot_threads*sizeof(threadAgs));

    //inicializando escritores
    for(register int i = 0; i < t_escritores; i++){
        t_args[i].tid=i;
        t_args[i].arr=arr;
    }
    //inicializando leitores
    for(register int i=t_escritores;i<tot_threads; i++){
        t_args[i].tid=i;
        t_args[i].arr=arr;
    }

    for(register int i=0;i<t_escritores;i++) {
        if(pthread_create(&threads[i],NULL,Escritor,(void*)(&t_args[i]))){printf("erro no pthread_create()\n");return -2;}
    }

    for(register int i=t_escritores;i<tot_threads;i++) {
        if(pthread_create(&threads[i],NULL,Leitor,(void*)(&t_args[i]))){printf("erro no pthread_create()\n");return -2;}
    }

    for(register int i=0;i<tot_threads;i++) {
        if(pthread_join(threads[i],NULL)){printf("erro no pthread_join");return -3;}
    }    

    return 0;
}
