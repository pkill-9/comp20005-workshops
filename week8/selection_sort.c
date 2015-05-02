
#include <stdio.h>

#define LENGTH          100

/**********************************************************/

void selection_sort (int *a, int length);
int find_max (int *array, int length);
void swap_ints (int *a, int *b);
void print_array (int *a, int n);

/**********************************************************/

    int
main (int argc, char **argv)
{
    int numbers [LENGTH];
    int numitems = 0;

    while (scanf ("%d", numbers + numitems) == 1 && numitems < LENGTH)
        numitems ++;

    selection_sort (numbers, numitems);
    print_array (numbers, numitems);

    return 0;
}

/**********************************************************/

/**
 *  Sort the given array by finding the largest value and swapping it to
 *  the end.
 */
    void
selection_sort (int a[], int length)
{
    int maxindex;

    /** An array of length 1 is implicitly sorted, since it cannot be in
     *  the wrong order. In that case, we have nothing more to do. */
    if (length == 1)
        return;

    /** For longer arrays, we need to find the largest value, swap it to
     *  the end, then sort the remaining unsorted values. */
    maxindex = find_max (a, length);
    swap_ints (a + maxindex, a + length - 1);
    selection_sort (a, length - 1);
}

/**********************************************************/

/**
 *  Returns the index of the largest value in the given array.
 */
    int
find_max (int *array, int length)
{
    int i, maxval = array [0], maxindex = 0;

    for (i = 0; i < length; i ++)
    {
        if (array [i] > maxval)
        {
            maxval = array [i];
            maxindex = i;
        }
    }

    return maxindex;
}

/**********************************************************/

/**
 *  print the contents of the given array on stdout with a new line at the
 *  end.
 */
    void
print_array (int *a, int n)
{
    int i;

    for (i = 0; i < n; i ++)
        printf ("%d, ", a [i]);

    printf ("\n");
}

/**********************************************************/

    void
swap_ints (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
