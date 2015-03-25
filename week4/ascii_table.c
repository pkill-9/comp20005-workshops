/**
 *  Prints out every single ASCII character.
 */

#include <stdio.h>

    int
main (int argc, char **argv)
{
    int ch = 0;

    /** table header */
    printf ("       +0   +1   +2   +3   +4   +5   +6   +7\n");
    printf ("     +----+----+----+----+----+----+----+----+");

    /** the table will only include "printable chars", which are between
     *  ASCII values of 32 and 126. */
    for (ch = 32; ch <= 126; ch ++)
    {
        /** each row will be 8 chars long, so print a newline before each
         *  row that is divisible by 8. */
        if (ch % 8 == 0)
            printf ("\n%4d |", ch);

        printf ("  %c  ", ch);
    }

    return 0;
}
