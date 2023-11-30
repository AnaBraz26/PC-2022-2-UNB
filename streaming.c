// Aluna Ana Caroline da Rocha Braz - 212008482
// Sistema de Streaming com threads

//----------------------------------------------------------------
//Bibliotecas
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

//----------------------------------------------------------------
// Definições
#define MAX_USUARIOS 5
#define MAX_SESSOES 2

//----------------------------------------------------------------
// Declaração de mutex, semáforos e barreiras

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t turno = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int sessoes_em_uso = 0;
//-----------------------------------------------------------------
// Função dos usuarios
void *thread_usuario(void *arg) {
    int id_usuario = *((int *)arg);

    while (1) {
        // Inicia sessão do usuário
        pthread_mutex_lock(&turno);
        pthread_mutex_lock(&mutex);

        while (sessoes_em_uso >= MAX_SESSOES) {
            // Aguarda disponibilidade de sessão
            pthread_cond_wait(&cond, &mutex);
        }

        sessoes_em_uso++;
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&turno);

        // Realiza streaming de vídeo e áudio simultaneamente
        printf("Usuario %d esta fazendo streaming de video e audio...\n", id_usuario);
        sleep(5);  // Simula o streaming

        // Encerra sessão do usuário
        pthread_mutex_lock(&mutex);
        sessoes_em_uso--;
        pthread_cond_signal(&cond);  // Libera uma sessão para outros usuários
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

//----------------------------------------------------------------
// Função para Manutenção do sistema
void *thread_administrador(void *arg) {
    while (1) {
        // Realiza operações administrativas exclusivas
        pthread_mutex_lock(&turno);
        printf("O administrador esta trabalhando...\n");
        sleep(2);
        pthread_mutex_unlock(&turno);
    }

    return NULL;
}

//----------------------------------------------------------------------------
// Main
int main() {
    pthread_t tid_administrador;
    pthread_t tid_usuario[MAX_USUARIOS];
    int ids_usuarios[MAX_USUARIOS];

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