/*
 * Mine Sweeper
 */

#include <stdint.h>
#include <stdlib.h>
#include <ncurses.h>

#include "grid.h"
#include "input.h"
#include "mines.h"
#include "render_menu.h"
#include "render_outlines.h"
#include "reveal.h"
#include "definitions.h"

int main(int argc, char ** argv) {
    /* Start curses */
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    start_color();
    clear();
    refresh();

    /* Cursor highlighting */
    init_pair(CURSOR, COLOR_BLACK, COLOR_RED);
    init_pair(FLAG, COLOR_BLACK, COLOR_YELLOW);

    /* Create grid */
    grid * p_grid = (grid *)malloc(sizeof(grid));
    if (p_grid == NULL) {
        endwin();
        return 1;
    }

    /* Dimensions, initial cursor placement, mines, flags */
    p_grid->width = 15;
    p_grid->height = 15;
    p_grid->y_offset = ((LINES - 1) / 2) - (3 * p_grid->height / 4);
    p_grid->x_offset = ((COLS - 1) / 2) - p_grid->width;
    p_grid->mines = 0;
    p_grid->flags = 0;
    p_grid->cursor = (p_grid->width / 2) - 1 + (p_grid->width * p_grid->height) / 2;

    /* Create minefield */
    int8_t * minefield = (int8_t *)malloc(p_grid->width * p_grid->height * sizeof(int8_t));
    if (minefield == NULL) {
        endwin();
        return 1;
    }
    p_grid->minefield = minefield;

    /* Fill minefield with mines and adjacent-to-mine values */
    initialize_minefield(p_grid);
    int8_t other_stuff = 0;
    for (int16_t i; i < p_grid->width * p_grid->height; ++i) {
        if (p_grid->minefield[i] != MINE && p_grid->minefield[i] > 0) {
            other_stuff = 1;
        }
    }
    if (!other_stuff) {
        endwin();
        printf("sad\n");
        return 1;
    }


    /* Render menu */
    render_menu(p_grid);

    /* Render outlines */
    render_outlines(p_grid);

    /* Render minefield */
    render_minefield(p_grid);

    /* Refresh page to show initial render */
    refresh();

    /* Gameplay */
    while (input(p_grid))
        ;

    getch();
    endwin();
    return 0;
}
