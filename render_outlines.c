#include "render_outlines.h"

void render_outlines(grid * p_grid) {
    /* Corners */
    mvaddch(p_grid->y_offset - 1, p_grid->x_offset - 2, ACS_ULCORNER);
    mvaddch(p_grid->y_offset - 1, p_grid->x_offset + (2 * p_grid->width), ACS_URCORNER);
    mvaddch(p_grid->y_offset + p_grid->height, p_grid->x_offset - 2, ACS_LLCORNER);
    mvaddch(p_grid->y_offset + p_grid->height, p_grid->x_offset + (2 * p_grid->width), ACS_LRCORNER);

    /* Top line */
    for (int16_t i = p_grid->x_offset - 1; i < (2 * p_grid->width) + p_grid->x_offset; ++i) {
        mvaddch(p_grid->y_offset - 1, i, ACS_HLINE);
    }

    /* Bottom line */
    for (int16_t i = p_grid->x_offset - 1; i < (2 * p_grid->width) + p_grid->x_offset; ++i) {
        mvaddch(p_grid->y_offset + p_grid->height, i, ACS_HLINE);
    }

    /* Side lines */
    for (int16_t i = p_grid->y_offset; i < p_grid->y_offset + p_grid->height; ++i) {
        mvaddch(i, p_grid->x_offset - 2, ACS_VLINE);
        mvaddch(i, p_grid->x_offset + (2 * p_grid->width), ACS_VLINE);
    }


    /* Corners menu */
    mvaddch(p_grid->y_offset + p_grid->height + 2, p_grid->x_offset - 2, ACS_ULCORNER);
    mvaddch(p_grid->y_offset + p_grid->height + 2, p_grid->x_offset + (2 * p_grid->width), ACS_URCORNER);
    mvaddch(p_grid->y_offset + p_grid->height + 4, p_grid->x_offset - 2, ACS_LLCORNER);
    mvaddch(p_grid->y_offset + p_grid->height + 4, p_grid->x_offset + (2 * p_grid->width), ACS_LRCORNER);

    /* Side menu */
    mvaddch(p_grid->y_offset + p_grid->height + 3, p_grid->x_offset - 2, ACS_VLINE);
    mvaddch(p_grid->y_offset + p_grid->height + 3, p_grid->x_offset + (2 * p_grid->width), ACS_VLINE);

    /* Top menu */
    for (int16_t i = p_grid->x_offset - 1; i < p_grid->x_offset + (p_grid->width * 2); ++i) {
        mvaddch(p_grid->y_offset + p_grid->height + 2, i, ACS_HLINE);
    }

    /* Bottom menu */
    for (int16_t i = p_grid->x_offset - 1; i < p_grid->x_offset + (p_grid->width *2); ++i) {
        mvaddch(p_grid->y_offset + p_grid->height + 4, i, ACS_HLINE);
    }
}
