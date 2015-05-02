/**
 *  Program to test if two strings are anagrams.
 */

#include <stdio.h>
#include <ctype.h>

/** alphanumeric chars are not in one contiguous block of ASCII chars, so
 *  we will set our array of counters to be of length 128. */
#define NUM_LETTERS     128
#define BUFFER_LENGTH   1024

/**********************************************************/

int is_anagram (const char *s1, const char *s2);
void add_to_counts (const char *string, int *counts, int value);
int identical (const char *s1, const char *s2);
int all_zeros (int *array, int length);

/**********************************************************/

    int
main (int argc, char **argv)
{
    char string1 [BUFFER_LENGTH], string2 [BUFFER_LENGTH];

    printf ("Enter two strings up to %d characters long, one per line:\n",
      BUFFER_LENGTH);

    fgets (string1, BUFFER_LENGTH, stdin);
    fgets (string2, BUFFER_LENGTH, stdin);

    printf ("strings are %s\n", is_anagram (string1, string2)? 
      "anagrams" : "not anagrams");

    return 0;
}

/**********************************************************/

/**
 *  Returns 1 if the two strings are anagrams, ie they have the same
 *  letters, and the same frequency of each letter. Only alphanumeric
 *  chars will be considered for the purpose of this function, and the
 *  strings may contain non alphanumeric chars.
 */
    int
is_anagram (const char *s1, const char *s2)
{
    int counts [NUM_LETTERS] = {0};

    /** identical strings are not anagrams */
    if (identical (s1, s2))
        return 0;

    /** count how many of each alphanumeric char occurs in the first
     *  string. */
    add_to_counts (s1, counts, 1);

    /** now subtract the chars that occur in the second string. If the two
     *  strings have the same letters with the same frequency, we will end
     *  up with all of the counts zeroed. */
    add_to_counts (s2, counts, -1);

    return all_zeros (counts, NUM_LETTERS);
}

/**********************************************************/

/**
 *  Goes through the given null terminated string char by char, and for
 *  each alphanumeric char, add the specified value to the corresponding
 *  element in the counts array.
 */
    void
add_to_counts (const char *string, int *counts, int value)
{
    int i = 0;

    while (string [i] != '\0')
    {
        if (isalnum (string [i]))
            counts [(int) string [i]] += value;

        i ++;
    }
}

/**********************************************************/

/**
 *  Test if two strings are the same, excluding non alphanumeric chars.
 */
    int
identical (const char *s1, const char *s2)
{
    int pos1 = 0, pos2 = 0;

    while ((s1 [pos1] != '\0') && (s2 [pos2] != '\0'))
    {
        /** skip over any non alphanumeric chars */
        if (!isalnum (s1 [pos1]))
        {
            pos1 ++;
            continue;
        }

        if (!isalnum (s2 [pos2]))
        {
            pos2 ++;
            continue;
        }

        /** do the strings differ? */
        if (s1 [pos1] != s2 [pos2])
            return 0;

        pos1 ++;
        pos2 ++;
    }

    return 1;
}

/**********************************************************/

/**
 *  Checks if all the values in a given array are zero. Returns 1 if all
 *  values are zero, 0 if not.
 */
    int
all_zeros (int *array, int length)
{
    int i;

    for (i = 0; i < length; i ++)
    {
        if (array [i] != 0)
            return 0;
    }

    return 1;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
