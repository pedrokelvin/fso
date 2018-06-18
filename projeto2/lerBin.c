#include <stdio.h>

#define TAMANHO_QUADRO 256 // Para dar 256 quadros de 256 bytes

int main (int argc, char **argv) {

  unsigned char buf[TAMANHO_QUADRO] = {0}; // 8bits = 1 byte (0 - 255)
  // int count = 0;
  int vetor[256];
  // int i;
  int flag=0;

  FILE *fp = fopen ("BACKING_STORE.bin", "rb");
  int quadro = 255; //varia de 0 até 255

  for(int count=0; ((fread (buf, sizeof *buf, TAMANHO_QUADRO, fp)) == TAMANHO_QUADRO && flag == 0); count++)
  {
      for (int i = 0; i < TAMANHO_QUADRO; i++)
      {
        if(count == quadro)
        {
          vetor[i] = buf[i]; //Salvando página
          flag = 1;
        }
      }
    // count++;
  }

  fclose (fp);

  for (int i = 0; i < TAMANHO_QUADRO; i++) {
    printf("%d ", vetor[i]);
  }

  return 0;
}
