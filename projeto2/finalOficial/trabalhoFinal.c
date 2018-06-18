
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define TAMANHO_PAGINA 256
#define TAMANHO_VIRTUAL 256
#define TAMANHO_FISICA 256
#define TAMANHO_TLB 16
#define TAMANHO_QUADRO 256 // Para dar 256 quadros de 256 bytes

int count_memoriaFisica = 0;
int count_memoriaVirtual = 0;

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
            vetor[count_memoriaFisica][i] = buf[i]; //Salvando quadro
            flag = 1;
          }
        }
    }
    count_memoriaFisica++;
    fclose (fp);
}

void atualizar_TLB(int tlb[][2], int pagina, int quadro)
{
  // printf("A TABELA DE PÁGINAS FOI UTILIZADA\n");
  //FIFO
  int aux_numero;
  int aux_endereco;
  for(int j=TAMANHO_TLB-1; j>=1; j--)
  {
    aux_numero = tlb[j-1][0];
    aux_endereco = tlb[j-1][1];
    tlb[j][0] = aux_numero;
    tlb[j][1] = aux_endereco;
  }
  //Colocando o número na primeira posição da fila
  tlb[0][0] = pagina;
  tlb[0][1] = quadro;

}

int main(int argc, char *argv[]){
  int total_numeros = 0;
  int tlb[TAMANHO_TLB][2];

  int memoria_fisica[TAMANHO_FISICA][256]; // [QUADRO][INFORMAÇÕES]
  int memoria_virtual[TAMANHO_VIRTUAL][2];

  // Memória Virtual
  // [0] Página [1] Nº Quadro (Memória física)
  for(int i = 0; i < TAMANHO_VIRTUAL; i++)
  {
    memoria_virtual[i][0] = -1;
    memoria_virtual[i][1] = -1;
    // Memória Física
    for (int j = 0; j < 256; j++)
    {
      memoria_fisica[i][j] = -1;
    }
  }

  for(int i = 0; i < TAMANHO_TLB; i++)
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
    int flag_tabelaPagina = 0;

    for(int i = 0; i < TAMANHO_TLB; i++)
    {
      if(tlb[i][0] == pagina)
       {
         tlb_count++;
         flag_tlb = 1;
         // printf("TLB FOI UTILIZADO\n");
         printf("Endereço lógico = %s      \tQuadro = %d    \tOffset = %d      \t", numero, tlb[i][1], offset);
         endereco_fisico = (tlb[i][1] * TAMANHO_PAGINA) + offset;
         printf("Endereço Físico: %d     \t", endereco_fisico);
         printf("Valor do Byte = %d\n", memoria_fisica[tlb[i][1]][offset]);
         break;
       }
    }

   if(!flag_tlb)
   {

     for (int i = 0; i < TAMANHO_VIRTUAL; i++)
     {
       if (memoria_virtual[i][0] == pagina)
       {
         flag_tabelaPagina = 1;
         printf("Endereço lógico = %s      \tQuadro = %d    \tOffset = %d      \t", numero, memoria_virtual[i][1], offset);
         endereco_fisico = (memoria_virtual[i][1] * TAMANHO_PAGINA) + offset;
         printf("Endereço Físico: %d     \t", endereco_fisico);
         printf("Valor do Byte = %d\n", memoria_fisica[memoria_virtual[count_memoriaVirtual-1][1]][offset]);
         atualizar_TLB(tlb, memoria_virtual[i][0], memoria_virtual[i][1]);
         break;
       }
     }

     if(flag_tabelaPagina == 0)
     {
       // printf("MANIPULAÇÃO DE ERROS FOI UTILIZADA!\n");
       manipulacaoErros(pagina, memoria_fisica);
       memoria_virtual[count_memoriaVirtual][0] = pagina;
       memoria_virtual[count_memoriaVirtual++][1] = count_memoriaFisica-1;
       error_count++;

       printf("Endereço lógico = %s      \tQuadro = %d    \tOffset = %d      \t", numero, memoria_virtual[count_memoriaVirtual-1][1], offset);
       endereco_fisico = (memoria_virtual[count_memoriaVirtual-1][1] * TAMANHO_PAGINA) + offset;
       printf("Endereço Físico: %d     \t", endereco_fisico);
       printf("Valor do Byte = %d\n", memoria_fisica[memoria_virtual[count_memoriaVirtual-1][1]][offset]);
       atualizar_TLB(tlb, memoria_virtual[count_memoriaVirtual-1][0], memoria_virtual[count_memoriaVirtual-1][1]);
     }

  }

    contador_pagina++;
  }

  printf("\nTaxa de erros de página %.2f%%   TOTAL ERROS -> %d\n", (double) error_count / (double)total_numeros *100, error_count);
  printf("\nTaxa de sucesso do TLB: %.2f%%   Total TLB -> %d\n", (double) tlb_count / total_numeros * 100, tlb_count);

  fclose(pFile);
  exit(EXIT_SUCCESS);
}
