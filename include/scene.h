#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>
#include "map.h"
#include "entity.h"

typedef enum{
  SCENE_GAMEPLAY,
  SCENE_MAINMENU,
  SCENE_STARTGAME,
  SCENE_PAUSED,
  SCENE_NEXTLEVEL,
  SCENE_GAMEOVER,
  SCENE_ENDGAME,
  SCENE_ABOUT
} SceneType;

typedef struct{
  SceneType sceneType;
  ListMaps *listMaps;   // TODO:
  Entity *entity1;
  Entity *entity2;
  Texture2D *texture;
  Font *font;
  int32_t level;
  int32_t maxLevel;
  bool running;
} Scene;

void SceneInit(Scene *scene);
void SceneUpdate(Scene *scene);
void SceneDraw(Scene *scene);

#endif
