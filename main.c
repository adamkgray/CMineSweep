/*
 * Mine Sweeper
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#define EMPTY       0       /* A space with no mines */
#define CURSOR      1       /* Cursor coloring */
#define FLAG        2       /* Flag coloring */
#define VALUE       15      /* Mask to turn on only the bits that say how many adjacent mines there are */
#define MINE        16      /* A space that contains a mine */
#define UNCOVERED   32      /* Flag that indicates a space has been uncovered */
#define FLAGGED     64      /* Flag that indicates a space has been flagged */

typedef struct {
    int8_t width;
    int8_t height;
    int8_t y_offset;
    int8_t x_offset;
    int16_t mines;
    int16_t flags;
    int16_t cursor;
    int8_t * minefield;
} grid;

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

    /* Do the rendering */
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

void reveal(grid * p_grid, int16_t i) {
    /* Return if the space is already uncovered */
    if ((p_grid->minefield[i] & UNCOVERED) || (p_grid->minefield[i] & MINE)) {
        return;
    }

    /* Unflag */
    p_grid->minefield[i] &= ~FLAGGED;

    /* Uncover */
    p_grid->minefield[i] |= UNCOVERED;

    /* Render the uncovered mine (with no cursor) */
    render_mine(p_grid, i, 0);

    /* If the space was empty, reveal all surrounding spaces */
    if ((p_grid->minefield[i] & VALUE) == EMPTY) {
        /* Above */
        if (i - p_grid->width >= 0) {
            reveal(p_grid, i - p_grid->width);
        }

        /* Below */
        if (i + p_grid->width < p_grid->width * p_grid->height) {
            reveal(p_grid, i + p_grid->width);
        }

        /* Right */
        if ((i + 1) % p_grid->width != 0) {
            reveal(p_grid, i + 1);
        }

        /* Left */
        if (i % p_grid->width != 0) {
            reveal(p_grid, i - 1);
        }
    }
}

int8_t input(grid * p_grid) {
    /* Read character from keyboard and compute effects */
    int32_t c = getch();
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
                /* so sad */
                mvprintw(
                    p_grid->y_offset + p_grid->height + 3,
                    p_grid->x_offset,
                    "YOU LOSE :(   "
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
                /* Render menu */
                mvprintw(
                        p_grid->y_offset + p_grid->height + 3,
                        p_grid->x_offset,
                        "m: %.3d  f: %.3d",
                        p_grid->mines,
                        p_grid->flags
                );
            /* Only unflag spaces that are covered and flagged */
            } else if (!(p_grid->minefield[p_grid->cursor] & UNCOVERED) && (p_grid->minefield[p_grid->cursor] & FLAGGED)) {
                p_grid->minefield[p_grid->cursor] &= ~FLAGGED;
                p_grid->flags -= 1;
                render_mine(p_grid, p_grid->cursor, 1);
                /* Render menu */
                mvprintw(
                        p_grid->y_offset + p_grid->height + 3,
                        p_grid->x_offset,
                        "m: %.3d  f: %.3d",
                        p_grid->mines,
                        p_grid->flags
                );
            }
            break;
        /* MOVEMENT */
        case 'h':
        case KEY_LEFT:
            if (p_grid->cursor % p_grid->width != 0) {
                render_mine(p_grid, p_grid->cursor, 0);
                p_grid->cursor -= 1;
                render_mine(p_grid, p_grid->cursor, 1);
            }
            break;
        case 'j':
        case KEY_DOWN:
            if (p_grid->cursor < ((p_grid->width * p_grid->height) - p_grid->width)) {
                render_mine(p_grid, p_grid->cursor, 0);
                p_grid->cursor += p_grid->width;
                render_mine(p_grid, p_grid->cursor, 1);
            }
            break;
        case 'k':
        case KEY_UP:
            if (p_grid->cursor >= p_grid->width) {
                render_mine(p_grid, p_grid->cursor, 0);
                p_grid->cursor -= p_grid->width;
                render_mine(p_grid, p_grid->cursor, 1);
            }
            break;
        case 'l':
        case KEY_RIGHT:
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
    int16_t actual = 0;
    for (int16_t i = 0; i < p_grid->width * p_grid->height; ++i) {
        if (p_grid->minefield[i] & UNCOVERED) {
            /* Increment actual for each uncovered space */
            ++actual;
        }
    }
    /* End condition */
    if (actual == (p_grid->width * p_grid->height) - p_grid->mines) {
        mvprintw(
                p_grid->y_offset + p_grid->height + 3,
                p_grid->x_offset,
                "YOU WIN :)    "
        );
        /* Wait for any input */
        getch();
        /* QUIT */
        return 0;
    }
    return 1;
}

