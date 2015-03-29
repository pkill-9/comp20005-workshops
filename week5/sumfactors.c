
#include "sumfactors.h"


/**
 *  Returns the sum of the factors of the argument.
 */
    int
sumfactors (int number)
{
    int sum = 0, divisor;

    for (divisor = 1; divisor < number; divisor ++)
    {
        if (number % divisor == 0)
            sum += divisor;
    }

    return sum;
}

/** vim: set ts=4 sw=4 et : */
