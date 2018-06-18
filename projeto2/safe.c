
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define TAMANHO_PAGINA 256
#define TAMANHO_VIRTUAL 256
#define TAMANHO_FISICA 256
#define TAMANHO_TBL 16
#define TAMANHO_QUADRO 256 // Para dar 256 quadros de 256 bytes


int main(int argc, char *argv[]){
  int total_numeros = 0;
  int tlb[TAMANHO_TBL][2];

  int memoria_fisica[TAMANHO_FISICA];
  int memoria_virtual[TAMANHO_VIRTUAL][2];

  // INITIAL PAGE TABLE FILLING
  // [0] Endereço Físico [1] Age
  for(int i = 0; i < TAMANHO_VIRTUAL; i++)
  {
    if (i > TAMANHO_FISICA -1)
    {
      memoria_virtual[i][0] = -1;
      memoria_virtual[i][1] = -1;
    }
    else
    {
      memoria_virtual[i][0] = i;
      memoria_virtual[i][1] = TAMANHO_FISICA - i;
    }
  }
  for(int i = 0; i < TAMANHO_FISICA; i++)
  {
      memoria_fisica[i] = i;
  }

  for(int i = 0; i < TAMANHO_TBL; i++)
  {
    if (i > TAMANHO_TBL - 1)
    {
      tlb[i][0] = -1;
      tlb[i][1] = -1;
    }
    else
    {
      tlb[i][0] = i;
      tlb[i][1] = TAMANHO_TBL - i;
    }
  }

  FILE *pFile;
  pFile = fopen(argv[1], "r");

  char numero[20];

  int contador_pagina = 0;
  int endereco_fisico = 0;
  int tbl_count = 0;


  printf("==================================\n" );
  printf("||    Virtual Memory Manager    ||\n" );
  printf("==================================\n" );

  printf("Translating 1000 Logical Addresses: \n\n");


  while((fscanf(pFile, "%s", numero)) == 1)
  {
      total_numeros++;
      int offset = atoi(numero) & 255; // 255 -> 0000 0000 1111 1111
      int pagina = (atoi(numero) & 65280) >> 8; //65280 -> 1111 1111 0000 0000 depois do shift 0000 0000 1111 1111
      int tlb_utilizado = 0;

      for(int i = 0; i < TAMANHO_TBL; i++)
      {
       if(tlb[i][0] == pagina)
       {
         tlb_utilizado = 1;
         printf("TLB UTILIZADO\n");
         tbl_count++;
         break;
       }
     }

      if(memoria_virtual[pagina][0] < 0 && !tlb_utilizado)
      {

          // EVICT SOMEONE
          int largest = 0;
          int evict = 0;
          for(int i = 0; i < TAMANHO_VIRTUAL; i++)
          {
              if(memoria_virtual[i][1] > largest)
              {
                  largest = memoria_virtual[i][1];
                  evict = i;
              }
          }

          //Andando a fila
          int aux_numero;
          int aux_endereco;
          for(int i=15; i< 1; i++)
          {
            aux_numero = tlb[i-1][0];
            aux_endereco = tlb[i-1][1];
            tlb[i][0] = aux_numero;
            tlb[i][1] = aux_endereco;
          }
          //Colocando o número na primeira posição da fila
          tlb[0][0] = pagina;
          tlb[0][1] = memoria_virtual[evict][0];

          memoria_virtual[pagina][0] = memoria_virtual[evict][0];
          memoria_virtual[pagina][1] = 0;
          memoria_virtual[evict][0] = -1;
          memoria_virtual[evict][1] = 0;

      }

      printf("Endereço Virtual = %d     \t", pagina);

      //Calculando endereço físico
      endereco_fisico = (memoria_fisica[memoria_virtual[pagina][0]] * TAMANHO_PAGINA) + offset;

      printf("Endereço Físico: %d\n", endereco_fisico);


      contador_pagina++;

      for(int i = 0; i < TAMANHO_VIRTUAL; i++)
      {
          memoria_virtual[i][1]++;
      }
  }

  //Calculando pages faults
  double porcentagem_tbl = 0.0;
  porcentagem_tbl = (double) tbl_count / total_numeros * 100;
  printf("\nPorcentagem de Page Fault: %.2f%% \n", porcentagem_tbl);
  //Falta a taxa de erros da página

  fclose(pFile);
  exit(EXIT_SUCCESS);
}
