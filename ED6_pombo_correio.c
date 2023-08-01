// Aluna Ana Caroline da Rocha Braz 212008482

#include <stdlib.h> 
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "unistd.h"

#define N 10 //número de usuários

//#define CARTAS 20 //quantidade de cartas na mochila

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pombo = PTHREAD_COND_INITIALIZER;
pthread_cond_t user = PTHREAD_COND_INITIALIZER;
int cartas = 0;

void * f_usuario(void *arg);
void * f_pombo(void *arg);

int main(int argc, char **argv){
    int i;

    pthread_t usuario[N];
    int *id;
    for(i = 0; i < N; i++){
         id = (int *) malloc(sizeof(int));
          *id = i;
          pthread_create(&(usuario[i]),NULL,f_usuario,  (void *) (id));
    }
     pthread_t pombo;
     id = (int *) malloc(sizeof(int));
     *id = 0;
     pthread_create(&(pombo),NULL,f_pombo, (void*) (id));

     pthread_join(pombo,NULL);
   
}

void * f_usuario(void *arg){
   while(1) {
    //pegar uma porção de comida e acordar o cozinheiro se as porções acabaram
    pthread_mutex_lock(&mutex);

    //dormir enquanto tiver comida
    while(cartas == 20){
      printf("Colocando user pra dormir\n");
      pthread_cond_wait(&user, &mutex);
      //printf("Acordando user\n");
    }    
    
    cartas++;
    printf("Colocando Carta = %d\n", cartas);
     
    //acordar os canibais   
    if(cartas == 20){
        printf("Acordando pombo\n");
      pthread_cond_signal(&pombo);
    }

    pthread_mutex_unlock(&mutex);

  }
}

void * f_pombo(void *arg){
  
  while(1){
    pthread_mutex_lock(&mutex);

    //dormir enquanto não tive cartas
    while(cartas == 0){     
      printf("Colocando pombo pra dormir\n");
      pthread_cond_wait(&pombo, &mutex);      
    } 

    cartas = 0;
    printf("Entregando cartas\n");    
    sleep(2);

    //acordar os user 
   
    pthread_cond_broadcast(&user);    
    

    pthread_mutex_unlock(&mutex);
   }
}