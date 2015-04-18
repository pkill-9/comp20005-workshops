/**
 *  Program that generates a certain number of polygons choosing the 
 *  coordinates of the corners at random.
 *
 *  If an argument is supplied to this program, it will generate that
 *  number of polygons. Without an argument, the number of polygons to be
 *  generated will also be chosen at random.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**********************************************************/

int random_int_between (int lower, int upper);
void print_random_polygon (void);

/**********************************************************/

    int
main (int argc, char **argv)
{
    int nr_polygons;

    /** seed the random number generator with the current time. */
    srand (time ());

    if (argc > 1)
    {
        nr_polygons = atoi (argv [1]);
    }
    else
    {
        nr_polygons = random_int_between (1, 100);
    }

    for (i = 0; i < nr_polygons; i ++)
        print_random_polygon ();

    /** the last line of the polygon list is to contain a 0, to indicate
     *  that there are no more polygons. */
    printf ("0\n");

    return 0;
}

/**********************************************************/

/**
 *  Choose a random nr_points, random polygo_id, and then a random list of
 *  points which traverse the perimeter in a clockwise direction. The
 *  nr_points, id and coordinates are then printed.
 */
    void
print_random_polygon (void)
{
    int nr_points, id;
    double x [MAX_POINTS], y [MAX_POINTS];

    nr_points = random_int_between (2, MAX_POINTS);
    id = rand ();

    do
        random_points (x, y, nr_points);
    while (!is_clockwise (x, y, nr_points));
}

/**********************************************************/

/**
 *  Returns a randomly generated int within the specified range, inclusive
 *  of the bounds.
 */
    int
random_int_between (int lower, int upper)
{
    int r = rand ();
    
    /** r is between 0 and RAND_MAX. Clamp it to be between 0 and the width
     *  of the range. */
    r %= upper - lower;
    r += lower;

    return r;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
