#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>


//struct dos argumentos da thread
typedef struct {
   int v0, numThreads, mv;
   int* vetor;
} thread_arg;

//funcao executada pelas threads
void *PrintHello (void *arg) {
  thread_arg *args = (thread_arg *) arg;

  for (int i = args->v0; i < args->mv; i+=args->numThreads){
  	args->vetor[i]++;
  }

  free(arg); //aqui pode liberar a alocacao feita na main

  pthread_exit(NULL);
}

//funcao principal do programa
int main() {
  pthread_t tid_sistema[2]; //terão apenas 2 threads então 2 ids
  int thread; //variavel auxiliar
  int vetor[100];

  for (int i = 0; i < 100; ++i){
  	vetor[i] = i;
  }
  thread_arg *arg; //receberá os argumentos para a thread
  for(int i=0; i<2; i++) {

    //preparando os argumentos para as threads
    arg = malloc(sizeof(thread_arg));
    if (arg == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    arg->vetor = vetor;
    arg->v0 = i; 
    arg->numThreads = 2; 
    arg->mv = 100;

    //criando as threads
    if (pthread_create(&tid_sistema[i], NULL, PrintHello, (void*) arg)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  for (int i=0; i<2; i++) {
    if (pthread_join(tid_sistema[i], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }

  //printa os elementos do vetor após a alteração.
  for (int i = 0; i < 100; ++i)
  {
  	printf("Vetor[%d] = %d\n", i, vetor[i]);
  }
}