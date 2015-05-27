/**
 *  Program to determine the set of adjacent elements in an array that
 *  has the largest sum.
 */

#include <stdio.h>

#define MAXITEMS        100

/** find the length of a run of items in an array from the indices of the
 *  first and last elements in the run */
#define RUN_LENGTH(start, end)  ((end) - (start) + 1)


/** This struct is used to specify a run of adjacent elements in an array,
 *  by storing the index of the start of the run, and the length. */
typedef struct
{
    int start_index;
    int length;
    int sum;
}
run_t;


/**********************************************************/

void adjacent_sum (run_t *max, const int *array, int length);
void print_array (const int *array, int length);


/**********************************************************/

    int
main (void)
{
    int array [MAXITEMS], length = 0;
    run_t max_run;

    /** prompt the user to enter some input. */
    printf ("Enter up to %d ints:\n", MAXITEMS);

    while (scanf ("%d", &(array [length ++])) == 1 && (length < MAXITEMS))
        ;

    /** find the run of adjacent elements with the biggest sum */
    adjacent_sum (&max_run, array, length);

    printf ("Adjacent elements with maximum sum:\n");
    print_array (array + max_run.start_index, max_run.length);

    return 0;
}


/**********************************************************/

/**
 *  Finds the run of items in the array with the maximum sum, and stores
 *  the start index and length in the respective fields of max.
 *
 *  The approach used is generate and test, basically a brute force
 *  approach: step through all possible runs of elements, calculate their
 *  sums, and select the maximum.
 */
    void
adjacent_sum (run_t *max, const int *array, int length)
{
    int start = 0, end, current_sum;

    /** initialise the max to a run containing only the first item in the
     *  array */
    max->start_index = 0;
    max->length = 1;
    max->sum = array [0];

    /** Step through all start indices, from 0 to the end of the array,
     *  and all end indices from the start index to the end of the array */
    for (start = 0; start < length; start ++)
    {
        current_sum = 0;

        for (end = start; end < length; end ++)
        {
            current_sum += array [end];

            if (current_sum > max->sum)
            {
                /** found a new max */
                max->start_index = start;
                max->length = RUN_LENGTH (start, end);
                max->sum = current_sum;
            }
        }
    }
}


/**********************************************************/

/**
 *  Prints the given array on stdout, with a newline at the end.
 */
    void
print_array (const int *array, int length)
{
    int i;

    for (i = 0; i < length; i ++)
        printf ("%d ", array [i]);

    printf ("\n");
}


/**********************************************************/

/** vim: set ts=4 sw=4 et : */
