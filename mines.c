#include "mines.h"

void initialize_minefield(grid * p_grid) {
    srand(time(NULL));

    /* Fill minefield with mines */
    int8_t * p_minefield = p_grid->minefield;
    for (int16_t i; i < p_grid->width * p_grid->height; ++i) {
        *p_minefield = ((rand() & 1) & (rand() & 1) & (rand() & 1) & (rand() & 1)) ? MINE : EMPTY;
        if (*p_minefield & MINE) {
            p_grid->mines += 1;

            /* Above */
            if (i - p_grid->width >= 0 && rand() & 1) {
                *(p_minefield - p_grid->width) = MINE;
                p_grid->mines += 1;
            }

            /* To the left */
            if (i - 1 >= 0 && rand() & 1) {
                *(p_minefield - 1) = MINE;
                p_grid->mines += 1;
            }

            /* Above and to the left */
            if (i - p_grid->width - 1 >= 0 && rand() & 1) {
                *(p_minefield - p_grid->width - 1) = MINE;
                p_grid->mines += 1;
            }
        }
        ++p_minefield;
    }

    /* Calculate all other spaces */
    for (int16_t i; i < (p_grid->width * p_grid->height); ++i) {
        if (p_grid->minefield[i] == EMPTY) {
            mvprintw(1, 1, "THIS SHOULD BE PRINTED ON SCREEN");
            refresh();

            p_grid->minefield[i] = 
                ((i < p_grid->width) ? 0 : (i % p_grid->width) ? 0 : (p_grid->minefield[i - p_grid->width - 1] == MINE) ? 1 : 0)
                + ((i < p_grid->width) ? 0 : (p_grid->minefield[i - p_grid->width] == MINE) ? 1 : 0)
                + ((i < p_grid->width) ? 0 : ((i + 1) % p_grid->width == 0) ? 0 : (p_grid->minefield[i - p_grid->width + 1] == MINE) ? 1 : 0)
                + ((i % p_grid->width == 0) ? 0 : (p_grid->minefield[i - 1] == MINE) ? 1 : 0)
                + (((i + 1) % p_grid->width == 0) ? 0 : (p_grid->minefield[i + 1] == MINE) ? 1 : 0)
                + ((i >= ((p_grid->width * p_grid->height) - p_grid->width)) ? 0 : (i % p_grid->width == 0) ? 0 : (p_grid->minefield[i + p_grid->width -1] == MINE) ? 1 : 0)
                + ((i >= ((p_grid->width * p_grid->height) - p_grid->width)) ? 0 : (p_grid->minefield[i + p_grid->width] == MINE) ? 1 : 0)
                + ((i >= ((p_grid->width * p_grid->height) - p_grid->width)) ? 0 : ((i + 1) % p_grid->width == 0) ? 0 : (p_grid->minefield[i + p_grid->width + 1] == MINE) ? 1 : 0);
        }
    }
}


void render_minefield(grid * p_grid) {
    int8_t * p_minefield = p_grid->minefield;
    for (int16_t i = 0; i < p_grid->width * p_grid->height; ++i) {
        /* Turn on cursor */
        if (i == p_grid->cursor) {
            attron(COLOR_PAIR(CURSOR));
        }

        if ((*p_minefield & VALUE) > 0) {
            mvprintw(1, 1, "value geater than 0");
        }

        mvaddch(
            p_grid->y_offset + (i / p_grid->width),
            p_grid->x_offset + (2 * (i % p_grid->width)),
            (*p_minefield & MINE) ? 'm' : (*p_minefield & VALUE) + '0'
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

