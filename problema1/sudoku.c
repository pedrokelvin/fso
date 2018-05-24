#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* check_linha(void* sudoku){

	int **sudoku_aux = (int **) sudoku;
	int valor[9];
	int flag = 0;

	for(int p = 0; p < 9; p++){
		valor[p] = 1;
	}

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){

			if(--valor[sudoku_aux[i][j]-1] == (-1)*(i + 1))
				pthread_exit(&flag);
		}
	}

	flag = 1;
	pthread_exit(&flag);
}

void* check_coluna(void* sudoku){
	int **sudoku_aux = (int **) sudoku;
	int valor[9];
	int flag = 0;

	for(int p = 0; p < 9; p++){
		valor[p] = 1;
	}

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){

			if(--valor[sudoku_aux[j][i]-1] == (-1)*(j + 1))
				pthread_exit(&flag);
		}
	}

	flag = 1;
	pthread_exit(&flag);
}

// void* check_grid(){
//
// }

int main(){

	int sudoku[9][9];
	int resultado[11];

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

	for(int i = 0; i < 9; i++){
		printf("%d ",sudoku[0][i]);
	}

	// Quantidade de Threads
	int num_args = 11;
	pthread_t tids[num_args];

	// Check_linha
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	resultado[0] = pthread_create(&tids[0], &attr, check_linha, sudoku);


	// Check_coluna
	pthread_attr_t attr_1;
	pthread_attr_init(&attr_1);
	resultado[1] = pthread_create(&tids[1], &attr_1, check_coluna, sudoku);

	pthread_join(tids[0], NULL);
	pthread_join(tids[1], NULL);

	printf("%d %d\n", resultado[1], resultado[0]);

	return 0;
}
