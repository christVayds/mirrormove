#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>
#include <stdint.h>
#include "map.h"

typedef struct{
  Rectangle rec;
  Rectangle textureRec;
  Color color;
  bool alive;
  bool sheildOn;
} Entity;

void EntityMove(Entity *entity, int32_t dir, MapTile *mapTile);
void EntityDraw(Entity *entity, Texture2D *texture);
bool EntityCollide(Entity *entity1, Entity *entity2);

#endif
