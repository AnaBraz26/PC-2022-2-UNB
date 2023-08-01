// Ana Caroline da Rocha Braz 212008482

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10 //número de usuários
#define CARTAS 20 //quantidade de cartas na mochila

sem_t pombo;
sem_t user;
sem_t mutex;
int cartas = 0;

void * f_usuario(void *arg);
void * f_pombo(void *arg);

int main(int argc, char **argv){
    int i;
    pthread_t usuario[N];
    int *id;

    sem_init(&pombo, 0, 1);
    sem_init(&user, 0, 10);
    sem_init(&mutex, 0, 1);

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

void * f_pombo(void *arg){
  
    while(1){
        //Inicialmente está em A, aguardar/dorme a mochila ficar cheia (20 cartas)
        sem_wait(&pombo);
        sem_wait(&mutex);
        //Leva as cartas para B e volta para A        
        printf("Levando cartas\n");
        sleep(5);
        cartas = 0;
        printf("Pombo de volta!\n");
        //Acordar os usuários  
        for(int i = 0; i < CARTAS; i++){
            sem_post(&user); 
        }     
        sem_post(&mutex);       
    }
}

void * f_usuario(void *arg){
    int id = *((int *) arg);
    while(1){
	    //Escreve uma carta
        sleep(2);
        //Caso o pombo não esteja em A ou a mochila estiver cheia, então dorme	
        sem_wait(&user);
        sem_wait(&mutex);
        //Posta sua carta na mochila do pombo
        cartas++;
        printf("Usuario %d, coloquei a carta número %d \n",id,cartas);
        //Caso a mochila fique cheia, acorda o ṕombo
        if(cartas == CARTAS){
            sem_post(&pombo);
        }
        sem_post(&mutex)
;    }
}
