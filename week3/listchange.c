/**
 *  Program to list the coins needed to make up a value between 0 and 99
 *  cents.
 */

#include <stdio.h>
#include <stdlib.h>

/** Definition of coin values for Australian currency. */
#define COIN0       50
#define COIN1       20
#define COIN2       10
#define COIN3       5
#define COIN4       2
#define COIN5       1

    int
main (int argc, char **argv)
{
    int balance;

    printf ("Enter a value in cents: ");

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
    if (balance >= COIN0)
    {
        printf ("%d, ", COIN0);
        balance -= COIN0;
    }

    if (balance >= COIN1)
    {
        printf ("%d, ", COIN1);
        balance -= COIN1;
    }

    if (balance >= COIN2)
    {
        printf ("%d, ", COIN2);
        balance -= COIN2;
    }

    if (balance >= COIN3)
    {
        printf ("%d, ", COIN3);
        balance -= COIN3;
    }

    if (balance >= COIN4)
    {
        printf ("%d, ", COIN4);
        balance -= COIN4;
    }

    if (balance >= COIN5)
    {
        printf ("%d, ", COIN5);
        balance -= COIN5;
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
