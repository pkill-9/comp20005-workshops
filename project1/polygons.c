/**
 *  Project 1 of COMP20005 Engineering Computation. Program that reads a
 *  sequence of polygons from stdin and calculates the perimeter, area and
 *  eccentricity of each one. Polygons are described by the coordinates of
 *  each vertex, and are not necessarily regular.
 */

#include <stdio.h>
#include <math.h>

#define PI              3.14159265358979323846

/**********************************************************/

void do_stage_1 (double *perimeter, double *area, int *id, int *nvals);
void do_stage_2 (int id, int nvals, double perimeter, double area);
void print_header (void);
void print_footer (void);
void print_row (int id, int nvals, double perimeter, double area);
void read_next_polygon (int *id, int *nvals, double *perimeter, double *area);
double next_area_segment (double x1, double y1, double x2, double y2);
double distance_between (double x1, double y1, double x2, double y2);
double eccentricity (double perimeter, double area);

/**********************************************************/

    int
main (int argc, char **argv)
{
    double perimeter, area;
    int polygon_id, num_vertices;

    do_stage_1 (&perimeter, &area, &polygon_id, &num_vertices);
    do_stage_2 (polygon_id, num_vertices, perimeter, area);

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
    printf ("\nStage 1\n=======\n");
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
    printf ("eccentricity   = %2.2f\n", eccentricity (*perimeter, *area));
}

/**********************************************************/

/**
 *  Stage 2 of the project. This function will read the vertex coords for
 *  the rest of the polygons and summarise them with a table of values.
 *
 *  The params for this function are the id, nvals perimeter and area of
 *  the first polygon, which was read by stage 1.
 */
    void
do_stage_2 (int id, int nvals, double perimeter, double area)
{
    print_header ();

    while (nvals != 0)
    {
        /** first print the row in the summary for the previous polygon,
         *  then read the next set of vertices. */
        print_row (id, nvals, perimeter, area);
        read_next_polygon (&id, &nvals, &perimeter, &area);
    }

    print_footer ();
}

/**********************************************************/

/**
 *  Prints the table header for stage 2.
 */
    void
print_header (void)
{
    printf ("\nStage 2\n=======\n");
    printf ("+-------+-------+-------+-------+-------+\n");
    printf ("|    id |  nval | perim |  area | eccen |\n");
    printf ("+-------+-------+-------+-------+-------+\n");
}

/**********************************************************/

/**
 *  Prints the table footer for stage 2.
 */
    void
print_footer (void)
{
    printf ("+-------+-------+-------+-------+-------+\n");
}

/**********************************************************/

/**
 *  This function prints out a single row of the summary table for stage 2
 *  of the project. Params are the id, nvals, perimeter and area of the
 *  polygon; eccentricity can be calculated from the perimeter and area.
 */
    void
print_row (int id, int nvals, double perimeter, double area)
{
    printf ("| %5d | %5d | %5.2f | %5.2f | %5.2f |\n", id, nvals, perimeter,
      area, eccentricity (perimeter, area));
}

/**********************************************************/

/**
 *  Reads the next polygon from stdin and stores it's id, number of
 *  vertices, perimeter and area in the variables pointed to by the
 *  corresponding parameters to this function.
 */
    void
read_next_polygon (int *id, int *nvals, double *perimeter, double *area)
{
    int i;
    double xi, yi;
    double x1, y1, x2, y2;

    *perimeter = 0;
    *area = 0;

    /** if there are no more polygons to be read, we will get a single line
     *  with nvals of 0 and no polygon id */
    if (scanf ("%d %d", nvals, id) != 2)
    {
        *nvals = 0;
        return;
    }

    scanf ("%lf %lf", &xi, &yi);
    x1 = xi;
    y1 = yi;

    for (i = 1; i < *nvals; i ++)
    {
        scanf ("%lf %lf", &x2, &y2);
        *perimeter += distance_between (x1, y1, x2, y2);
        *area += next_area_segment (x1, y1, x2, y2);

        x1 = x2;
        y1 = y2;
    }

    *perimeter += distance_between (x2, y2, xi, yi);
    *area += next_area_segment (x2, y2, xi, yi);
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

/**
 *  Calculate eccentricity of a polygon, defined as the square of the
 *  perimeter divided by the area, divided by 4 pi.
 */
    double
eccentricity (double perimeter, double area)
{
    return (perimeter * perimeter) / area / (4.0 * PI);
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
