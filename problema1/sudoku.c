#include <stdio.h>
#include <stdlib.h>
// #include <pthread.h>

// struct sum_runner_struct {
// 	long long limit;
// 	long long answer;
// };
//
// // Thread function to generate sum of 0 to N
// void* sum_runner(void* arg)
// {
// 	struct sum_runner_struct *arg_struct =
// 			(struct sum_runner_struct*) arg;
//
// 	long long sum = 0;
// 	for (long long i = 0; i <= arg_struct->limit; i++) {
// 		sum += i;
// 	}
//
// 	arg_struct->answer = sum;
//
// 	pthread_exit(0);
// }

int main(){

	int sudoku[9][9];

	int c;
	FILE *file;
	file = fopen("numeros.txt", "r");

	int k = 0;
	fscanf (file, "%d", &k);

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			sudoku[i][j] = k;
			fscanf (file, "%d", &k);
		}
	}

	fclose(file);

	// Quantidade de Threads
	int num_args = 11;

	struct sum_runner_struct args[num_args];

	// Launch thread
	pthread_t tids[num_args];
	for (int i = 0; i < num_args; i++) {
		args[i].limit = atoll(argv[i + 1]);

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids[i], &attr, sum_runner, &args[i]);
	}

	// Wait until thread is done its work
	for (int i = 0; i < num_args; i++) {
		pthread_join(tids[i], NULL);
		printf("Sum for thread %d is %lld\n",
				i, args[i].answer);
	}

	return 0;
}
