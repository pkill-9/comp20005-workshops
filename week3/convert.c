/**
 *  Program to convert values between different units. The input consists
 *  of a floating point number followed by a single character for the unit.
 *  The program will then convert the value to a single alternative unit.
 *
 *  Supported units:
 *      temperature: C (Celsius) and F (Farenheit)
 *      distance: M (miles) and K (kilometers)
 *      mass: P (pounds) G (kilograms)
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

    int
main (int argc, char **argv)
{
    double input;
    char units;

    printf ("Enter a quantity:\n");

    if (scanf ("%lf%c", &input, &units) != 2)
    {
        printf ("Enter a number followed by a unit\n");
        exit (1);
    }

    switch (toupper (units))
    {
    case 'C':
        /** Celsius to Farenheit */
        printf ("%.3f F\n", input * 9 / 5 + 32);
        break;

    case 'F':
        /** Farenheit to Celsius. Careful of integer division here */
        printf ("%.3f C\n", 5.0 * (input - 32) / 9);
        break;

    case 'M':
        /** Miles to kilometers */
        printf ("%.3f km\n", input * 1.609);
        break;

    case 'K':
        /** kilometers to miles */
        printf ("%.3f miles\n", input / 1.609);
        break;

    case 'P':
        /** pounds to kilograms */
        printf ("%.3f kg\n", input * 0.454);
        break;

    case 'G':
        /** kilograms to pounds */
        printf ("%.3f lbs\n", input / 0.454);
        break;

    default:
        printf ("\"%c\": Unknown unit.\n", units);
    }

    return 0;
}

/** vim: set ts=4 sw=4 et : */
