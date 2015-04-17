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
void print_row (const polygon_t *p);
void print_stage_3 (const polygon_t *max);
void set_perimeter_and_area (polygon_t *p);
double area_segment (const vector_t *start, const vector_t *end);
double distance_between (const vector_t *a, const vector_t *b);
double eccentricity (double perimeter, double area);
void copy_polygon (const polygon_t *src, polygon_t *dest);

/**********************************************************/

    int
main (int argc, char **argv)
{
    polygon_t current, max;

    /** Stage 1 is to read the first polygon and print out properties. */
    read_next_polygon (&current);
    print_stage_1 (&current);

    /** max for stage 3 can be initialised with the first polygon. */
    copy_polygon (&current, &max);

    /** Stage 2 is to step through all the polygons provided, and print out
     *  a table summarising the perimeter, area and eccentricity. */
    print_header ();
    print_row (&current);

    while (read_next_polygon (&current) != 0)
    {
        /** keep track of which polygon has the largest area for stage 3.
         *  If two polygons have the same area, the polygon with the
         *  smallest id is chosen as the max. */
        if (current.area >= max.area)
        {
            if (current.area != max.area || current.id < max.id)
                copy_polygon (&current, &max);
        }

        print_row (&current);
    }

    print_footer ();

    print_stage_3 (&max);

    return 0;
}

/**********************************************************/

/**
 *  Stage 1. Prints out the coordinates of the given polygon, and also the
 *  perimeter, area and eccentricity from the fields of the polygon struct.
 */
    void
print_stage_1 (const polygon_t *p)
{
    int i;

    printf ("Stage 1\n=======\n");
    printf ("First polygon is %d\n", p->id);
    printf ("   x_val   y_val\n");

    for (i = 0; i < p->npoints; i ++)
        printf (COORDINATE_FORMAT, p->vertices [i].x, p->vertices [i].y);

    printf ("perimeter      = %5.1f m\n", p->perimeter);
    printf ("area           = %5.2f m^2\n", p->area);
    printf ("eccentricity   = %5.2f\n", p->eccentricity);
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
    read_vertices (p->vertices, p->npoints);

    /** Set the perimeter, area and eccentricity fields in the struct. */
    set_perimeter_and_area (p);

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
print_row (const polygon_t *p)
{
    printf ("| %5d | %5d | %5.2f | %5.2f | %5.2f |\n", p->id, p->npoints, 
      p->perimeter, p->area, p->eccentricity);
}

/**********************************************************/

/**
 *  Print output for stage 3, consisting of the header and a list of the
 *  coordinates of the largest polygon.
 */
    void
print_stage_3 (const polygon_t *max)
{
    int i;

    printf ("\nStage 3\n=======\n");
    printf ("Largest polygon is %d\n", max->id);
    printf ("   x_val    y_val\n");

    for (i = 0; i < max->npoints; i ++)
    {
        printf (COORDINATE_FORMAT, max->vertices [i].x, 
          max->vertices [i].y);
    }
}

/**********************************************************/

/**
 *  Given a polygon with a list of points, calculate the perimeter, area
 *  and eccentricity and store them in the respective fields of the polygon
 *  struct.
 */
    void
set_perimeter_and_area (polygon_t *p)
{
    int i;

    p->perimeter = 0;
    p->area = 0;

    for (i = 1; i < p->npoints; i ++)
    {
        p->perimeter += distance_between (&(p->vertices [i - 1]), 
          &(p->vertices [i]));
        p->area += area_segment (&(p->vertices [i - 1]), 
          &(p->vertices [i]));
    }

    /** finally, handle the segment from the last point back to the
     *  first. */
    p->perimeter += distance_between (&(p->vertices [i]), 
      &(p->vertices [0]));
    p->area += area_segment (&(p->vertices [i]), &(p->vertices [0]));

    p->eccentricity = eccentricity (p->perimeter, p->area);
}

/**********************************************************/

/**
 *  Calculates the value to add to the total area for the segment between
 *  the line from start to end and the x axis. The value returned by
 *  this function may be positive or negative.
 */
    double
area_segment (const vector_t *start, const vector_t *end)
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

/**
 *  Copy the contents of the first polygon to the second one.
 */
    void
copy_polygon (const polygon_t *src, polygon_t *dest)
{
    int i;

    for (i = 0; i < src->npoints; i ++)
    {
        dest->vertices [i].x = src->vertices [i].x;
        dest->vertices [i].y = src->vertices [i].y;
    }

    dest->perimeter = src->perimeter;
    dest->area = src->area;
    dest->eccentricity = src->eccentricity;

    dest->npoints = src->npoints;
    dest->id = src->id;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
