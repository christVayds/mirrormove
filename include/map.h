#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include <stdint.h>
#include "game.h"

typedef enum{
  TILE_FLOOR,
  TILE_WALL,
  TILE_GOAL,
  TILE_DEADLY,
  TILE_EDGE,
  TILE_TELEPORTA,
  TILE_TELEPORTB,
  TILE_PUSHUP,
  TILE_PUSHDOWN,
  TILE_PUSHLEFT,
  TILE_PUSHRIGHT,
  TILE_SWITCH,
  TILE_SHEILD,
  TILE_REMOVESHEILD
} TileType;

typedef struct{
  TileType tileType;
  Rectangle rec;
  Rectangle textureRec;
  bool trigger;
} Tile;

typedef struct{
  Tile *tiles;
  Vector2 posEntity1;
  Vector2 posEntity2;
} MapTile;

// TODO:
typedef struct{
  MapTile **mapTiles;
  int32_t count;
  int32_t capacity;
} ListMaps;

bool MapInit(MapTile *mapTile, uint32_t *Map);
bool ListMapInit(ListMaps *listMaps, uint32_t (*Map)[MAPSIZE*MAPSIZE]);
void MapDraw(MapTile *mapTile, Texture2D *texture);
void FreeMap(MapTile *mapTile);
void FreeListMaps(ListMaps *listMaps);

#endif
