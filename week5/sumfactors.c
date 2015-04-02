
#include "sumfactors.h"


/**
 *  Returns the sum of the factors of the argument.
 */
    int
sumfactors (int number)
{
    int sum = 1, divisor;

    for (divisor = 2; divisor * divisor <= number; divisor ++)
    {
        /** if divisor goes evenly into number, we have found two factors,
         *  divisor and number / divisor, since the product of the two
         *  equals number. */
        if (number % divisor == 0)
        {
            sum += divisor;
            sum += number / divisor;
        }
    }

    return sum;
}

/** vim: set ts=4 sw=4 et : */
