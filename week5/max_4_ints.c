/**
 *  Test program for a function that returns the largest of four ints.
 */

#include <stdio.h>

int max_4_ints (int n1, int n2, int n3, int n4);

    int
main (int argc, char **argv)
{
    int a, b, c, d;

    if (scanf ("%d %d %d %d", &a, &b, &c, &d) != 4)
    {
        printf ("Only numbers accepted\n");
        return 1;
    }

    printf ("max: %d\n", max_4_ints (a, b, c, d));

    return 0;
}

/**
 *  Returns the largest of four ints
 */
    int
max_4_ints (int n1, int n2, int n3, int n4)
{
}

/** vim: set ts=4 sw=4 et : */
