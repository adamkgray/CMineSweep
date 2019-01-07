#include "mines.h"

void render_mine(grid * p_grid, int16_t i, int8_t is_cursor) {
    /* Get value of space at i */
    int8_t space = p_grid->minefield[i];

    /* Turn on flag */
    if (space & FLAGGED) {
        attron(COLOR_PAIR(FLAG));
    }

    /* Turn on cursor */
    if (is_cursor) {
        /* Turn off flag if it's flagged */
        if (space & FLAGGED) {
            attroff(COLOR_PAIR(FLAG));
        }
        attron(COLOR_PAIR(CURSOR));
    }

    /* Compute pixel value (ncurses ACS need 32 bits) */
    int32_t pixel = ACS_CKBOARD;
    if (space & UNCOVERED) {
        if (space & MINE) {
            pixel = ACS_DEGREE;
        } else {
            if ((space & VALUE) == EMPTY) {
                pixel = ' ';
            } else {
                pixel = '0' + (space & VALUE);
            }
        }
    } else if (space & FLAGGED) {
        pixel = ACS_PLMINUS;
    }

    mvaddch(
        p_grid->y_offset + (i / p_grid->width),
        p_grid->x_offset + (2 * (i % p_grid->width)),
        pixel
    );

    /* Turn of flag */
    if ((space & FLAGGED) && !is_cursor) {
        attroff(COLOR_PAIR(FLAG));
    } else if (is_cursor) {
        attroff(COLOR_PAIR(CURSOR));
    }

}

void render_minefield(grid * p_grid) {
    int8_t * p_minefield = p_grid->minefield;
    for (int16_t i = 0; i < (p_grid->width * p_grid->height); ++i) {
        /* Render covered mine to screen */
        mvaddch(
            p_grid->y_offset + (i / p_grid->width),
            p_grid->x_offset + (2 * (i % p_grid->width)),
            ACS_CKBOARD
        );

        /* Increment minefield pointer */
        ++p_minefield;
    }

    /* Render cursor */
    attron(COLOR_PAIR(CURSOR));
    mvaddch(
        p_grid->y_offset + (p_grid->cursor / p_grid->width),
        p_grid->x_offset + (2 * (p_grid->cursor % p_grid->width)),
        ACS_CKBOARD
    );
    attroff(COLOR_PAIR(CURSOR));

}

void initialize_minefield(grid * p_grid) {
    srand(time(NULL));
    int8_t width = p_grid->width;
    int8_t height = p_grid->height;
    int16_t minefield_size = width * height;

    /* Fill minefield with mines */
    int8_t * p_minefield = p_grid->minefield;
    for (int16_t i = 0; i < minefield_size; ++i) {
        /* Space is default empty, but has 4 chances to become a MINE */
        *p_minefield = ((rand() & 1) && (rand() & 1) && (rand() & 1) && (rand() & 1)) ? MINE : EMPTY;
        /* Add some other mines around this mine */
        if (*p_minefield == MINE) {
            p_grid->mines += 1;
            /* Above */
            if (i - width >= 0 && (rand() & 1)) {
                if (*(p_minefield - width) != MINE) {
                    *(p_minefield - width) = MINE;
                    p_grid->mines += 1;
                }
            }
            /* To the left */
            if (i - 1 >= 0 && (rand() & 1)) {
                if( *(p_minefield - 1) != MINE) {
                    *(p_minefield - 1) = MINE;
                    p_grid->mines += 1;
                }
            }
            /* Above and to the left */
            if (i - width - 1 >= 0 && (rand() & 1)) {
                if (*(p_minefield - width - 1) != MINE) {
                    *(p_minefield - width - 1) = MINE;
                    p_grid->mines += 1;
                }
            }
        }
        ++p_minefield;
    }

    /* Calculate all other spaces */
    p_minefield = p_grid->minefield;
    for (int16_t i = 0; i < minefield_size; ++i) {
        if (!(*p_minefield)) {
            /* Upper left */
            if ((i >= width) && (i % width != 0) && (p_grid->minefield[i - width - 1] == MINE)) {
                ++(*p_minefield);
            }

            /* Above */
            if ((i >= width) && (p_grid->minefield[i - width] == MINE)) {
                ++(*p_minefield);
            }

            /* Upper right */
            if ((i >= width) && ((i + 1) % width != 0) && (p_grid->minefield[i - width + 1] == MINE)) {
                ++(*p_minefield);
            }

            /* Left */
            if ((i % width != 0) && (p_grid->minefield[i - 1] == MINE)) {
                ++(*p_minefield);
            }

            /* Right */
            if (((i + 1) % width != 0) && (p_grid->minefield[i + 1] == MINE)) {
                ++(*p_minefield);
            }

            /* Bottom left */
            if ((i < (minefield_size - width)) && (i % width != 0) && (p_grid->minefield[i + width - 1] == MINE)) {
                ++(*p_minefield);
            }

            /* Below */
            if ((i < (minefield_size - width)) && (p_grid->minefield[i + width] == MINE)) {
                ++(*p_minefield);
            }

            /* Bottom right */
            if ((i < (minefield_size - width)) && ((i + 1) % width != 0) && (p_grid->minefield[i + width + 1] == MINE)) {
                ++(*p_minefield);
            }
        }
        ++p_minefield;
    }
}

