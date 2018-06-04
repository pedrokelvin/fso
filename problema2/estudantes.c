#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#define totalThreads 4
#define maxAlunos totalThreads / 2
#define DELAY 1

//Global variables
int countThreads = totalThreads;
sem_t sem_aluno;
sem_t sem_monitor;
sem_t sem_filaEspera;
sem_t sem_sendoAtendido;
pthread_mutex_t mutex;
pthread_mutex_t mutex_filaEspera;

typedef struct thread{
  int count;
  long unsigned id;
  int filaEspera;
}Thread;

void* monitor(void *pt) {
  while(countThreads > 1)
  {
    printf("THREADS: %d\n", countThreads);
    int qntAluno;
    int sendoAtendido;
    int requisicaoMonitor;
    sem_getvalue(&sem_sendoAtendido, &sendoAtendido);
    sem_getvalue(&sem_filaEspera, &qntAluno);
    sem_getvalue(&sem_monitor, &requisicaoMonitor);
    if(requisicaoMonitor == 1) {
      printf("Monitor dormindo\n");
      sleep(DELAY);
    }
    else {
      printf("Monitor atendendo\n");
      sem_wait(&sem_aluno);
      sem_post(&sem_monitor);
      sleep(DELAY);
    }
  }
}

void* comportamentoAluno(void* pt){
  Thread *pt_aux = (Thread *) pt;

  while(pt_aux->count < 3){
    int total;
    int sendoAtendido;
    // printf("ESCOLHA: %d\n", r);

    switch(rand()%2){
      case 0:
        printf("O aluno %lu foi dormir\n", pt_aux->id);
        sleep(DELAY);
        // ficar em casa
      break;

      case 1:
        printf("ENTROU %lu\n", pt_aux->id);
        pthread_mutex_lock(&mutex_filaEspera);
        sem_getvalue(&sem_filaEspera, &total);
        sem_getvalue(&sem_sendoAtendido, &sendoAtendido);
        // printf("Número de vagas na fila de espera = %d\n", total);


        if(sendoAtendido == 0 && total > 0)
        {
          printf("O aluno %lu entrou na fila de espera, na posição %d\n", pt_aux->id, (maxAlunos - total)+1);
          sem_wait(&sem_filaEspera);
          pt_aux->filaEspera = 1;
        }

        else if(total <= 0)
        {
          pthread_mutex_unlock(&mutex_filaEspera);
          sleep(DELAY);
          break;
        }
        pthread_mutex_unlock(&mutex_filaEspera);


        pthread_mutex_lock(&mutex);

        printf("VALOR DO SENDOATENDIDO = %d\n", sendoAtendido);
        sem_wait(&sem_sendoAtendido);


        sem_wait(&sem_monitor);
        printf("O aluno %lu está sendo atendido\n", pt_aux->id);
        sleep(DELAY);
        if(pt_aux->filaEspera == 1)
        {
          sem_post(&sem_filaEspera);
          pt_aux->filaEspera = 0;
        }

        pt_aux->count++;
        if(pt_aux->count == 3)
          --countThreads;

        sem_post(&sem_aluno);
        sem_post(&sem_sendoAtendido);

        pthread_mutex_unlock(&mutex);
        break;
    }
  }
}

void ctrlc(int signal)
{
  printf("CTRL+C recebido");
  // for (int i = 0; i < totalThreads; i++) {
  // }
  exit(0);
}

int main(){
  signal(SIGINT, ctrlc);
  srand(time(NULL));
  sem_init(&sem_aluno, 0, 0);
  sem_init(&sem_monitor, 0, 1);
  sem_init(&sem_filaEspera, 0, maxAlunos);
  sem_init(&sem_sendoAtendido, 0, 1);
  pthread_mutex_init(&mutex,NULL);
  pthread_mutex_init(&mutex_filaEspera,NULL);

  Thread pt[totalThreads];
	pthread_t tids[totalThreads];

  // Criação da thread do monitor
  pthread_attr_t attr;
  pthread_attr_init(&attr);
	pthread_create(&tids[0], &attr, monitor, NULL);

  for(int i = 1; i < totalThreads; ++i){
    pthread_attr_t attr_i;
    pthread_attr_init(&attr_i);
    pt[i].count = 0;
    pt[i].id = i;
  	pthread_create(&tids[i], &attr_i, comportamentoAluno, &pt[i]);
    // pthread_cancel(&pt[i]);
  }

  for (int i = totalThreads-1; i >= 0; i--) {
    pthread_join(tids[i], NULL);
  }

  //Signal do ctrl + c para matar threads

  return 0;
}
