#include "entity.h"
#include "game.h"
#include <math.h>

#include <stdio.h>

void EntityMove(Entity *entity, int32_t dir, MapTile *mapTile, const float dt){
  Vector2 gridOrigin = (Vector2){(SCREENWIDTH - (MAPSIZE * TILESIZE)) / 2, (SCREENHEIGHT - (MAPSIZE * TILESIZE)) / 2};
 
  Vector2 targetPosition = (Vector2){entity->targetPosition.x, entity->targetPosition.y};
  if(!entity->isMoving){ 
    if(IsKeyPressed(KEY_D)) targetPosition.x += 1 * dir;
    if(IsKeyPressed(KEY_A)) targetPosition.x += -1 * dir;
    if(IsKeyPressed(KEY_W)) targetPosition.y += -1 * dir;
    if(IsKeyPressed(KEY_S)) targetPosition.y += 1 * dir;
  }

  Vector2 targetWorldPosition = (Vector2){
    (int32_t)(gridOrigin.x + targetPosition.x * TILESIZE), 
    (int32_t)(gridOrigin.y + targetPosition.y * TILESIZE)
  };

  entity->isMoving = (entity->rec.x != targetWorldPosition.x || entity->rec.y != targetWorldPosition.y);

  Rectangle desiredPos = (Rectangle){entity->rec.x, entity->rec.y, entity->rec.width, entity->rec.height};
  desiredPos.x += (targetWorldPosition.x - entity->rec.x) * ENTITYSPEED * dt;
  desiredPos.y += (targetWorldPosition.y - entity->rec.y) * ENTITYSPEED * dt;

  if(fabs(desiredPos.x - targetWorldPosition.x) < 1) desiredPos.x = targetWorldPosition.x;
  if(fabs(desiredPos.y - targetWorldPosition.y) < 1) desiredPos.y = targetWorldPosition.y;

  for(int32_t y=0;y<MAPSIZE;y++){
    for(int32_t x=0;x<MAPSIZE;x++){
      int32_t index = y * MAPSIZE + x;
      if(CheckCollisionRecs(desiredPos, mapTile->tiles[index].rec)){
        switch(mapTile->tiles[index].tileType){
          case TILE_WALL:{
            targetPosition.x = entity->targetPosition.x;
            targetPosition.y = entity->targetPosition.y;
            desiredPos.x = (int32_t)(gridOrigin.x + targetPosition.x * TILESIZE);
            desiredPos.y = (int32_t)(gridOrigin.y + targetPosition.y * TILESIZE);
            break;
          }
          case TILE_DEADLY:{
            if(!entity->sheildOn)
              entity->alive = false;
            else{
              targetPosition.x = entity->targetPosition.x;
              targetPosition.y = entity->targetPosition.y;
              desiredPos.x = (int32_t)(gridOrigin.x + targetPosition.x * TILESIZE);
              desiredPos.y = (int32_t)(gridOrigin.y + targetPosition.y * TILESIZE);
            }
            break;
          }
          case TILE_EDGE:{
            entity->alive = false;
            break;
          }
          case TILE_PUSHDOWN:{
            if(!entity->isMoving)
              targetPosition.y += 1;
            break;
          }
          case TILE_PUSHUP:{
            if(!entity->isMoving)
              targetPosition.y -= 1;
            break;
          }
          case TILE_PUSHLEFT:{
            if(!entity->isMoving)
              targetPosition.x -= 1;
            break;
          }
          case TILE_PUSHRIGHT:{
            if(!entity->isMoving)
              targetPosition.x += 1;
            break;
          } 
          case TILE_TELEPORTA:{
            for(int32_t i=0;i<MAPSIZE*MAPSIZE;i++){
              if(mapTile->tiles[i].tileType == TILE_TELEPORTB){ 
                desiredPos.x = mapTile->tiles[i].rec.x;
                desiredPos.y = mapTile->tiles[i].rec.y;
                targetPosition.x = (int32_t)((desiredPos.x - gridOrigin.x) / TILESIZE);
                targetPosition.y = (int32_t)((desiredPos.y - gridOrigin.y) / TILESIZE);
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
  entity->targetPosition.x = targetPosition.x;
  entity->targetPosition.y = targetPosition.y;
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
