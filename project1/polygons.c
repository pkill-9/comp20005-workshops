/**
 *  Project 1 of COMP20005 Engineering Computation. Program that reads a
 *  sequence of polygons from stdin and calculates the perimeter, area and
 *  eccentricity of each one. Polygons are described by the coordinates of
 *  each vertex, and are not necessarily regular.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI                      3.14159265358979323846

/** flag to indicate to read_next_polygon if output is to be printed for 
 *  stage 1. */
#define STAGE_1                 1

/** format string for the list of coordinates for stage 1. */
#define COORDINATE_FORMAT       "%7.1f %8.1f\n"

/** According to the project spec, each polygon contains no more than 100
 *  vertices */
#define MAX_VERTICES            100

/**********************************************************/

void read_next_polygon (int is_stage_1);
void print_header (void);
void print_footer (void);
void print_row (int id, int nvals, double perimeter, double area);
void print_stage_3 (int max_id, double *xvals, double *yvals, int count);
double next_area_segment (double x1, double y1, double x2, double y2);
double distance_between (double x1, double y1, double x2, double y2);
double eccentricity (double perimeter, double area);
void copy_global_coords_to (double *dest_xvals, double *dest_yvals, int count);

/**********************************************************/

/** 
 *  global variables for the perimeter, area, id and number of vertices of
 *  the last polygon that was read in. Global variables are best avoided,
 *  but in this case they provide a way of reducing the ammount of
 *  duplicated code.
 */
double saved_perimeter, saved_area;
int saved_id, saved_nvals;
double saved_xvals [MAX_VERTICES], saved_yvals [MAX_VERTICES];

/**********************************************************/

    int
main (int argc, char **argv)
{
    double biggest_xvals [MAX_VERTICES], biggest_yvals [MAX_VERTICES];
    double max_area = 0;
    int max_id, max_nvals;

    /** read the first polygon for stage 1. */
    read_next_polygon (STAGE_1);

    print_header ();

    while (saved_nvals != 0)
    {
        /** first print the row in the summary for the previous polygon,
         *  then read the next set of vertices. */
        print_row (saved_id, saved_nvals, saved_perimeter, saved_area);
        read_next_polygon (!STAGE_1);

        /** check if this polygon has the largest area. If so, save it's 
         *  list points. */
        if (saved_area >= max_area)
        {
            /** in cases of area ties, the polygon with the smallest id is
             *  chosen. */
            if (saved_area == max_area && max_id < saved_id)
                continue;

            max_area = saved_area;
            max_id = saved_id;
            max_nvals = saved_nvals;
            copy_global_coords_to (biggest_xvals, biggest_yvals, max_nvals);
        }
    }

    print_footer ();

    /** Stage 3. Print out the coordinates of the polygon with the largest 
     *  area. */
    print_stage_3 (max_id, biggest_xvals, biggest_yvals, max_nvals);

    return 0;
}

/**********************************************************/

/**
 *  Reads the next polygon from stdin and stores it's id, number of
 *  vertices, perimeter and area in the four corresponding global 
 *  variables.
 *
 *  The param is a flag to indicate whether this function is to print
 *  stage 1 output, consisting of a list of the vertex coordinates.
 */
    void
read_next_polygon (int is_stage_1)
{
    int i;
    double xi, yi;
    double x1, y1, x2, y2;

    saved_perimeter = 0;
    saved_area = 0;

    /** if there are no more polygons to be read, we will get a single line
     *  with nvals of 0 and no polygon id */
    if (scanf ("%d %d", &saved_nvals, &saved_id) != 2)
        return;

    /** check that nvals is within the bounds given in the spec. */
    if (saved_nvals > MAX_VERTICES)
    {
        printf ("Error: Too many vertices\n");
        exit (1);
    }

    scanf ("%lf %lf", &xi, &yi);
    x1 = xi;
    y1 = yi;

    if (is_stage_1)
    {
        printf ("\nStage 1\n=======\n");
        printf ("First polygon is %d\n", saved_id);
        printf ("   x_val    y_val\n");
        printf (COORDINATE_FORMAT, xi, yi);
    }

    for (i = 1; i < saved_nvals; i ++)
    {
        scanf ("%lf %lf", &x2, &y2);
        saved_perimeter += distance_between (x1, y1, x2, y2);
        saved_area += next_area_segment (x1, y1, x2, y2);

        if (is_stage_1)
            printf (COORDINATE_FORMAT, x2, y2);

        saved_xvals [i - 1] = x1;
        saved_yvals [i - 1] = y1;

        x1 = x2;
        y1 = y2;
    }

    saved_xvals [i - 1] = x2;
    saved_yvals [i - 1] = y2;

    /** lastly, add the section of perimeter and the area segment for the
     *  edge between the final point and the first point, closing the
     *  loop. */
    saved_perimeter += distance_between (x2, y2, xi, yi);
    saved_area += next_area_segment (x2, y2, xi, yi);
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
 *  Print output for stage 3, consisting of the header and a list of the
 *  coordinates of the largest polygon.
 */
    void
print_stage_3 (int id, double *xvals, double *yvals, int count)
{
    int i;

    printf ("\nStage 3\n=======\n");
    printf ("Largest polygon is %d\n", id);
    printf ("   x_val    y_val\n");

    for (i = 0; i < count; i ++)
        printf (COORDINATE_FORMAT, xvals [i], yvals [i]);
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

/**
 *  Copy the contents of the xval and yval arrays to specified locations.
 */
    void
copy_global_coords_to (double *dest_xvals, double *dest_yvals, int count)
{
    int i;

    for (i = 0; i < count; i ++)
    {
        dest_xvals [i] = saved_xvals [i];
        dest_yvals [i] = saved_yvals [i];
    }
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
