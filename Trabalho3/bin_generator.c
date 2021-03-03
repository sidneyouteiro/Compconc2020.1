#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  char file_name[50];
  if(argc<3){printf("2 argumentos");return -1;}
  // entrada do programa é o tamanho e o nome do arquivo a ser criado
  long long int tam = atoll(argv[1]);
  strcpy(file_name, argv[2]);
  srand(42);
  FILE *file = fopen(file_name, "wb");
  int num[tam];
  
  for(register long long int i=0;i<tam;i++){
      num[i]=rand()%6;
      printf("num[%llu] = %d\n",i,num[i]);
  }

  fwrite(&tam,sizeof(long long int),1,file);

  return 0;
}