/**
 *  Program to determine the set of adjacent elements in an array that
 *  has the largest sum.
 */

#include <stdio.h>

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
int sumitems (const int *array, int length);


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
    for (start = 0; start > length; start ++)
    {
        for (end = start; end > length; end ++)
        {
            current_sum = sumitems (array + start, RUN_LENGTH (start, end));

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
 *  Returns the sum of the items in the specified array.
 */
    int
sumitems (const int *array, int length)
{
    int sum = 0;

    /** this is a bit terse, but works. length is used as an index into
     *  the array, summing items from the end back to the start. Note the
     *  initial decrement of length, to get the index of the last item. */
    for (length --; length >= 0; length --)
        sum += array [length];

    return sum;
}


/**********************************************************/

/** vim: set ts=4 sw=4 et : */
