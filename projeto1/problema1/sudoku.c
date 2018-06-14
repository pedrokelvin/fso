#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct dados_grid
{
	int (*sudoku)[9];
	int coluna;
	int linha;
} struct_grid;

void inicializar_vetor(int *valor)
{
	for(int p = 0; p <= 9; p++)
		valor[p] = 1;
}

void* check_linha(void* sudoku){

	int (*sudoku_aux)[9] = sudoku;
	int valor[9];
	int flag = 1;

	inicializar_vetor(valor);

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			// if(--valor[sudoku_aux[i][j]-1] == (-1)*(i + 1))
			// {
			// 	printf("Um erro encontrado na linha!\nNúmero %d se repete na linha %d, coluna: %d.\n\n", sudoku_aux[i][j], i+1, j+1);
			// 	pthread_exit((int*)0);
			// }

			if(--valor[sudoku_aux[i][j]-1] < 0){
				printf("Numero %d se repete na linha %d.\n\n",sudoku_aux[i][j],i+1);
				flag = 0;
			}
		}
		inicializar_vetor(valor);
	}

	if(flag)
		pthread_exit((int*)1);
	else
		pthread_exit((int*)0);
}

void* check_coluna(void* sudoku){
	int (*sudoku_aux)[9] = sudoku;
	int valor[9];
	int flag = 1;

	inicializar_vetor(valor);

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){

			// if(--valor[sudoku_aux[j][i]-1] == (-1)*(i + 1))
			// {
			// 	printf("Um erro encontrado na coluna!\nNúmero %d se repete na linha %d, coluna: %d.\n\n", sudoku_aux[i][j], i+1, j+1);
			// 	pthread_exit((int*)0);
			// }

			if(--valor[sudoku_aux[j][i]-1] < 0)
			{
				printf("Numero %d se repete na coluna %d.\n\n",sudoku_aux[j][i],i+1);
				flag = 0;
			}

		}
		inicializar_vetor(valor);
	}

	if(flag)
		pthread_exit((int*)1);
	else
		pthread_exit((int*)0);
}

void* check_grid(void* dados){
	struct_grid *dados_grid_aux = (struct_grid *) dados;
	int (*sudoku_aux)[9] = dados_grid_aux->sudoku;
	int valor[9];
	inicializar_vetor(valor);

	for(int i=dados_grid_aux->coluna; i<dados_grid_aux->coluna+3; i++)
	{
		for(int j=dados_grid_aux->linha; j > dados_grid_aux->linha-3; j--){
			if(--valor[sudoku_aux[j][i]] == -1)
			{
				printf("Um erro encontrado no grid!\nNúmero %d se repete na linha %d, coluna: %d.\n\n", sudoku_aux[j][i], j+1, i+1);
				pthread_exit((int*)0);
			}
		}
	}
	pthread_exit((int*)1);
}

int main(){
	int sudoku[9][9];
	int resultado[11];
	struct_grid dados[9];

	FILE *file;
	file = fopen("sudokus/10.txt", "r");

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
	pthread_t tids[num_args];

	// Check_linha
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tids[0], &attr, check_linha, sudoku);


	// Check_coluna
	pthread_attr_t attr_1;
	pthread_attr_init(&attr_1);
	pthread_create(&tids[1], &attr_1, check_coluna, sudoku);

	for (int i = 2; i < 11; i++) {
		dados[i-2].sudoku = sudoku;
		dados[i-2].coluna = ((i-2)%3)*3;

		if((i-2)<3)
			dados[i-2].linha = 2;
		else if((i-2)<6)
			dados[i-2].linha = 5;
		else
			dados[i-2].linha = 8;

		pthread_attr_t attr_i;
		pthread_attr_init(&attr_i);
		pthread_create(&tids[i], &attr_i, check_grid, &dados[i-2]);
	}

	int flag = 0;

	for (int i = 0; i < 11; i++) {
		pthread_join(tids[i], (void*)&resultado[i]);
	}

	for (int i = 0; i < 11; i++) {
		printf("%d\n", resultado[i]);
		if(resultado[i] == 0)
			flag = 1;
	}

	if(flag == 0)
		printf("SUDOKU VÁLIDO!\n");
	else
		printf("SUDOKU INVÁLIDO!\n");

	return 0;
}
