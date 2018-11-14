#include "mines.h"

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
            *p_minefield = 
                ((i < width) ? 0 : (i % width) ? 0 : (p_grid->minefield[i - width - 1] == MINE) ? 1 : 0)
                + ((i < width) ? 0 : (p_grid->minefield[i - width] == MINE) ? 1 : 0)
                + ((i < width) ? 0 : ((i + 1) % width == 0) ? 0 : (p_grid->minefield[i - width + 1] == MINE) ? 1 : 0)
                + ((i % width == 0) ? 0 : (p_grid->minefield[i - 1] == MINE) ? 1 : 0)
                + (((i + 1) % width == 0) ? 0 : (p_grid->minefield[i + 1] == MINE) ? 1 : 0)
                + ((i >= ((width * height) - width)) ? 0 : (i % width == 0) ? 0 : (p_grid->minefield[i + width -1] == MINE) ? 1 : 0)
                + ((i >= ((width * height) - width)) ? 0 : (p_grid->minefield[i + width] == MINE) ? 1 : 0)
                + ((i >= ((width * height) - width)) ? 0 : ((i + 1) % width == 0) ? 0 : (p_grid->minefield[i + width + 1] == MINE) ? 1 : 0);
        }
        ++p_minefield;
    }
}


void render_minefield(grid * p_grid) {
    int8_t * p_minefield = p_grid->minefield;
    int16_t minefield_size = p_grid->width * p_grid->height;

    for (int16_t i = 0; i < minefield_size; ++i) {
        /* Show cursor */
        if (i == p_grid->cursor) {
            attroff(COLOR_PAIR(FLAG));
            attron(COLOR_PAIR(CURSOR));
        }

        /* Compute pixel value */
        int32_t pixel = ACS_CKBOARD;
        if (*p_minefield & UNCOVERED) {
            if (*p_minefield & MINE) {
                pixel = ACS_DEGREE;
            } else {
                if (*p_minefield == EMPTY) {
                    pixel = ' ';
                } else {
                    pixel = '0' + (*p_minefield & VALUE);
                }
            }
        } else if (*p_minefield & FLAGGED) {
            pixel = ACS_LANTERN;
        }


        /* Render to screen */
        mvaddch(
            p_grid->y_offset + (i / p_grid->width),
            p_grid->x_offset + (2 * (i % p_grid->width)),
            pixel
        );

        /* Turn off highlighting */
        attroff(COLOR_PAIR(CURSOR));

        /* Increment minefield pointer */
        ++p_minefield;
    }
}

void render_mine(grid * p_grid, int16_t i, int8_t is_cursor) {
    /* Turn on cursor */
    if (is_cursor) {
        attron(COLOR_PAIR(CURSOR));
    }

    int8_t * p_minefield = (p_grid->minefield + i);

    /* Compute pixel value */
    int32_t pixel = ACS_CKBOARD;
    if (*p_minefield & UNCOVERED) {
        if (*p_minefield & MINE) {
            pixel = ACS_DEGREE;
        } else {
            if (*p_minefield == EMPTY) {
                pixel = ' ';
            } else {
                pixel = '0' + (*p_minefield & VALUE);
            }
        }
    } else if (*p_minefield & FLAGGED) {
        pixel = ACS_LANTERN;
    }

    mvaddch(
        p_grid->y_offset + (i / p_grid->width),
        p_grid->x_offset + (2 * (i % p_grid->width)),
        pixel
    );

    attroff(COLOR_PAIR(CURSOR));
    attroff(COLOR_PAIR(FLAG));
}

