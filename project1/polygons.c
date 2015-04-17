/**
 *  Project 1 of COMP20005 Engineering Computation. Program that reads a
 *  sequence of polygons from stdin and calculates the perimeter, area and
 *  eccentricity of each one. Polygons are described by the coordinates of
 *  each vertex, and are not necessarily regular.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

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

/**
 *  Polygons are described by a sequence of coordinates in the two
 *  dimensional cartesian plane.
 */
typedef struct
{
    double x, y;
}
vector_t;

/**
 *  A polygon itself has a list of vertices (corners) and also properties
 *  of perimeter and area.
 */
typedef struct
{
    vector_t vertices [MAX_VERTICES];
    double perimeter, area, eccentricity;
    int npoints, id;
}
polygon_t;

/**********************************************************/

int read_next_polygon (polygon_t *p);
void read_vertices (vector_t *buffer, int num_vertices);
void print_stage_1 (const polygon_t *p);
void print_header (void);
void print_footer (void);
void print_row (int id, int nvals, double perimeter, double area);
void print_stage_3 (int max_id, double *xvals, double *yvals, int count);
double calculate_perimeter (const polygon_t *p);
double calculate_area (const polygon_t *p)
double next_area_segment (vector_t *start, vector_t *end);
double distance_between (const vector_t *a, const vector_t *b);
double eccentricity (double perimeter, double area);

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
 *  Reads the next polygon data from stdin, and store the details in the
 *  struct pointed to by p.
 *
 *  If there are no more polygons to read, this function returns 0,
 *  otherwise it returns 1.
 */
    int
read_next_polygon (polygon_t *p)
{
    /** First, read the number of points, and the id. */
    if (scanf ("%d %d", &(p->npoints), &(p->id)) != 2)
        return 0;
    
    /** next, read all of the vertices into the array inside the polygon
     *  struct. */
    read_vertices (&(p->vertices), p->npoints);

    /** Set the perimeter, area and eccentricity fields in the struct. */
    p->perimeter = calculate_perimeter (p);
    p->area = calculate_area (p);
    p->eccentricity = calculate_eccentricity (p);

    return 1;
}

/**********************************************************/

/**
 *  Read a given number of pairs of x and y coordinates from stdin and
 *  store them in the given array.
 */
    void
read_vertices (vector_t *buffer, int num_vertices)
{
    int i;

    assert (num_vertices <= MAX_VERTICES);

    for (i = 0; i < num_vertices; i ++)
        scanf ("%lf %lf", &(buffer [i].x), &(buffer [i].y));
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
 *  Given a polygon struct, calculate the perimeter from the array of
 *  vertex coordinates. For this function, it does not matter if the struct
 *  does not have the perimeter field correctly set.
 */
    double
calculate_perimeter (const polygon_t *p)
{
    int i;
    double perimeter = 0;

    for (i = 1; i < p->npoints; i ++)
    {
        perimeter += distance_between (p->vertices [i], 
          p->vertices [i - 1]);
    }

    return perimeter;
}

/**********************************************************/

/**
 *  Calculates the value to add to the total area for the segment between
 *  the line from start to end and the x axis. The value returned by
 *  this function may be positive or negative.
 */
    double
next_area_segment (vector_t *start, vector_t *end)
{
    /** The edge of the polygon and the x axis form the boundaries of a
     *  trapezoid of area, where a and b are the two y values, and h is
     *  the difference in x values. A negative h indicates that the
     *  trapezoid is outside the polygon, and should be subtracted from
     *  the subtotal. */
    return 0.5 * (start->y + end->y) * (end->x - start->x);
}

/**********************************************************/

/**
 *  Returns the distance between the two points (x1, y1) and (x2, y2).
 */
    double
distance_between (const vector_t *a, const vector_t *b)
{
    double distance;
    distance = (b->x - a->x) * (b->x - a->x) + 
        (b->y - a->y) * (b->y - a->y);
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
