#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MA 10 //macacos que andam de A para B
#define MB 10 //macacos que andam de B para A

pthread_mutex_t lock_ab = PTHREAD_MUTEX_INITIALIZER; //Locks para macacos A --> B
pthread_mutex_t lock_ba = PTHREAD_MUTEX_INITIALIZER; //Locks para macacos B --> A
pthread_mutex_t lock_gorila = PTHREAD_MUTEX_INITIALIZER; //Locks para macacos B --> A
pthread_mutex_t on = PTHREAD_MUTEX_INITIALIZER; // chave 

int macacos_ab = 0; // Contador para macacos A --> B
int macacos_ba = 0; // Contador para macacos B --> A
int gorila_passando = 0;


void * macacoAB(void * a) {
    int i = *((int *) a);    
    while(1){
      //Procedimentos para acessar a corda
      pthread_mutex_lock(&on);
      pthread_mutex_lock(&lock_ab);
      macacos_ab++;

      if(macacos_ab == 1){
        pthread_mutex_lock(&lock_ba);
        pthread_mutex_lock(&lock_gorila);
      }

      pthread_mutex_unlock(&lock_ab);
      pthread_mutex_unlock(&on);

      printf("Macaco %d passando de A para B\n",i, macacos_ab);
      sleep(5 * macacos_ab);
	    
      //Procedimentos para quando sair da corda
      pthread_mutex_lock(&lock_ab);
      macacos_ab--;
      if(macacos_ab == 0){
         pthread_mutex_unlock(&lock_ba);
         pthread_mutex_unlock(&lock_gorila);
         printf("\n");
      }
      pthread_mutex_unlock(&lock_ab);
    }
    pthread_exit(0);
}

void * macacoBA(void * a) {
    int i = *((int *) a);    
    while(1){
        //Procedimentos para acessar a corda
        pthread_mutex_lock(&on);
        pthread_mutex_lock(&lock_ba);
        macacos_ba++;

        if(macacos_ba == 1){
          pthread_mutex_lock(&lock_ab);
          pthread_mutex_lock(&lock_gorila);
        }

        pthread_mutex_unlock(&lock_ba);
        pthread_mutex_unlock(&on);

        printf("Macaco %d passando de B para A\n", i);
        sleep(5 * macacos_ba);
        
        //Procedimentos para quando sair da corda
        pthread_mutex_lock(&lock_ba);
        macacos_ba--;
        if(macacos_ba == 0){
          pthread_mutex_unlock(&lock_ab);
          pthread_mutex_unlock(&lock_gorila);
          printf("\n");
        }
        pthread_mutex_unlock(&lock_ba);
    }
    pthread_exit(0);
}


void * gorila(void * a){
    while(1){
        //Procedimentos para acessar a corda
        pthread_mutex_lock(&on);
        pthread_mutex_lock(&lock_gorila);
        gorila_passando++;

        if(gorila_passando == 1){
          pthread_mutex_lock(&lock_ab);
          pthread_mutex_lock(&lock_ba);
        }

        pthread_mutex_unlock(&lock_gorila);
        pthread_mutex_unlock(&on);

        printf("Gorila passando, Gorila = %d \n", gorila_passando);
        sleep(5);
        
        //Procedimentos para quando sair da corda
        pthread_mutex_lock(&lock_gorila);
        gorila_passando--;
        if(gorila_passando == 0){
          pthread_mutex_unlock(&lock_ab);
          pthread_mutex_unlock(&lock_ba);
          printf("\n");
        }
        pthread_mutex_unlock(&lock_gorila);
    }
    pthread_exit(0);
}

int main(int argc, char * argv[])
{
    pthread_t macacos[MA+MB];
    int *id;
    int i = 0;

    for(i = 0; i < MA+MB; i++){
        id = (int *) malloc(sizeof(int));
        *id = i;
        if(i%2 == 0){
          if(pthread_create(&macacos[i], NULL, &macacoAB, (void*)id)){
            printf("Não pode criar a thread %d\n", i);
            return -1;
          }
        }else{
          if(pthread_create(&macacos[i], NULL, &macacoBA, (void*)id)){
            printf("Não pode criar a thread %d\n", i);
            return -1;
          }
        }
    }
    pthread_t g;
    pthread_create(&g, NULL, &gorila, NULL);

  
    pthread_join(macacos[0], NULL);
    return 0;
}
