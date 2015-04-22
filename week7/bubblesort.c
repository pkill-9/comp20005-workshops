/**
 *  Simple sorting program.
 */

#include <stdio.h>

#define MAX_ITEMS       10

/**********************************************************/

void bubble_sort (int array [], int nitems);
void swap_ints (int *a, int *b);
void print_array (int array [], int nitems);

/**********************************************************/

    int
main (int argc, char **argv)
{
    int length = 0, next_num;
    int array [MAX_ITEMS];

    printf ("Enter as many as 10 numbers, ^D to end.\n");

    while ((scanf ("%d", &next_num) == 1) && length < MAX_ITEMS)
    {
        array [length] = next_num;
        length ++;
    }

    printf ("Before:    ");
    print_array (array, length);

    printf ("After :    ");
    bubble_sort (array, length);
    print_array (array, length);

    return 0;
}

/**********************************************************/

/**
 *  Sort the given array using bubblesort.
 */
    void
bubble_sort (int array [], int nitems)
{
    int didswaps = 1, i;

    while (didswaps)
    {
        didswaps = 0;

        for (i = 0; i < nitems - 1; i ++)
        {
            if (array [i] > array [i + 1])
            {
                swap_ints (&(array [i]), &(array [i + 1]));
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
 *  Steps through an array of ints and prints each item on stdout, with
 *  items separated by spaces. A new line is printed at the end of the
 *  array.
 */
    void
print_array (int array [], int nitems)
{
    int i;

    for (i = 0; i < nitems; i ++)
        printf ("%4d", array [i]);

    printf ("\n");
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
