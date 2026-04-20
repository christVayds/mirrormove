#ifndef GAME_H
#define GAME_H

#include "raylib.h"

// GAME 
#define SCREENWIDTH 800
#define SCREENHEIGHT 500
#define MAXFPS 60

// MAP 
#define MAPSIZE 7 // 15x15 size of the map
#define MAPCOUNT 11

// TILES 
#define TILESIZE 32
#define ENTITYSPEED 20.0f

// FONTS 
#define FONTSIZE 30.0f

// TEXTURE REC
#define TITLE_REC (Rectangle){256, 16, 368, 144}
#define INFO_REC (Rectangle){192, 192, 64, 64}
#define INFO_REC_POS (Vector2){SCREENWIDTH - 64 - 16, SCREENHEIGHT-64-16}
#define START_REC (Rectangle){0, 128, 128, 64}
#define EXIT_REC (Rectangle){128, 128, 128, 64}
#define STARTBUTTON_REC (Rectangle){(SCREENWIDTH - 272) / 2, 350, 128, 64}
#define EXITBUTTON_REC (Rectangle){STARTBUTTON_REC.x + 144, 350, 128, 64}
#define SHEILDFACE_REC (Rectangle){224, 64, TILESIZE, TILESIZE}
#define ABOUT_REC (Rectangle){INFO_REC_POS.x, INFO_REC_POS.y, 64, 64}

// COLOR 
#define BACKGROUND_COLOR (Color){52, 97, 79, 255}
#define YANJIBLUE (Color){199, 155, 255, 255}

#endif
