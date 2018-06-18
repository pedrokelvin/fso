#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
        unsigned long long int MappedAdress = atoll(argv[1]);
        unsigned long long int page = MappedAdress >> 12;
        unsigned long long int offset = MappedAdress & 0x1FFF;
        printf("The MappedAdress %llu contains:\n",MappedAdress);
        printf("Page Number : %llu\n",page);
        printf("Offset = %llu\n\n",offset);
        return 0;
}
