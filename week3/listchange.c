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

    int
main (int argc, char **argv)
{
    int balance;

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

    /** This will be a long sequence of if constructs. We will start with
     *  subtracting the largest coin from the balance then move on to
     *  smaller coins. */
    if (balance >= COIN50)
    {
        printf ("%d, ", COIN50);
        balance -= COIN50;
    }

    if (balance >= COIN20)
    {
        printf ("%d, ", COIN20);
        balance -= COIN20;
    }

    if (balance >= COIN10)
    {
        printf ("%d, ", COIN10);
        balance -= COIN10;
    }

    if (balance >= COIN5)
    {
        printf ("%d, ", COIN5);
        balance -= COIN5;
    }

    if (balance >= COIN2)
    {
        printf ("%d, ", COIN2);
        balance -= COIN2;

        /** Sometimes (eg for 4 cents) we will need two 2 cent coins. */
        if (balance >= COIN2)
        {
            printf ("%d, ", COIN2);
            balance -= COIN2;
        }
    }

    if (balance >= COIN1)
    {
        printf ("%d, ", COIN1);
        balance -= COIN1;
    }

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
