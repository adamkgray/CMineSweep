#include "reveal.h"

void reveal(grid * p_grid, int16_t i) {
    /* Return if the space is already uncovered */
    if ((p_grid->minefield[i] & UNCOVERED) || (p_grid->minefield[i] & MINE)) {
        return;
    }

    /* Unflag */
    p_grid->minefield[i] &= ~FLAGGED;

    /* Uncover */
    p_grid->minefield[i] |= UNCOVERED;

    /* Render the uncovered mine (with no cursor) */
    render_mine(p_grid, i, 0);

    /* If the space was empty, reveal all surrounding spaces */
    if ((p_grid->minefield[i] & VALUE) == EMPTY) {
        /* Above */
        if (i - p_grid->width >= 0) {
            reveal(p_grid, i - p_grid->width);
        }
        /* Below */
        if (i + p_grid->width < p_grid->width * p_grid->height) {
            reveal(p_grid, i + p_grid->width);
        }
        /* Right */
        if ((i + 1) % p_grid->width != 0) {
            reveal(p_grid, i + 1);
        }
        /* Left */
        if (i % p_grid->width != 0) {
            reveal(p_grid, i - 1);
        }
    }
}
