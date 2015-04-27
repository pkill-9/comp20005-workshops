/**
 *  Program to read numbers from stdin and print how many times each
 *  distinct number occurrs.
 */

#include <stdio.h>

#define NR_BUCKETS      1000

/**********************************************************/

void clear_histogram (int *buckets, int length);
void print_histogram (const int *buckets, int length);

/**********************************************************/

    int
main (int argc, char **argv)
{
    int histogram [NR_BUCKETS];
    int number;

    printf ("Enter numbers between 0 and %d, ^D to end\n", NR_BUCKETS);
    clear_histogram (histogram, NR_BUCKETS);

    while (scanf ("%d", &number) == 1) 
        histogram [number] += 1;

    print_histogram (histogram, NR_BUCKETS);

    return 0;
}

/**********************************************************/

/**
 *  Steps through the given array and sets all the values to 0.
 */
    void
clear_histogram (int *buckets, int length)
{
    int i;

    for (i = 0; i < length; i ++)
        buckets [i] = 0;
}

/**********************************************************/

/**
 *  Given a list of n ints where the ith int is the number of occurences
 *  of the value i in the input, prints out how many times each distinct
 *  value occurrs.
 */
    void
print_histogram (const int *buckets, int length)
{
    int i;

    printf ("value      frequency\n");

    for (i = 0; i < length; i ++)
    {
        if (buckets [i] != 0)
            printf ("%4d %10d\n", i, buckets [i]);
    }
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
