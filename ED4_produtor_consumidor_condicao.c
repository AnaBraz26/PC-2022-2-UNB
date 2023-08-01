//ALUNA ANA CAROLINE DA ROCHA BRAZ MATRÍCULA 212008482

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define PR 1 //número de produtores
#define CN 1 // número de consumidores
#define N 5  //tamanho do buffer

void * produtor(void * meuid);
void * consumidor (void * meuid);

pthread_cond_t stt_produtor = PTHREAD_COND_INITIALIZER; 
pthread_cond_t stt_consumidor = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t mutex_buff = PTHREAD_MUTEX_INITIALIZER; 

int count = 0;
int buffer[N];
int buff_livre = N;
int buff_ocupado = 0; 

int insert_item(int item){
  if(count < N){
    buffer[N] = item;
    count++;
    return 0;
  }
  else{
    return 1;
  }
}

int remove_item(int item){
  if(count > 0){
    item = buffer[(count-1)];
    count--;
    return 0;
  }
  else{
    return 1;
  }
}
void main(argc, argv)
int argc;
char *argv[];
{

  int erro;
  int i, n, m;
  int *id;

  pthread_t tid[PR];
   
  for (i = 0; i < PR; i++)
  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tid[i], NULL, produtor, (void *) (id));

    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  pthread_t tCid[CN];

  for (i = 0; i < CN; i++)
  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tCid[i], NULL, consumidor, (void *) (id));

    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }
 
  pthread_join(tid[0],NULL);

}

void * produtor (void* pi)
{
  int item;

  while(1)
  {
    item = rand() % 10;

    pthread_mutex_lock(&mutex);
    if(buff_ocupado == N){
      printf("Dorme produtor\n");
      pthread_cond_wait(&stt_produtor, &mutex);
      printf("Acordando Produtor\n\n");
    }

    pthread_mutex_unlock(&mutex);
    
    buff_livre--;
    buff_ocupado++;
    printf("Produzindo e inserindo item %d -- buff_ocupado = %d, buff_livre = %d\n", item,buff_ocupado,buff_livre);
    insert_item(item);
    sleep(2);

    pthread_mutex_lock(&mutex);
   
    if(buff_ocupado == 1){      
      pthread_cond_signal(&stt_consumidor);
    }        

    pthread_mutex_unlock(&mutex);   

  }
  pthread_exit(0);
  
}

void * consumidor (void* pi)
{ 
  int item;
 while(1)
  {
    item = rand() % 10;
    pthread_mutex_lock(&mutex);

    if(buff_ocupado == 0){      
      printf("Dorme consumidor\n");
      pthread_cond_wait(&stt_consumidor, &mutex);
      printf("Acordando Consumido\n");
    }
    pthread_mutex_unlock(&mutex);

    buff_livre++;
    buff_ocupado--;
    if(!remove_item(item)){
      printf("Consumindo e removendo item %d -- buff_ocupado = %d, buff_livre = %d\n\n", item,buff_ocupado,buff_livre);
    } 
    sleep(4);

    pthread_mutex_lock(&mutex);
   
    if(buff_livre == N-1){
      pthread_cond_signal(&stt_produtor);
    }        
    pthread_mutex_unlock(&mutex);   
    
  }
  pthread_exit(0);
  
}
