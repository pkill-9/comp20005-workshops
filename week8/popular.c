/**
 *  Solution to problem 7.7 in PPSAA. A function that takes as arguments
 *  an array and length, and returns the value that appears most frequently
 *  in the array.
 */

#include <stdio.h>

#define MAX_NUMBERS     1000

/**********************************************************/

int popular (int *array, int length);
int occurences (int *array, int length, int value);

/**********************************************************/

    int
main (int argc, char **argv)
{
    int numbers [MAX_NUMBERS], i = 0;

    printf ("Enter up to %d numbers, ^D to end\n", MAX_NUMBERS);

    while ((scanf ("%d", &(numbers [i])) == 1) && i < MAX_NUMBERS)
        i ++;

    printf ("Most frequent value: %d\n", popular (numbers, i));

    return 0;
}

/**********************************************************/

/**
 *  Function that returns the value that appears most frequently in the
 *  given array.
 */
    int
popular (int *array, int length)
{
    int most_popular = array [0]; 
    int max_frequency = occurences (array, length, most_popular);
    int i, frequency;

    /** step through the array, and for each value, count how many times
     *  it occurrs in total. */
    for (i = 0; i < length; i ++)
    {
        if ((frequency = occurences (array, length, array [i])) >= 
          max_frequency)
        {
            /** if there are two values with the same frequency, take the
             *  smaller value as the most frequent. */
            if (frequency == max_frequency)
            {
                if (array [i] < most_popular)
                    most_popular = array [i];
            }
            else
            {
                most_popular = array [i];
                max_frequency = frequency;
            }
        }
    }

    return most_popular;
}

/**********************************************************/

/**
 *  Returns the number of times the specified value occurrs in the given
 *  array.
 */
    int
occurences (int *array, int length, int value)
{
    int nr_occurences = 0, i;

    for (i = 0; i < length; i ++)
    {
        if (array [i] == value)
            nr_occurences += 1;
    }

    return nr_occurences;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
