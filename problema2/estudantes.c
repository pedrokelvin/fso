#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define totalThreads 4
#define RAND_MAX 1

int countThreads = totalThreads;

typedef struct thread{
  int count = 0;
}Thread;

void emAtendimento(void *pt){

  Thread *pt_aux = (Thread *) pt;

  // lock mutex
  pt_aux->count++;

  if(pt_aux->count == 3)
    --countThreads;

  wait();
  // unlock mutex
}

void comportamentoAluno(void* pt){

  srand(time(NULL));

  while(countThreads > 0){

    int r = rand();

    switch(r){
      case 0:
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

  Thread pt[totalThreads];
	pthread_t tids[totalThreads];

  // Criação da thread do monitor
  pthread_attr_t attr;
  pthread_attr_init(&attr);
	pthread_create(&tids[0], &attr, emAtendimento, );

  for(int i = 1; i < totalThreads; ++i){
    pthread_attr_t attr;
    pthread_attr_init(&attr);
  	pthread_create(&tids[i], &attr, comportamentoAluno, pt[i]);

  }


  return 0;
}
