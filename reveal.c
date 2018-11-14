#include "reveal.h"

void reveal(grid * p_grid, int16_t i) {
    /* Unflag */
    p_grid->minefield[i] &= ~FLAGGED;

    /* Uncover */
    p_grid->minefield[i] |= UNCOVERED;

    render_mine(p_grid, i, 0);
}
