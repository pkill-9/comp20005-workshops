/**
 *  Program to search for amicable pairs; two numbers whose factors add up
 *  to the other number.
 */

#include <stdio.h>
#include <stdlib.h>
#include "sumfactors.h"

int isamicable (int n1, int n2);
int next_amicable (int number);


    int
main (int argc, char **argv)
{
    int number = 1, pair;

    while (1)
    {
        /** number always contains the larger number of the amicable pair.
         *  Start next_amicable searching from the largest of the last
         *  pair, so that the next amicable pair it will find will not be
         *  the same pair. */
        number = next_amicable (number);
        pair = sumfactors (number);
        printf ("%10d and %10d\n", pair, number);
    }

    return 0;
}

/**
 *  Returns one number of the next largest amicable pair. The second number
 *  of the pair can then be computed by sumfactors. This function only
 *  requires one parameter, the second amicable number will be calculated
 *  by summing the factors of the number that we are given.
 *
 *  Returns the larger number of the pair.
 */
    int
next_amicable (int number)
{
    int pair;

    number += 1;
    pair = sumfactors (number);

    while (sumfactors (pair) != number || number == pair)
    {
        number ++;
        pair = sumfactors (number);
    }

    /** note that number and pair are not the same number, and we have
     *  swept number up through all the smaller numbers. This means that
     *  pair has to be greater than number. */
    return pair;
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

/** vim: set ts=4 sw=4 et : */
