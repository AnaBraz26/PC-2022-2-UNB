#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define PR 10//númerodeprodutores
#define CN 6//númerodeconsumidores
#define N 5 //tamanhodobuffer


void *produtor(void *meuid);
void *consumidor (void *meuid);
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
sem_t prod;
sem_t cons;

int buffer[N];
int inserir=0;
int remover=0;
int contador=0;
int qtd_cons=0;

void main(argc,argv)
int argc;
char *argv[];
{

int erro;
int i,n,m;
int *id;

sem_init(&prod,0,3);
sem_init(&cons,0,3);

pthread_t tid[PR];

for (i=0;i<PR;i++)
{
    id=(int *)malloc(sizeof(int));
    *id=i;
    erro=pthread_create(&tid[i],NULL,produtor,(void *)(id));

    if(erro)
    {
        printf("erro na criacao do thread%d\n",i);
        exit(1);
    }
}

    pthread_t tCid[CN];

for (i=0;i<CN;i++)
{
    id=(int *)malloc(sizeof(int));
    *id=i;
    erro=pthread_create(&tCid[i],NULL,consumidor,(void *)(id));

    if(erro)
    {
        printf("erro na criacao do thread %d\n",i);
        exit(1);
    }
 }

pthread_join(tid[0],NULL);

}

void *produtor (void*pi)
{
    while(1){
    printf("produtor %d: vou produzir um item \n", *(int *)(pi));
    int item = (int) (rand() * 1000.0);
    sleep(10);

    int produtores_Ativos = 0;

    sem_wait(&prod);
    pthread_mutex_lock(&mutex);
	    // while(contador == N) {
	    //   printf("produtor %d: vou esperar espaço no buffer \n", *(int *)(pi));
	    //   sem_wait(&prod);
	    // }
	    printf("produtor %d: vou inserir item %d na posição %d\n", *(int *)(pi),item,inserir);
	    buffer[inserir] = item;
	    inserir++;
	    if(inserir >= N){
		inserir = 0;
	    }
	    contador++;
	    if(contador <= CN){
	       sem_post(&cons);
	    }
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(0);

}

void *consumidor (void*pi)
{
 while(1)  {
    sem_wait(&cons);
    pthread_mutex_lock(&mutex);
	    // while(contador == 0) {
	    //   printf("consumidor %d: vou esperar por itens \n", *(int *)(pi));
	     
	    // }
	    
	    int lido = buffer[remover];
	    printf("consumidor %d: vou remover item %d na posição %d\n", *(int *)(pi),lido,remover);
	    remover++;
	    if(remover >= N){
		remover = 0;
	    }
	    contador--;
	    if(contador >= (N-PR)){
	      sem_post(&prod);
	    }
   pthread_mutex_unlock(&mutex); 
   printf("consumidor %d: vou consumir item %d \n", *(int *)(pi),lido);
   sleep(1);

 }
  pthread_exit(0);

}
