/**
 *  Project 1 of COMP20005 Engineering Computation. Program that reads a
 *  sequence of polygons from stdin and calculates the perimeter, area and
 *  eccentricity of each one. Polygons are described by the coordinates of
 *  each vertex, and are not necessarily regular.
 */

#include <stdio.h>
#include <math.h>

/**********************************************************/

double distance_between (double x1, double y1, double x2, double y2);

/**********************************************************/

    int
main (int argc, char **argv)
{
    /** coordinates of the first vertex. */
    double xi, yi;

    /** coordinates of two end points of an edge of the polygon */
    double x1, y1, x2, y2;
    double perimeter = 0;
    int i, polygon_id, num_vertices;

    /** Stage 1. Read the first polygon and calculate the perimeter and
     *  area. */
    printf ("Stage 1\n=======\n");
    scanf ("%d %d", &num_vertices, &polygon_id);

    printf ("First polygon is %d\n", polygon_id);

    scanf ("%lf %lf", &xi, &yi);
    printf ("   x_val    y_val\n");
    printf ("%4.1f %4.1f\n", xi, yi);
    x1 = xi;
    y1 = yi;

    for (i = 1; i < num_vertices; i ++)
    {
        scanf ("%lf %lf", &x2, &y2);
        printf ("%4.1f %4.1f\n", x2, y2);
        perimeter += distance_between (x1, y1, x2, y2);

        /** endpoint of the current edge section becomes the start point
         *  of the next section. */
        x1 = x2;
        y1 = y2;
    }

    /** Final edge section is from the final vertex back to the initial
     *  vertex, closing the loop of the perimeter. */
    perimeter += distance_between (x2, y2, xi, yi);

    printf ("perimeter      = %2.2f m\n", perimeter);
    printf ("area           = unknown m^2\n");
    printf ("eccentricity   = unknown\n");

    return 0;
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
