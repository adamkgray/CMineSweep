#include "mines.h"

void initialize_minefield(grid * p_grid) {
    srand(time(NULL));
    int8_t width = p_grid->width;
    int8_t height = p_grid->height;
    int16_t minefield_size = width * height;

    /* Fill minefield with mines */
    int8_t * p_minefield = p_grid->minefield;
    for (int16_t i; i < minefield_size; ++i) {
        /* Space is default empty, but has 4 chances to become a MINE */
        *p_minefield = ((rand() & 1) && (rand() & 1) && (rand() & 1) && (rand() & 1)) ? MINE : EMPTY;

        if (*p_minefield & MINE) {
            p_grid->mines += 1;

            /* Above */
            if (i - p_grid->width >= 0 && (rand() & 1)) {
                *(p_minefield - width) = MINE;
                p_grid->mines += 1;
            }

            /* To the left */
            if (i - 1 >= 0 && (rand() & 1)) {
                *(p_minefield - 1) = MINE;
                p_grid->mines += 1;
            }

            /* Above and to the left */
            if (i - width - 1 >= 0 && (rand() & 1)) {
                *(p_minefield - width - 1) = MINE;
                p_grid->mines += 1;
            }
        }
        ++p_minefield;
    }

    /* Calculate all other spaces */
    for (int16_t i; i < minefield_size; ++i) {
        if (p_grid->minefield[i] == EMPTY) {
            p_grid->minefield[i] = 
                ((i < width) ? 0 : (i % width) ? 0 : (p_grid->minefield[i - width - 1] & MINE) ? 1 : 0)
                + ((i < width) ? 0 : (p_grid->minefield[i - width] & MINE) ? 1 : 0)
                + ((i < width) ? 0 : ((i + 1) % width == 0) ? 0 : (p_grid->minefield[i - width + 1] & MINE) ? 1 : 0)
                + ((i % width == 0) ? 0 : (p_grid->minefield[i - 1] & MINE) ? 1 : 0)
                + (((i + 1) % width == 0) ? 0 : (p_grid->minefield[i + 1] & MINE) ? 1 : 0)
                + ((i >= ((width * height) - width)) ? 0 : (i % width == 0) ? 0 : (p_grid->minefield[i + width -1] & MINE) ? 1 : 0)
                + ((i >= ((width * height) - width)) ? 0 : (p_grid->minefield[i + width] & MINE) ? 1 : 0)
                + ((i >= ((width * height) - width)) ? 0 : ((i + 1) % width == 0) ? 0 : (p_grid->minefield[i + width + 1] & MINE) ? 1 : 0);
        }
    }
}


void render_minefield(grid * p_grid) {
    static int16_t p = 0;

    int8_t * p_minefield = p_grid->minefield;
    int16_t minefield_size = p_grid->width * p_grid->height;

    for (int16_t i = 0; i < minefield_size; ++i) {
        /* Turn on cursor */
        if (i == p_grid->cursor) {
            attron(COLOR_PAIR(CURSOR));
        }

        mvaddch(
            p_grid->y_offset + (i / p_grid->width),
            p_grid->x_offset + (2 * (i % p_grid->width)),
            (*p_minefield & MINE) ? 'm' : 'a' + (p_grid->minefield[i]) 
        );

        attroff(COLOR_PAIR(CURSOR));
        attroff(COLOR_PAIR(FLAG));
        ++p_minefield;
    }
}

void render_mine(grid * p_grid, int16_t i, int8_t is_cursor) {
    if (is_cursor) {
        attron(COLOR_PAIR(CURSOR));
    }

    /* Compute sprite value */
    int32_t sprite = ACS_CKBOARD;

    if (p_grid->minefield[i] & FLAGGED) {
        /* Turn off cursor, turn on flag */
        attroff(COLOR_PAIR(CURSOR));
        attron(COLOR_PAIR(FLAG));
        sprite = ACS_LANTERN;
    }

    if (p_grid->minefield[i] & UNCOVERED) {
        if (p_grid->minefield[i] & MINE) {
            sprite = ACS_DEGREE;
        }

        if (p_grid->minefield[i] & VALUE) {
            mvaddch(0, 0, (p_grid->minefield[i] & VALUE) + '0');
            sprite = (p_grid->minefield[i] & VALUE) + '0';
        } else {
            sprite = ' ';
        }
    }

    mvaddch(
        p_grid->y_offset + (i / p_grid->width),
        p_grid->x_offset + (2 * (i % p_grid->width)),
        sprite
    );

    attroff(COLOR_PAIR(CURSOR));
    attroff(COLOR_PAIR(FLAG));
}

