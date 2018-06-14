#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int indexx=0;
char codigo[33];

void printInBinary(long num, int bit)
{
   if ( bit >= 4*sizeof(num) )
   {
      return;
   }

   printInBinary(num/2, bit+1);
   codigo[indexx++] = num%2 + '0';
}

int main()
{
   int y = 31;
   int conteudo;
   int pagina;
   int deslocamento;
   char aux[17];

   uint32_t x1 = 1;
   scanf(" %u", &x1);
   printInBinary(x1, 0);
   codigo[32] = '\0';
   printf("%s\n", codigo);

   strncpy(aux, codigo+16, 8);
   aux[8] = '\0';
   pagina = atoi(aux);
   strncpy(aux, codigo+24, 8);
   aux[8] = '\0';
   deslocamento = atoi(aux);
   printf("Pagina: %d\nDeslocamento: %d\n",pagina, deslocamento);
   // conteudo = atoi();
}
