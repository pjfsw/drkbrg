#pragma once

typedef enum {
    WALL,
    EXIT
} RoomSide;

typedef struct {
    /* Sides: North, East, South, West */
    RoomSide sides[4];
} RoomTile;