/**
 *  Program to list the coins needed to make up a value between 0 and 99
 *  cents.
 */

#include <stdio.h>
#include <stdlib.h>

/** Definition of coin values for Australian currency. */
#define COIN50       50
#define COIN20       20
#define COIN10       10
#define COIN5       5
#define COIN2       2
#define COIN1       1

#define NUM_COINS   6
const int coins [] = {COIN50, COIN20, COIN10, COIN5, COIN2, COIN1};

    int
main (int argc, char **argv)
{
    int balance, i, numcoins;

    printf ("Enter a value in cents:\n");

    /** get the ammount of money from the user, and abort the program on
     *  non integer input. */
    if (scanf ("%d", &balance) != 1)
    {
        printf ("Integer values only.\n");
        exit (1);
    }

    if ((balance < 0) || (balance > 99))
    {
        printf ("Only supports values between 0 and 99 cents.\n");
        exit (1);
    }

    for (i = 0; i < NUM_COINS; i ++)
    {
        numcoins = balance / coins [i];

        if (numcoins != 0)
        {
            while (numcoins > 0)
            {
                printf ("%d, ", coins [i]);
                numcoins -= 1;
            }
        }

        balance = balance % coins [i];
    }

    printf ("\n");

    /** Check that we have provided coins adding up to the value that the
     *  user specified. If we have not provided enough coins, the balance
     *  will not be 0. */
    if (balance != 0)
    {
        printf ("Error: short changed by %d cents.\n", balance);
        exit (1);
    }

    return 0;
}

/** vim: set ts=4 sw=4 et : */