int main(int argc, char ** argv) {
    /* Scale */
    int8_t width = 10;
    int8_t height = 10;

    /* Parse args */
    if (argc > 1) {
        for (int8_t i = 1; i < argc; ++i) {
            char * arg = argv[i];
            /* Width */
            if (strcmp(arg, "-W") == 0) {
                if (i + 1 < argc) {
                    ++i;
                    width = atoi(argv[i]);
                    if (width == 0 || width < 8) {
                        /* Deafult is 10 */
                        width = 10;
                    }
                } else {
                    fprintf(stderr, "%s: No value provided for -W option\n", argv[0]);
                    return 1;
                }
            /* Height */
            } else if (strcmp(arg, "-H") == 0) {
                if (i + 1 < argc) {
                    ++i;
                    height = atoi(argv[i]);
                    if (height == 0 || height < 8) {
                        /* Deafult is 10 */
                        height = 10;
                    }
                } else {
                    fprintf(stderr, "%s: No value provided for -H option\n", argv[0]);
                    return 1;
                }
            } else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
                printf("Usage: %s\n-H height -> The height of the minefield. Default 10\n-W width -> Height of the minefield. Default 10. Minimum 8\n", argv[0]);
                return 0;
            } else {
                fprintf(stderr, "Usage: %s\n-H height -> The height of the minefield. Default 10\n-W width -> Width of the minefield. Default 10. Minimum 8\n", argv[0]);
                return 1;
            }
        }
    }

    /* This value just makes some computations below easier to read :) */
    int16_t minefield_size = width * height;

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
    p_grid->cursor = 0;

    /* Create minefield */
    int8_t * p_minefield = (int8_t *)malloc((minefield_size) * sizeof(int8_t));
    /* Exit if allocation failed */
    if (p_minefield == NULL) {
        free(p_grid);
        free(p_minefield);
        endwin();
        fprintf(stderr, "%s: not enough memory\n", argv[0]);
        return 1;
    }
    p_grid->minefield = p_minefield;

    /* Seed rand function */
    srand(time(NULL));


    /* Fill minefield with mines */
    for (int16_t i = 0; i < minefield_size; ++i) {
        /* Space is default EMPTY, but has 4 chances to become a MINE */
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

    /* Render menu */
    mvprintw(
            p_grid->y_offset + p_grid->height + 3,
            p_grid->x_offset,
            "m: %.3d  f: %.3d",
            p_grid->mines,
            p_grid->flags
    );

    /* Render outlines */
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

    /* Render minefield */
    p_minefield = p_grid->minefield;
    for (int16_t i = 0; i < (minefield_size); ++i) {
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
        p_grid->y_offset,
        p_grid->x_offset,
        ACS_CKBOARD
    );
    attroff(COLOR_PAIR(CURSOR));

    /* Refresh page to show initial render */
    refresh();

    /* Gameplay */
    while (input(p_grid))
        ;

    /* Stop curses */
    endwin();

    /* Free allocated memory before exiting */
    free(p_grid->minefield);
    free(p_grid);

    return 0;
}
