#include "render_menu.h"

void render_menu(grid * p_grid) {
    mvprintw(
        p_grid->y_offset + p_grid->height + 3,
        p_grid->x_offset,
        "m: %.3d  f: %.3d",
        p_grid->mines,
        p_grid->flags
    );
}
