/**
 *  Functions to test if a number is perfect and find the next perfect
 *  number, as well as scaffolding to test these functions.
 */

#include <stdio.h>
#include "sumfactors.h"

int isperfect (int number);
int nextperfect (int number);


    int
main (int argc, char **argv)
{
    int num;

    printf ("Enter a number: ");

    if (scanf ("%d", &num) != 1)
    {
        printf ("Only numbers\n");
    }

    printf ("%d %s perfect.\n", num, isperfect (num)? "is" : "isn't");
    printf ("Next biggest perfect number is %d\n", nextperfect (num));

    return 0;
}

/**
 *  Finds the next largest perfect number from it's argument. If the
 *  argument is perfect, this function will search for the next largest
 *  perfect number.
 */
    int
nextperfect (int number)
{
    for (number ++; isperfect (number) == 0; number ++)
        ;

    return number;
}

/**
 *  Test if a given number is perfect, meaning that the number's factors
 *  add up to the number itself.
 */
    int
isperfect (int number)
{
    if (sumfactors (number) == number)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/** vim: set ts=4 sw=4 et : */
