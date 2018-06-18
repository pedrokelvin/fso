
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

void manipulacaoErros(int pagina, int vetor[][256]) // Retorna o quadro da página requerida
{
    unsigned char buf[TAMANHO_QUADRO] = {0}; // 8bits = 1 byte (0 - 255)
    int flag = 0;

    FILE *fp = fopen ("BACKING_STORE.bin", "rb");
    for(int count=0; ((fread(buf, sizeof *buf, TAMANHO_QUADRO, fp) && flag == 0)); count++)
    {
        for (int i = 0; i < TAMANHO_QUADRO; i++)
        {
          if(count == pagina)
          {
            vetor[pagina][i] = buf[i]; //Salvando quadro
            flag = 1;
          }
        }
    }
    fclose (fp);
}

int main(int argc, char *argv[]){
  int total_numeros = 0;
  int tlb[TAMANHO_TBL][2];

  int memoria_fisica[TAMANHO_FISICA][256]; // [QUADRO][INFORMAÇÕES]
  int memoria_virtual[TAMANHO_VIRTUAL][2];

  // Memória Virtual
  // [0] Id [1] Nº Quadro (Memória física)
  for(int i = 0; i < TAMANHO_VIRTUAL; i++)
  {
      memoria_virtual[i][0] = i;
      memoria_virtual[i][1] = -1;
      // Memória Física
      // [0] Id [1] Informações
      for (int j = 0; j < 256; j++) {
        memoria_fisica[i][j] = -1;
      }
  }

  for(int i = 0; i < TAMANHO_TBL; i++)
  {
    tlb[i][0] = -1;
    tlb[i][1] = -1;
  }

  char numero[20];
  int contador_pagina = 0;
  int endereco_fisico = 0;
  int tlb_count = 0;
  int error_count = 0;

  FILE *pFile;
  pFile = fopen(argv[1], "r");

  while((fscanf(pFile, "%s", numero)) == 1)
  {
      total_numeros++;
      int offset = atoi(numero) & 255; // 255 -> 0000 0000 1111 1111
      int pagina = (atoi(numero) & 65280) >> 8; //65280 -> 1111 1111 0000 0000 depois do shift 0000 0000 1111 1111
      int flag_tlb = 0;

      for(int i = 0; i < TAMANHO_TBL; i++)
      {
       if(tlb[i][0] == pagina)
       {
         flag_tlb = 1;
         // printf("TLB FOI UTILIZADO\n");
         tlb_count++;
         break;
       }
      }

     if(memoria_virtual[pagina][1] < 0 && !flag_tlb)
     {
       // printf("MANIPULAÇÃO DE ERROS FOI UTILIZADA!\n");
       manipulacaoErros(pagina, memoria_fisica);
       memoria_virtual[pagina][1] = pagina;
       error_count++;
       // printf("Informação = %d\n", memoria_fisica[pagina][offset]);
     }
     else
     {
        // printf("A TABELA DE PÁGINAS FOI UTILIZADA\n");

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
        tlb[0][1] = memoria_virtual[pagina][0];
      }

      printf("Endereço Virtual = %d     \t", pagina);

      //Calculando endereço físico
      endereco_fisico = (memoria_virtual[pagina][1] * TAMANHO_PAGINA) + offset;

      printf("Endereço Físico: %d     \t", endereco_fisico);

      printf("Informação = %d\n", memoria_fisica[memoria_virtual[pagina][1]][offset]);
      contador_pagina++;
  }

  //Calculando pages faults
  printf("\nTaxa de erros de página %.2f%%   TOTAL ERROS -> %d\n", (double) error_count / total_numeros *100, error_count);
  printf("\nTaxa de sucesso do TLB: %.2f%%   Total TLB -> %d\n", (double) tlb_count / total_numeros * 100, tlb_count);

  //Falta a taxa de erros da página

  // for (int i = 0; i < TAMANHO_QUADRO; i++) {
  //   for (int j = 0; j < TAMANHO_QUADRO; j++) {
  //     printf("%d ", memoria_fisica[i][j]);
  //   }
  //   printf("\n");
  // }


  fclose(pFile);
  exit(EXIT_SUCCESS);
}
