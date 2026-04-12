#include "entity.h"
#include "game.h"

#include <stdio.h>

void EntityMove(Entity *entity, int32_t dir, MapTile *mapTile){
  Vector2 move = {0};
  if(IsKeyPressed(KEY_D)) move.x = 1 * dir;
  if(IsKeyPressed(KEY_A)) move.x = -1 * dir;
  if(IsKeyPressed(KEY_W)) move.y = -1 * dir;
  if(IsKeyPressed(KEY_S)) move.y = 1 * dir;

  Rectangle desiredPos = (Rectangle){entity->rec.x, entity->rec.y, entity->rec.width, entity->rec.height};
  desiredPos.x += entity->rec.width * move.x;
  desiredPos.y += entity->rec.height * move.y;

  for(int32_t y=0;y<MAPSIZE;y++){
    for(int32_t x=0;x<MAPSIZE;x++){
      int32_t index = y * MAPSIZE + x;
      if(CheckCollisionRecs(desiredPos, mapTile->tiles[index].rec)){
        switch(mapTile->tiles[index].tileType){
          case TILE_WALL:{
            desiredPos.x = entity->rec.x;
            desiredPos.y = entity->rec.y;
            break;
          }
          case TILE_DEADLY:{
            if(!entity->sheildOn)
              entity->alive = false;
            else{
              desiredPos.x = entity->rec.x;
              desiredPos.y = entity->rec.y;
            }
            break;
          }
          case TILE_EDGE:{
            entity->alive = false;
            break;
          }
          case TILE_PUSHDOWN:{
            desiredPos.y += TILESIZE; 
            break;
          }
          case TILE_PUSHUP:{
            desiredPos.y -= TILESIZE; 
            break;
          }
          case TILE_PUSHLEFT:{
            desiredPos.x -= TILESIZE; 
            break;
          }
          case TILE_PUSHRIGHT:{
            desiredPos.x += TILESIZE; 
            break;
          } 
          case TILE_TELEPORTA:{
            for(int32_t i=0;i<MAPSIZE*MAPSIZE;i++){
              if(mapTile->tiles[i].tileType == TILE_TELEPORTB){
                desiredPos.x = mapTile->tiles[i].rec.x;
                desiredPos.y = mapTile->tiles[i].rec.y;
                break;
              }
            } 
            break;
          }
          case TILE_SHEILD:{
            if(!entity->sheildOn) entity->sheildOn = true;
            break;
          }
          case TILE_REMOVESHEILD:{
            if(entity->sheildOn) entity->sheildOn = false;
            else{
              desiredPos.x = entity->rec.x;
              desiredPos.y = entity->rec.y;
            }
            break;
          }
          default:
            break;
        }
      }
    }
  }

  entity->rec.x = desiredPos.x;
  entity->rec.y = desiredPos.y;
}

void EntityDraw(Entity *entity, Texture2D *texture){
  //DrawRectangleRec(entity->rec, entity->color);
  DrawTextureRec(*texture, entity->textureRec, (Vector2){entity->rec.x, entity->rec.y}, WHITE);
  if(entity->sheildOn)
    DrawTextureRec(*texture, SHEILDFACE_REC, (Vector2){entity->rec.x, entity->rec.y}, WHITE);
}

bool EntityCollide(Entity *entity1, Entity *entity2){
  if(CheckCollisionRecs(entity1->rec, entity2->rec))
    return true;
  return false;
}
