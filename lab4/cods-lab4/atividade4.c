#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4

int thread1lock=0, thread2lock=0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

void* A1(void* arg){
    printf("A: começando\n");
    pthread_mutex_lock(&x_mutex);

    printf("tudo bem?\n");
    thread1lock=1;
    if(thread1lock && thread2lock) pthread_cond_broadcast(&x_cond);

    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}
void* B2(void* arg){
    printf("B: começando\n");
    pthread_mutex_lock(&x_mutex);

    printf("bom dia!\n");
    thread2lock=1;
    if(thread1lock && thread2lock) pthread_cond_broadcast(&x_cond);

    
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}
void* C3(void* arg){
    printf("C: começando\n");
    pthread_mutex_lock(&x_mutex);

    if(!thread1lock || !thread2lock) pthread_cond_wait(&x_cond, &x_mutex);

    printf("ate mais!\n");
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}
void* D4(void* arg){
    printf("D: começando\n");
    pthread_mutex_lock(&x_mutex);

    if(!thread1lock || !thread2lock) pthread_cond_wait(&x_cond, &x_mutex);

    printf("boa tarde!\n");
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}


int main(int argc, char const *argv[]){ 
    pthread_t threads[NTHREADS];

    pthread_mutex_init(&x_mutex, NULL);

    pthread_cond_init(&x_cond, NULL);

    pthread_create(&threads[1], NULL, B2, NULL);
    pthread_create(&threads[0], NULL, A1, NULL);
    pthread_create(&threads[3], NULL, D4, NULL);
    pthread_create(&threads[2], NULL, C3, NULL);
    

    /* Espera todas as threads completarem */
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("\nFIM\n");

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    return 0;
}
