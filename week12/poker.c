/**
 *  Program to deal randomly generated poker hands, and provide an estimate
 *  of the probability of obtaining cards of the same face value in a
 *  hand.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

#define CARDS_PER_PACK      52
#define CARDS_PER_SUIT      13
#define CARDS_PER_HAND      5
#define NUMPLAYERS          4
#define NUMGAMES            10000

/**********************************************************/

typedef int card_t;

/**********************************************************/

void deal_hand (card_t *hand);
bool empty_pack (void);
void reset_pack (void);
void print_card (card_t card);
void count_duplicates (card_t *hand, int *counts);
bool same_face_value (card_t c1, card_t c2);
bool full_house (card_t *hand);

/**********************************************************/

/** this global keeps track of which cards have been dealt so far. */
bool dealt [CARDS_PER_PACK] = {0};

/**********************************************************/

/**
 *  Program to deal four poker hands.
 */
    int
main (int argc, char **argv)
{
    int player, game, i;
    int counts [CARDS_PER_HAND] = {0};
    int full_house_hands = 0;
    card_t hand [CARDS_PER_HAND];

    /** if the user provides a seed as a command line parameter, use that,
     *  otherwise, use the current time. */
    if (argc != 2)
    {
        srand (time (NULL));
    }
    else
    {
        srand (atoi (argv [1]));
    }

    /** we will simulate the dealing of 10,000 4 player poker games, to
     *  see how many times hands contain multiple cards with the same
     *  face value.
     */
    for (game = 0; game < NUMGAMES; game ++)
    {
        /** deal cards */
        for (player = 0; player < NUMPLAYERS; player ++)
        {
            deal_hand (hand);
            count_duplicates (hand, counts);

            if (full_house (hand))
                full_house_hands += 1;
        }

        reset_pack ();
    }

    /** print out how many hands had duplicates, eg two of a kind or three
     *  of a kind. This will not count the number of hands with a full
     *  house. */
    for (i = 0; i < CARDS_PER_HAND; i ++)
        printf ("%d of a kind: %6d hands.\n", i + 1, counts [i]);

    printf ("Full house hands: %6d\n", full_house_hands);

    return 0;
}

/**********************************************************/

/**
 *  Count cards that repeat multiple times, and increment counters in
 *  the given array. The array counts single non repeated cards in the
 *  first element, pair in the second, triplets in the third element and
 *  so on.
 */
    void
count_duplicates (card_t *hand, int *counts)
{
    int count, i, j;
    bool counted [CARDS_PER_HAND] = {0};

    /** For each card in the hand, step through the other cards and see if
     *  any have the same face value. If any do, increment the count, and
     *  mark those cards as counted. */
    for (i = 0; i < CARDS_PER_HAND; i ++)
    {
        if (counted [i])
            continue;

        count = 0;
        counted [i] = true;

        for (j = i + 1; j < CARDS_PER_HAND; j ++)
        {
            if (same_face_value (hand [i], hand [j]))
            {
                count ++;
                counted [j] = true;
            }
        }

        counts [count] += 1;
    }
}

/**********************************************************/

/**
 *  Test if a hand is a full house, ie contains 3 of a kind and a pair.
 */
    bool
full_house (card_t *hand)
{
    int duplicates [CARDS_PER_HAND] = {0};
    count_duplicates (hand, duplicates);
    return (duplicates [2] == 1 && duplicates [1] == 1)? true : false;
}

/**********************************************************/

/**
 *  Test if two cards have the same face value.
 */
    bool
same_face_value (card_t c1, card_t c2)
{
    return (c1 % CARDS_PER_SUIT == c2 % CARDS_PER_SUIT)? true : false;
}

/**********************************************************/

/**
 *  deals a random poker hand. Cards are represented as an int between 0
 *  and 51, and this function keeps track of previously dealt cards to
 *  ensure that the same card cannot be dealt twice.
 */
    void
deal_hand (card_t *hand)
{
    int i;
    card_t card = rand () % CARDS_PER_PACK;

    /** check that we still have cards in the pack. */
    if (empty_pack ())
    {
        printf ("Can't deal cards from empty pack.\n");
        exit (1);
    }

    /** poker hands have 5 cards. */
    for (i = 0; i < CARDS_PER_HAND; i ++)
    {
        /** select a card, then check if it has already been dealt. If so,
         *  keep trying other cards until we find one that hasn't been
         *  dealt. */
        while (dealt [card])
            card = rand () % CARDS_PER_PACK;

        hand [i] = card;
        dealt [card] = true;
    }
}

/**********************************************************/

/**
 *  Print out the human readable representation of a card on stdout. The
 *  card is printed in the form <face value>-<suit> with no space padding,
 *  no commas and no newlines. Any such formatting should be added by the
 *  caller as required.
 */
    void
print_card (card_t card)
{
    const char *suits [] = {"H", "D", "C", "S"};
    const char *face_values [] = {"Ac", "2", "3", "4", "5", "6", "7", "8",
            "9", "10", "J", "K", "Q"};

    int suit = card / CARDS_PER_PACK;
    int face_value = card % CARDS_PER_SUIT;

    printf ("%s-%s", face_values [face_value], suits [suit]);
}

/**********************************************************/

/**
 *  Check if there are enough cards still in the pack to produce another
 *  hand. We need to have at least 5 cards.
 *
 *  Returns true if there are enough cards, false if not.
 */
    bool
empty_pack (void)
{
    int card, num_cards = 0;

    for (card = 0; card < CARDS_PER_PACK; card ++)
    {
        if (!dealt [card])
            num_cards ++;
    }

    return (num_cards < CARDS_PER_HAND)? true : false;
}

/**********************************************************/

/**
 *  Reset the dealt array so that all cards are again available to be
 *  dealt.
 */
    void
reset_pack (void)
{
    int card;

    for (card = 0; card < CARDS_PER_PACK; card ++)
        dealt [card] = false;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
