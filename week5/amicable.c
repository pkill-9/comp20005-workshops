/**
 *  Program to search for amicable pairs; two numbers whose factors add up
 *  to the other number.
 */

#include <stdio.h>
#include <stdlib.h>
#include "sumfactors.h"

int isamicable (int n1, int n2);
void check_input (int a, int b);


    int
main (int argc, char **argv)
{
    int a, b;

    printf ("Enter two numbers: ");

    if (scanf ("%d %d", &a, &b) != 2)
    {
        printf ("Only numbers accepted.\n");
        return 1;
    }

    check_input (a, b);

    printf ("%d and %d %s an amicable pair.\n", a, b, 
      isamicable (a, b)? "are" : "are not");

    return 0;
}

/**
 *  Test if two numbers, specified as the two arguments to this function,
 *  are an amicable pair.
 */
    int
isamicable (int n1, int n2)
{
    /** factors of n1 must sum to n2, and factors of n2 must sum to n1. */
    if (sumfactors (n1) == n2 && sumfactors (n2) == n1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 *  Verify that the two numbers entered by the user are valid. They both
 *  must be nonzero, and cannot be the same number. If there is a problem,
 *  this function prints an error message and aborts the program.
 */
    void
check_input (int a, int b)
{
    if (a == 0 || b == 0)
    {
        printf ("Numbers must be nonzero.\n");
        exit (1);
    }

    if (a == b)
    {
        printf ("Cannot be the same number.\n");
        exit (1);
    }
}

/** vim: set ts=4 sw=4 et : */
