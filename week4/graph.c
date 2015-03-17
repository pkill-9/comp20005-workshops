/**
 *  Program that draws a simple bar graph in the terminal, with each bar
 *  drawn with a line of asterisks. Exercise 4.5 in the book.
 */

#include <stdio.h>


    int
main (int argc, char **argv)
{
    int value;

    while (scanf ("%d", &value) == 1)
    {
        /** Check that the value is between 1 and 70. The reason for this
         *  choice of limits is because terminals are by default only 80
         *  columns wide. If we print more than 80 characters (including
         *  the axis down the left hand side) then the row of asterisks
         *  will wrap over onto the next line which looks ugly. */
        if (value < 1 || value > 70)
        {
            printf ("Only supports values between 1 and 70\n");
            continue; /** no need to abort the program */
        }

        /** print the axis and label first */
        printf ("%d |", value);

        /** now print a line of asterisks of length specified by value */
        for ( ; value > 0; value --)
            printf ("*");

        printf ("\n");
    }

    return 0;
}

/** vim: set ts=4 sw=4 et : */
