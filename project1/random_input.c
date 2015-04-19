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

#define MAX_POINTS      100

/**********************************************************/

int random_int_between (int lower, int upper);
void print_random_polygon (void);
void random_points (double *x, double *y, int nr_points);
int is_clockwise (double *x, double *y, int nr_points);
double random_double (double lower, double upper);

/**********************************************************/

    int
main (int argc, char **argv)
{
    int nr_polygons, i;

    /** seed the random number generator with the current time. */
    srand (time (NULL));
    srand48 ((long int) time (NULL));

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
    int nr_points, id, i;
    double x [MAX_POINTS], y [MAX_POINTS];

    nr_points = random_int_between (2, 10);
    id = rand ();

    do
    {
        random_points (x, y, nr_points);
    }
    while (!is_clockwise (x, y, nr_points));

    printf ("%d %d  ", nr_points, id);

    for (i = 0; i < nr_points; i ++)
        printf ("%f %f  ", x [i], y [i]);

    printf ("\n");
}

/**********************************************************/

/**
 *  Generates the specified number of coordinates at random and stores them
 *  in the two respective arrays.
 */
    void
random_points (double *x, double *y, int nr_points)
{
    int i;

    for (i = 0; i < nr_points; i ++)
    {
        x [i] = random_double (-10, 10);
        y [i] = random_double (-10, 10);
    }
}

/**********************************************************/

/**
 *  Test if the points in the x and y arrays traverse the perimeter of
 *  the polygon in a clockwise direction. Return value is 1 if they are
 *  clockwise, 0 otherwise.
 */
    int
is_clockwise (double *x, double *y, int nr_points)
{
    double total = 0;
    int i;

    for (i = 1; i < nr_points; i ++)
        total += (x [i] - x [i - 1]) * (y [i] + y [i - 1]);

    total += (x [i - 1] - x [0]) * (y [i - 1] + y [0]);

    return (total > 0)? 1 : 0;
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

/**
 *  Returns a randomly generated double from the specified range, inclusive
 *  of the endpoints.
 */
    double
random_double (double lower, double upper)
{
    double d = drand48 ();

    /** same sort of thing as for random int. Note that since drand48
     *  returns a value between 0 and 1, we use multiplication rather
     *  than modulo division to map d to the range. */
    d *= upper - lower;
    d += lower;

    return d;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
