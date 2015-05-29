/**
 *  Program to deal randomly generated poker hands, and provide an estimate
 *  of the probability of obtaining cards of the same face value in a
 *  hand.
 */

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define CARDS_PER_PACK      52
#define CARDS_PER_HAND      5

/**********************************************************/

typedef int card_t;

/**********************************************************/

void deal_hand (card_t *hand);
bool empty_pack (void);
void reset_pack (void);
void print_card (card_t card);

/**********************************************************/

/** this global keeps track of which cards have been dealt so far. */
bool dealt [CARDS_PER_PACK] = {0};

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
    int face_value = card % CARDS_PER_PACK;

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

    return (num_cards >= CARDS_PER_HAND)? true : false;
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
