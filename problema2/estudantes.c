#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <math.h>

#define RAND 1

int totalThreads = 10;
int countThreads = 10;
sem_t sem_aluno;
sem_t sem_monitor;
sem_t sem_aviso;
sem_t sem_filaEspera;
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
    printf("COUNT = %d\n", countThreads);
    sem_wait(&sem_aluno);
    sleep(1);
    sem_post(&sem_monitor);
    // --countThreads;
    sem_wait(&sem_aviso);
  }
}

void* emAtendimento(void *pt){
  Thread *pt_aux = (Thread *) pt;

  sem_wait(&sem_monitor);
  sleep(2);
  printf("O ALUNO %lu FOI ATENDIDO\n", pt_aux->id);
  sem_post(&sem_aluno);
  if(pt_aux->filaEspera == 1)
  {
    sem_post(&sem_filaEspera);
    pt_aux->filaEspera = 0;
  }

  pt_aux->count++;
  if(pt_aux->count == 3)
    --countThreads;

  sem_post(&sem_aviso);
}

void* comportamentoAluno(void* pt){
  Thread *pt_aux = (Thread *) pt;

  while(pt_aux->count < 3){
    int r = rand() % 2;
    int total;
    // printf("ESCOLHA: %d\n", r);

    switch(r){
      case 0:
        printf("O ALUNO %lu FOI DORMIR\n", pt_aux->id);
        sleep(1);
        // ficar em casa
      break;

      case 1:

        sem_getvalue(&sem_filaEspera, &total);
        printf("TOTAL = %d\n", total);
        pthread_mutex_lock(&mutex_filaEspera);
        if(total > 0 && total < (int)ceil(totalThreads/2.0))
        {
          sem_wait(&sem_filaEspera);
          printf("O ALUNO %lu ENTROU NA FILA DE ESPERA\n", pt_aux->id);
          pt_aux->filaEspera = 1;
        }
        else if(total <= 0)
        {
          break;
        }
        pthread_mutex_unlock(&mutex_filaEspera);

        pthread_mutex_lock(&mutex);
        emAtendimento(pt);
        pthread_mutex_unlock(&mutex);
        // unlock mutex
      break;
    }
  }
}

int main(){
  sem_init(&sem_aluno, 0, 0);
  sem_init(&sem_monitor, 0, 1);
  sem_init(&sem_aviso, 0, 0);
  sem_init(&sem_filaEspera, 0, (int)ceil(totalThreads/2.0));
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
  }

  for (int i = totalThreads-1; i >= 0; i--) {
    pthread_join(tids[i], NULL);
    printf("ACABOU %i\n", i);
  }


  return 0;
}
