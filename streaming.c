/// Aluna Ana Caroline da Rocha Braz - 212008482
// Sistema de Streaming com threads

//----------------------------------------------------------------
//Bibliotecas
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
// --------------------------------------------------------------
// Cores no terminal
#define VERMELHO "\033[1;31m"                                 
#define VERDE "\033[32m"
#define AMARELO "\033[33m"
#define RESET "\033[0;0m"

//----------------------------------------------------------------
// Definições
#define MAX_USUARIOS 5
#define MAX_SESSOES 2

//----------------------------------------------------------------
// Declaração de mutex, semáforos e barreiras

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t turno = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
sem_t tela;
sem_t admin;
int sessoes_em_uso = 0;
int atualizacao = 0;

//-----------------------------------------------------------------
// Função dos usuarios e streaming
void *thread_usuario(void *arg) {
    int id_usuario = *((int *)arg);

    while (1) {
        // Inicia sessão do usuário
        sem_wait(&tela);
        pthread_mutex_lock(&mutex);

        while (sessoes_em_uso >= MAX_SESSOES) {
            // Aguarda disponibilidade de sessão
            pthread_cond_wait(&cond, &mutex);
        }

        sessoes_em_uso++;
        pthread_mutex_unlock(&mutex);

            // Realiza streaming de vídeo e áudio simultaneamente
            printf(VERDE ">> Usuario %d esta fazendo streaming de video e audio... << \n" RESET, id_usuario);
            sleep(2);  // Simula o streaming
            atualizacao++;

        // Encerra sessão do usuário
        pthread_mutex_lock(&mutex);
        sessoes_em_uso--;
        printf("Atualizacao daqui a %d streaming\n", atualizacao);
        pthread_cond_signal(&cond);  // Libera uma sessão para outros usuários
        printf(VERMELHO "<< Streaming  do usuartio %d finalizada >>\n" RESET, id_usuario);
        if(atualizacao == 10){
            sem_post(&admin);
        }
        pthread_mutex_unlock(&mutex);        
        sem_post(&tela);
    }

     pthread_exit(0);
}

//----------------------------------------------------------------
// Função para Manutenção do sistema
void *thread_administrador(void *arg) {
    while (1) {
        // Realiza operações administrativas exclusivas
        sem_wait(&admin);
        pthread_mutex_lock(&mutex);
        printf(AMARELO "\nO administrador esta realizando a atualizacao do sistema...\n" RESET);
        sleep(5);
        printf(AMARELO "Atualizacao do sistema realizada com sucesso!\n\n" RESET);
        atualizacao = 0;
        pthread_mutex_unlock(&mutex);
        sem_post(&tela);
    }

     pthread_exit(0);
}

//----------------------------------------------------------------------------
// Main
int main() {
    pthread_t tid_administrador;
    pthread_t tid_usuario[MAX_USUARIOS];
    int ids_usuarios[MAX_USUARIOS];

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&turno, NULL);

    sem_init(&tela, 0, MAX_SESSOES);
    sem_init(&admin, 0, 0);

    // Inicia a thread do administrador
    pthread_create(&tid_administrador, NULL, thread_administrador, NULL);

    // Inicia as threads dos usuários
    for (int i = 0; i < MAX_USUARIOS; i++) {
        ids_usuarios[i] = i + 1;
        pthread_create(&tid_usuario[i], NULL, thread_usuario, &ids_usuarios[i]);
    }

    // Aguarda as threads terminarem (nunca deve acontecer no exemplo)
    pthread_join(tid_administrador, NULL);
    for (int i = 0; i < MAX_USUARIOS; i++) {
        pthread_join(tid_usuario[i], NULL);
    }

    return 0;
}
