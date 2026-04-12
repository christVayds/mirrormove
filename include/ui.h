#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <stdint.h>

typedef struct{
  Rectangle textureRec;
  Rectangle rec;
  bool clicked;
} Button;

typedef struct{
  Button *buttons;
  int32_t quantity;
} MapButtons;

#endif
