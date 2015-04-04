/**
 *  Project 1 of COMP20005 Engineering Computation. Program that reads a
 *  sequence of polygons from stdin and calculates the perimeter, area and
 *  eccentricity of each one. Polygons are described by the coordinates of
 *  each vertex, and are not necessarily regular.
 */

#include <stdio.h>
#include <math.h>

/**********************************************************/

void do_stage_1 (double *perimeter, double *area, int *id, int *nvals);
double next_area_segment (double x1, double y1, double x2, double y2);
double distance_between (double x1, double y1, double x2, double y2);

/**********************************************************/

    int
main (int argc, char **argv)
{
    double perimeter, area;
    int polygon_id, num_vertices;

    do_stage_1 (&perimeter, &area, &polygon_id, &num_vertices);

    return 0;
}

/**********************************************************/

/**
 *  This function is responsible for stage 1 of the project. It will
 *  print out the points and calculate perimeter and area of the first
 *  polygon. The perimeter, area, id and number of vertices will also
 *  be stored in the variables pointed to by this functions parameters.
 *
 *  Yay, pointers 3:)
 */
    void
do_stage_1 (double *perimeter, double *area, int *id, int *nvals)
{
    int i;

    /** coordinates of the first vertex. */
    double xi, yi;

    /** coordinates of two end points of an edge of the polygon */
    double x1, y1, x2, y2;

    *perimeter = 0;
    *area = 0;

    scanf ("%d %d %lf %lf", nvals, id, &xi, &yi);

    /** Stage 1. Read the first polygon and calculate the perimeter and
     *  area. */
    printf ("Stage 1\n=======\n");
    printf ("First polygon is %d\n", *id);
    printf ("   x_val    y_val\n");
    printf ("%4.1f %4.1f\n", xi, yi);
    x1 = xi;
    y1 = yi;

    for (i = 1; i < *nvals; i ++)
    {
        scanf ("%lf %lf", &x2, &y2);
        printf ("%4.1f %4.1f\n", x2, y2);
        *perimeter += distance_between (x1, y1, x2, y2);
        *area += next_area_segment (x1, y1, x2, y2);

        /** endpoint of the current edge section becomes the start point
         *  of the next section. */
        x1 = x2;
        y1 = y2;
    }

    /** Final edge section is from the final vertex back to the initial
     *  vertex, closing the loop of the perimeter. */
    *perimeter += distance_between (x2, y2, xi, yi);
    *area += next_area_segment (x1, y1, xi, yi);

    printf ("perimeter      = %2.2f m\n", *perimeter);
    printf ("area           = %2.2f m^2\n", *area);
    printf ("eccentricity   = unknown\n");
}

/**********************************************************/

/**
 *  Calculates the value to add to the total area for the segment between
 *  the line from x1,y1 to x2,y2 and the x axis. The value returned by
 *  this function may be positive or negative.
 */
    double
next_area_segment (double x1, double y1, double x2, double y2)
{
    /** The edge of the polygon and the x axis form the boundaries of a
     *  trapezoid of area, where a and b are the two y values, and h is
     *  the difference in x values. A negative h indicates that the
     *  trapezoid is outside the polygon, and should be subtracted from
     *  the subtotal. */
    return 0.5 * (y1 + y2) * (x2 - x1);
}

/**********************************************************/

/**
 *  Returns the distance between the two points (x1, y1) and (x2, y2).
 */
    double
distance_between (double x1, double y1, double x2, double y2)
{
    double distance;
    distance = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    distance = sqrt (distance);
    return distance;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
