// ANA CAROLINE BRAZ 212008482

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5
#define ESQ(id) (id+N-1)%N
#define DIR(id)	(id+1)%N
#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2

sem_t mutex;
sem_t s[N];
int estado[10];

void *filosofos (void *arg);
void pega_talher (int n);
void devolve_talher (int n);
void mostrar();

int main () {
	int i;
	int *id;
	//semaforo 
	sem_init(&mutex, 0, 1);

	for (i=0; i<N; i++) {
		sem_init(&s[i], 0, 1);
	}
	pthread_t r[N]; 

	//criacao das threads de filosofos
       for (i = 0; i < N ; i++) {
	     id = (int *) malloc(sizeof(int));
	     *id = i;
	     pthread_create(&r[i], NULL, filosofos, (void*) (id));
	}

        pthread_join(r[0],NULL);
	return 0;
}

void mostrar(){
   for(int i=0; i<N; i++){
   	if(estado[i] == PENSANDO){
      	printf("O filosofo %d esta pensando...\n", i);
   	}
   	if(estado[i] == FAMINTO){
      	printf("O filosofo %d esta com fome...\n", i);
   	}
   	if(estado[i] == COMENDO){
      	printf("O filosofo %d esta comendo!\n", i);
   	}
   }
   printf("\n");
}

void *filosofos (void *arg) {
	int n = *((int *) arg);
	while(1) {
		//pensar
		//printf("Filosofo %d pensando ...\n", n);
		sleep(3);
		pega_talher(n);
		//printf("Filosofo %d esta com fome\n", n);
		//comer
		//printf("Filosofo %d comendo ...\n", n);
		sleep(3);
        //printf("Filosofo %d acabou de comer...\n\n", n);
		devolve_talher(n);
	} 
}

void pega_talher (int n) {

	sem_wait(&mutex);
	estado[n] = FAMINTO;
	mostrar();

	if(estado[n] == FAMINTO && estado[ESQ(n)] != COMENDO && estado[DIR(n)] != COMENDO){
		estado[n] = COMENDO;
		mostrar();
		sem_post(&s[n]);
	}

	sem_post(&mutex);
	sem_wait(&s[n]);
}

void devolve_talher (int n) {
	sem_wait(&mutex);
	estado[n] = PENSANDO;
	mostrar();
	int esq = ESQ(n);
	int dir = DIR(n);

	if(estado[esq] == FAMINTO && estado[ESQ(esq)] != COMENDO && estado[DIR(esq)] != COMENDO){
		estado[esq] = COMENDO;
		mostrar();
		sem_post(&s[esq]);
	}

	if(estado[dir] == FAMINTO && estado[ESQ(dir)] != COMENDO && estado[DIR(dir)] != COMENDO){
		estado[dir] = COMENDO;
		mostrar();
		sem_post(&s[dir]);
	}

	sem_post(&mutex);
}

