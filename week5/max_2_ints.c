/**
 *  Scaffolding around our max_2_ints function.
 */

#include <stdio.h>

int max_2_ints (int n1, int n2);

    int
main (int argc, char **argv)
{
    int a, b;

    if (scanf ("%d %d", &a, &b) != 2)
    {
        printf ("Only numbers accepted\n");
        return 1;
    }

    printf ("max: %d\n", max_2_ints (a, b));

    return 0;
}

/**
 *  Returns the larger of the two arguments.
 */
    int
max_2_ints (int n1, int n2)
{
}

/** vim: set ts=4 sw=4 et : */
