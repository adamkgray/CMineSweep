#ifndef MINES_H_
#define MINES_H_

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "definitions.h"
#include "grid.h"
void initialize_minefield(grid * p_grid);
void render_minefield(grid * p_grid);
void render_mine(grid * p_grid, int16_t i, int8_t is_cursor);

#endif
