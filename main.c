/*
 * Mine Sweeper
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "grid.h"
#include "input.h"
#include "mines.h"
#include "render_menu.h"
#include "render_outlines.h"
#include "reveal.h"
#include "definitions.h"

int main(int argc, char ** argv) {
    /* Create screen */
    initscr();
    /* Disable default keyboard echo to screen */
    noecho();
    /* Input is immediately available to program, no need for newline */
    cbreak();
    /* Hide terminal cursor */
    curs_set(0);
    /* Enable colors */
    start_color();
    /* Clear the terminal */
    clear();

    /* Scale */
    int8_t width = 10;
    int8_t height = 10;

    /* Parse args */
    if (argc > 1) {
        for (int8_t i = 1; i < argc; ++i) {
            char * arg = argv[i];
            /* Width */
            if (strcmp(arg, "-w") == 0) {
                if (i + 1 < argc) {
                    ++i;
                    width = atoi(argv[i]);
                    if (width == 0 || width < 8) {
                        /* Deafult is 10 */
                        width = 10;
                    }
                }
            /* Height */
            } else if (strcmp(arg, "-h") == 0) {
                if (i + 1 < argc) {
                    ++i;
                    height = atoi(argv[i]);
                    if (height == 0 || height < 8) {
                        /* Deafult is 10 */
                        height = 10;
                    }
                }
            }
        }
    }

    /* Cursor highlighting */
    init_pair(CURSOR, COLOR_BLACK, COLOR_RED);
    init_pair(FLAG, COLOR_BLACK, COLOR_YELLOW);

    /* Create grid */
    grid * p_grid = (grid *)malloc(sizeof(grid));
    /* Exit if allocation failed */
    if (p_grid == NULL) {
        free(p_grid);
        endwin();
        fprintf(stderr, "%s: not enough memory\n", argv[0]);
        return 1;
    }

    /* Dimensions, initial cursor placement, mines, flags */
    p_grid->width = width;
    p_grid->height = height;
    p_grid->y_offset = 2;
    p_grid->x_offset = 4;
    p_grid->mines = 0;
    p_grid->flags = 0;
    p_grid->cursor = ((width * height) / 2);

    /* Create minefield */
    int8_t * p_minefield = (int8_t *)malloc((width * height) * sizeof(int8_t));
    /* Exit if allocation failed */
    if (p_minefield == NULL) {
        free(p_grid);
        free(p_minefield);
        endwin();
        fprintf(stderr, "%s: not enough memory\n", argv[0]);
        return 1;
    }
    p_grid->minefield = p_minefield;

    /* Fill minefield with mines and adjacent-to-mine values */
    initialize_minefield(p_grid);

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

    /* Stop curses */
    endwin();

    /* Free allocated memory before exiting */
    free(p_grid);
    free(p_minefield);

    return 0;
}
