/**
 *  Program to read numbers from stdin and print how many times each
 *  distinct number occurrs.
 */

#include <stdio.h>

#define MAX_NUMBERS     1000

/**********************************************************/

void bubble_sort (int *array, int nitems);
void swap_ints (int *a, int *b);
void print_histogram (int *numbers, int length);

/**********************************************************/

    int
main (int argc, char **argv)
{
    int numbers [MAX_NUMBERS];
    int i = 0;

    printf ("Enter up to %d numbers, ^D to end\n", MAX_NUMBERS);

    while ((scanf ("%d", &(numbers [i])) == 1) && i < MAX_NUMBERS)
        i ++;

    bubble_sort (numbers, i);
    print_histogram (numbers, i);

    return 0;
}

/**********************************************************/

/**
 *  Sort the given array using bubblesort.
 */
    void
bubble_sort (int *array, int nitems)
{
    int didswaps = 1, i;

    while (didswaps)
    {
        didswaps = 0;

        for (i = 0; i < nitems - 1; i ++)
        {
            if (array [i] > array [i + 1])
            {
                swap_ints (array + i, array + i + 1);
                didswaps = 1;
            }
        }
    }
}

/**********************************************************/

/**
 *  Swap the two ints pointed to by the two arguments.
 */
    void
swap_ints (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**********************************************************/

/**
 *  Function that, given a sorted array, prints out each distinct value,
 *  and the number of times each value occurrs.
 */
    void
print_histogram (int *numbers, int length)
{
    int previous_value = 0, i, nr_occurences = 1;

    printf ("value      frequency\n");

    /** if a value occurrs more than once, the repeat values will be all
     *  clustered together, so we can just count them. */
    for (i = 0; i < length; i ++)
    {
        /** the first item has no previous value. */
        if (i != 0)
        {
            /** is this value the same as the previous? If so, increment
             *  the number of occurences. */
            if (numbers [i] != previous_value)
            {
                printf ("%4d %10d\n", previous_value, nr_occurences);
                previous_value = numbers [i];
                nr_occurences = 1;
            }
            else
            {
                nr_occurences += 1;
            }
        }
        else
        {
            previous_value = numbers [i];
            nr_occurences = 1;
        }
    }

    printf ("%4d %10d\n", previous_value, nr_occurences);
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
