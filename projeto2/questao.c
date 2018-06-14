#include <stdio.h>
#include <limits.h>

int main()
{
    long number;
    if ( 1 != scanf("%ld", &number) )
        return 1;

    // sign bit  (cannot use 1L left-shift as that causes UB)
    putchar( '0' + (number < 0) );

    // value bits
    for (long bit = 1L << (CHAR_BIT * sizeof number - 2); bit; bit >>= 1)
        putchar( '0' + !!(number & bit) );

    putchar('\n');
}
