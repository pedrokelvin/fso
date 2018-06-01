#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#define RAND 1

int totalThreads = 5;
int countThreads = 5;
sem_t sem_aluno;
sem_t sem_monitor;

typedef struct thread{
  int count;
  long unsigned id;
}Thread;

void* monitor(void *pt) {
  while(countThreads > 1)
  {
    sem_wait(&sem_aluno);
    sleep(4);
    sem_post(&sem_monitor);
    --countThreads;
  }
}

void* emAtendimento(void *pt){
  Thread *pt_aux = (Thread *) pt;
  // int a;
  // sem_getvalue(&sem_monitor, &a);
  // printf("MONITOR ATENDE! %d\n", a);

  sem_wait(&sem_monitor);
  printf("ALUNO FOI ATENDIDO %lu\n", pt_aux->id);
  sem_post(&sem_aluno);
  int a;
  sem_getvalue(&sem_aluno, &a);
  sleep(4);

  // pt_aux->count++;
  //
  // if(pt_aux->count == 3)
  // {
  //   pthread_cancel(pt_aux->id);
  // }
}

void comportamentoAluno(void* pt){
  while(countThreads > 1){
    int r = rand() % 2;

    switch(r){
      case 0:
        sleep(3);
        // ficar em casa
      break;

      case 1:
        //ir ao monitor
        //lock mutex
        emAtendimento(pt);
        // unlock mutex
      break;
    }
  }
}

int main(){
  sem_init(&sem_aluno, 0, 0);
  sem_init(&sem_monitor, 0, 1);

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
  	pthread_create(&tids[i], &attr_i, emAtendimento, &pt[i]);
  }

  for (int i = totalThreads-1; i >= 0; i--) {
    pthread_join(tids[i], NULL);
    printf("ACABOU %i\n", i);
  }


  return 0;
}
