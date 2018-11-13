#ifndef GRID_H_
#define GRID_H_

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

#endif
