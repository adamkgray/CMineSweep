#include "input.h"

int8_t end_conditions(grid * p_grid) {
    /* Number of spaces actually uncovered */
    int16_t actual = 0;
    for (int16_t i = 0; i < p_grid->width * p_grid->height; ++i) {
        if (p_grid->minefield[i] & UNCOVERED) {
            /* Increment actual for each uncovered space */
            ++actual;
        }
    }
    /* return the comparison between the number of spaces actually uncovered with the number of spaces needed for win */
    return (actual == (p_grid->width * p_grid->height) - p_grid->mines) ? 1 : 0;
}

int8_t input(grid * p_grid) {
    int16_t c = getch();
    switch (c) {
        /* QUIT */
        case 'q':
            return 0;
        /* DIG */
        case 'd':
            /* If the space was a mine, you lose haha */
            if (p_grid->minefield[p_grid->cursor] & MINE) {
                /* Unflag */
                p_grid->minefield[p_grid->cursor] &= ~FLAGGED;
                /* Uncover */
                p_grid->minefield[p_grid->cursor] |= UNCOVERED;
                /* Render the uncovered mine */
                render_mine(p_grid, p_grid->cursor, 1);
                /* Too bad so sad */
                mvprintw(
                    p_grid->y_offset + p_grid->height + 3,
                    p_grid->x_offset,
                    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
                );
                /* Wait for any input */
                getch();
                /* QUIT */
                return 0;
            }
            /* Otherwise, reveal this space and, recursively, all the surrounding spaces that may be empty */
            reveal(p_grid, p_grid->cursor);
            /* Render the now uncovered space with the cursor on it */
            render_mine(p_grid, p_grid->cursor, 1);
            break;
        /* FLAG */
        case 'f':
            /* Only flag spaces that are covered and not flagged */
            if (!(p_grid->minefield[p_grid->cursor] & UNCOVERED) && !(p_grid->minefield[p_grid->cursor] & FLAGGED)) {
                p_grid->minefield[p_grid->cursor] |= FLAGGED;
                p_grid->flags += 1;
                render_mine(p_grid, p_grid->cursor, 0);
                render_menu(p_grid);
            /* Only unflag spaces that are covered and flagged */
            } else if (!(p_grid->minefield[p_grid->cursor] & UNCOVERED) && (p_grid->minefield[p_grid->cursor] & FLAGGED)) {
                p_grid->minefield[p_grid->cursor] &= ~FLAGGED;
                p_grid->flags -= 1;
                render_mine(p_grid, p_grid->cursor, 1);
                render_menu(p_grid);
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
    /* Check for end conditions */
    if (end_conditions(p_grid)) {
        /* Wait for any input */
        mvprintw(
                p_grid->y_offset + p_grid->height + 3,
                p_grid->x_offset,
                "WINNER WINNER WINNER WINNER"
                );
        getch();
        /* QUIT */
        return 0;
    }
    return 1;
}
