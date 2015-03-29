/**
 *  Takes a value from the user and prints out the next biggest prime
 *  number.
 */

#include <stdio.h>

    int
main (int argc, char **argv)
{
    int number, isprime = 0, divisor;

    printf ("Enter a number: ");

    if (scanf ("%d", &number) != 1)
    {
        printf ("Must be a number.\n");
        return 1;
    }

    while (!isprime)
    {
        isprime = 1;
        number += 1;

        /** search for a factor of number that is not either 1 or number.
         *  If we find such a factor, number is not prime. */
        for (divisor = 2; divisor * divisor <= number; divisor ++)
        {
            if (number % divisor == 0)
            {
                isprime = 0;
                break;
            }
        }
    }

    printf ("%d is the next biggest prime.\n", number);

    return 0;
}

/** vim: set ts=4 sw=4 et : */
