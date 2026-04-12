#include "map.h"
#include "game.h"
#include <stdlib.h>
#include <stdio.h>

bool MapInit(MapTile *mapTile, uint32_t *Map){
  mapTile->tiles = (Tile*)malloc(sizeof(Tile) * (MAPSIZE*MAPSIZE));
  if(!mapTile->tiles) return 0;

  const int32_t xPos = (SCREENWIDTH - (MAPSIZE * TILESIZE)) / 2;
  const int32_t ypos = (SCREENHEIGHT - (MAPSIZE * TILESIZE)) / 2;
  Rectangle rec = {xPos,ypos, TILESIZE, TILESIZE};
  int32_t countX = 0;
  
  for(int32_t y=0;y<MAPSIZE;y++){
    for(int32_t x=0;x<MAPSIZE;x++){
      int32_t index = y * MAPSIZE + x;
      
      Tile tile = {0};
      
      // NOTE: temporary
      tile.textureRec = (Rectangle){0, 32, TILESIZE, TILESIZE};
      switch(Map[index]){
        case 0:
          tile.tileType = TILE_FLOOR;
          tile.textureRec = (Rectangle){0, 32, TILESIZE, TILESIZE};
          break;
        case 1: // WALL 
          tile.tileType = TILE_WALL;
          tile.textureRec = (Rectangle){32, 32, TILESIZE, TILESIZE};
          break;
        case 2: // SPIKE 
          tile.tileType = TILE_DEADLY;
          break;
        case 3: // GOAL
          tile.tileType = TILE_GOAL;
          break;
        case 4: // ENTITY 1 POSITION 
          tile.tileType = TILE_FLOOR;
          mapTile->posEntity1 = (Vector2){rec.x, rec.y};
          break;
        case 5: // ENTITY 2 POSITION
          tile.tileType = TILE_FLOOR;
          mapTile->posEntity2 = (Vector2){rec.x, rec.y};
          break;
        case 6: // FLOOR EDGE
          tile.tileType = TILE_FLOOR;
          tile.textureRec = (Rectangle){0, 64, TILESIZE, TILESIZE};
          break;
        case 7: // MAP EDGE
          tile.tileType = TILE_EDGE;
          tile.textureRec = (Rectangle){0, 96, TILESIZE, TILESIZE};
          break;
        case 8: // WALL EDGE 
          tile.tileType = TILE_WALL;
          tile.textureRec = (Rectangle){32, 64, TILESIZE, TILESIZE};
          break;
        case 9: // SPIKE 
          tile.tileType = TILE_DEADLY;
          tile.textureRec = (Rectangle){64, 32, TILESIZE, TILESIZE};
          break;
        case 10:  // PUSH UP 
          tile.tileType = TILE_PUSHUP;
          tile.textureRec = (Rectangle){32, 96, TILESIZE, TILESIZE};
          break;
        case 11:  // PUSH DOWN 
          tile.tileType = TILE_PUSHDOWN;
          tile.textureRec = (Rectangle){64, 96, TILESIZE, TILESIZE};
          break;
        case 12:  // PUSH LEFT 
          tile.tileType = TILE_PUSHLEFT;
          tile.textureRec = (Rectangle){128, 96, TILESIZE, TILESIZE};
          break;
        case 13:  // PUSH RIGHT 
          tile.tileType = TILE_PUSHRIGHT;
          tile.textureRec = (Rectangle){96, 96, TILESIZE, TILESIZE};
          break;
        case 14:
          tile.tileType = TILE_TELEPORTA;
          tile.textureRec = (Rectangle){96, 64, TILESIZE, TILESIZE};
          break;
        case 15:
          tile.tileType = TILE_TELEPORTB;
          tile.textureRec = (Rectangle){128, 64, TILESIZE, TILESIZE};
          break;
        case 16:
          tile.tileType = TILE_SWITCH;
          tile.textureRec = (Rectangle){192, 96, TILESIZE, TILESIZE};
          break;
        case 17:
          tile.tileType = TILE_SHEILD;
          tile.textureRec = (Rectangle){160, 64, TILESIZE, TILESIZE};
          break;
        case 18:
          tile.tileType = TILE_REMOVESHEILD;
          tile.textureRec = (Rectangle){192, 64, TILESIZE, TILESIZE};
          break;
      }

      // apply the position to the tile 
      tile.rec = rec;
      tile.trigger = false;
     
      // modify tile position
      countX++;
      rec.x += TILESIZE;
      if(countX >= MAPSIZE){
        rec.x = xPos;
        rec.y += TILESIZE;
        countX = 0;
      }

      // append the new tile in the map
      mapTile->tiles[index] = tile;
    }
  }

  return 1;
}

bool ListMapInit(ListMaps *listMaps, uint32_t (*Map)[MAPSIZE*MAPSIZE]){
  listMaps->capacity = MAPCOUNT;
  listMaps->count = 0;
  listMaps->mapTiles = (MapTile**)malloc(sizeof(MapTile) * listMaps->capacity);
  if(!listMaps->mapTiles) return false;

  // TODO: Loop through the Map
  for(int32_t i=0;i<listMaps->capacity;i++){
    listMaps->mapTiles[i] = (MapTile*)malloc(sizeof(MapTile));
    if(!listMaps->mapTiles[i]) return 0;

    // initialize map
    if(!MapInit(listMaps->mapTiles[i], Map[i])) return 0;
  }

  return true;
}

void MapDraw(MapTile *mapTile, Texture2D *texture){
  for(int32_t y=0;y<MAPSIZE;y++){
    for(int32_t x=0;x<MAPSIZE;x++){
      int32_t index = y * MAPSIZE + x;
      DrawTextureRec(*texture, mapTile->tiles[index].textureRec, (Vector2){mapTile->tiles[index].rec.x, mapTile->tiles[index].rec.y}, WHITE);
      //DrawRectangleLines(mapTile->tiles[index].rec.x, mapTile->tiles[index].rec.y, mapTile->tiles[index].rec.width, mapTile->tiles[index].rec.height, RED);
    }
  }
}

void FreeMap(MapTile *mapTile){
  free(mapTile->tiles);
}

void FreeListMaps(ListMaps *listMaps){
  for(int32_t i=0;i<listMaps->capacity;i++){
    free(listMaps->mapTiles[i]->tiles);
    free(listMaps->mapTiles[i]);
  }
  free(listMaps->mapTiles); 
}
