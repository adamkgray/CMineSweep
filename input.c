#include "input.h"

int8_t input(grid * p_grid) {
    int16_t c = getch();
    switch (c) {
        /* DIG */
        case 'd':
            /* Reveal this space and, recursively, all the surrounding spaces that may be empty */
            reveal(p_grid, p_grid->cursor);

            /* Render the now uncovered space with the cursor on it */
            render_mine(p_grid, p_grid->cursor, 1);

            break;
        /* FLAG */
        case 'f':
            /* Flag if not uncovered */
            if (!(p_grid->minefield[p_grid->cursor] & UNCOVERED)) {
                p_grid->minefield[p_grid->cursor] |= FLAGGED;
                p_grid->flags += 1;
                render_mine(p_grid, p_grid->cursor, 0);
            }

            /* Unflag if flagged */
            if (p_grid->minefield[p_grid->cursor] & FLAGGED) {
                p_grid->minefield[p_grid->cursor] &= ~FLAGGED;
                p_grid->flags -= 1;
                render_mine(p_grid, p_grid->cursor, 1);
            }

            break;
        /* MOVEMENT */
        case 'h':
            if (p_grid->cursor % p_grid->width != 0) {
                render_mine(p_grid, p_grid->cursor, 0);
                p_grid->cursor -= 1;
                render_mine(p_grid, p_grid->cursor, 1);
            }
            break;
        case 'j':
            if (p_grid->cursor < ((p_grid->width * p_grid->height) - p_grid->width)) {
                render_mine(p_grid, p_grid->cursor, 0);
                p_grid->cursor += p_grid->width;
                render_mine(p_grid, p_grid->cursor, 1);
            }
            break;
        case 'k':
            if (p_grid->cursor >= p_grid->width) {
                render_mine(p_grid, p_grid->cursor, 0);
                p_grid->cursor -= p_grid->width;
                render_mine(p_grid, p_grid->cursor, 1);
            }
            break;
        case 'l':
            if ((p_grid->cursor + 1) % p_grid->width != 0) {
                render_mine(p_grid, p_grid->cursor, 0);
                p_grid->cursor += 1;
                render_mine(p_grid, p_grid->cursor, 1);
            }
            break;
        default:
            break;
    }

    return 1;
}
