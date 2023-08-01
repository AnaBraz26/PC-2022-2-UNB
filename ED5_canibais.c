#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXCANIBAIS 20

pthread_cond_t stt_cozinheiro = PTHREAD_COND_INITIALIZER; 
pthread_cond_t stt_canibal = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

int comida = 0;

void *cozinheiro(int m);
void *canibal(void*meuid);


void main(argc, argv)
int argc;
char *argv[];
{
  int erro;
  int i, n, m;
  int *id;

  pthread_t tid[MAXCANIBAIS];

  if(argc != 3){
    printf("erro na chamada do programa: jantar <#canibais> <#comida>\n");
    exit(1);
  }
  
  n = atoi (argv[1]); //número de canibais
  m = atoi (argv[2]); // quantidade de porções que o cozinheiro consegue preparar por vez
  printf("numero de canibais: %d -- quantidade de comida: %d\n", n, m);

  if(n > MAXCANIBAIS){
    printf("o numero de canibais e' maior que o maximo permitido: %d\n", MAXCANIBAIS);
    exit(1);
  }
  
  for (i = 0; i < n; i++)  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tid[i], NULL, canibal, (void *) (id));

    if(erro){
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  cozinheiro(m);
}

void *cozinheiro (int m){
 
  while(1){
    pthread_mutex_lock(&mutex);

    //dormir enquanto tiver comida
    while(comida > 0){
      printf("Colocando cozinheiro pra dormir\n");
      pthread_cond_wait(&stt_cozinheiro, &mutex);
      printf("Acordando cozinheiro\n");
    } 

    comida = m;
    printf("cozinheiro: vou cozinhar comida %d\n", comida);    
    sleep(2);

    //acordar os canibais   
    pthread_cond_broadcast(&stt_canibal);
    

    pthread_mutex_unlock(&mutex);
   }

}

void * canibal (void* pi){
  
  while(1) {
    //pegar uma porção de comida e acordar o cozinheiro se as porções acabaram
    pthread_mutex_lock(&mutex);

    //dormir enquanto tiver comida
    while(comida == 0){
      printf("Colocando canibal pra dormir\n");
      pthread_cond_wait(&stt_canibal, &mutex);
      printf("Acordando canibais\n");
    }    
    
    comida--;

    //acordar os canibais   
    if(comida == 0){
      pthread_cond_signal(&stt_cozinheiro);
    }

    pthread_mutex_unlock(&mutex);
    printf("%d: vou comer a porcao que peguei\n", *(int *)(pi));
    sleep(1);

  }
  
}




