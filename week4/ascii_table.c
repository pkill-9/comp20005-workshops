/**
 *  Prints out every single ASCII character.
 */

#include <stdio.h>

    int
main (int argc, char **argv)
{
    int ch = 0;

    for (ch = 0; ch <= 0xFF; ch ++)
    {
        printf ("%c", ch);
    }

    return 0;
}
