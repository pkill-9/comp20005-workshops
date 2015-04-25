
int all_zero (int a[], int n);


/**
 *  Function that returns true if all of the elements of the given array
 *  are zero.
 */
    int
all_zero (int a[], int n)
{
    int i;

    for (i = 0; i < n; i ++)
    {
        if (a [i] != 0)
            return 0;
    }

    return 1;
}

/** vim: set ts=4 sw=4 et : */
