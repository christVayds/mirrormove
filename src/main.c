#include <stdint.h>
#include <raylib.h>
#include <stdlib.h>

#include "game.h"
#include "entity.h"
#include "map.h"
#include "scene.h"

int main(){
  // INITIALIZE WINDOW
  InitWindow(SCREENWIDTH, SCREENHEIGHT, "Mirror Move");

  // LOAD TEXTURE
  Texture2D texture = LoadTexture("resources/textures/Sprite.png");
  Font pixelFont = LoadFont("resources/fonts/Tiny5-Regular.ttf");

  // AUDIO INIT 
  InitAudioDevice();

  uint32_t Map[MAPCOUNT][MAPSIZE*MAPSIZE] = {
    {
      1,8,8,8,8,8,1,
      1,17,0,0,0,0,1,
      1,0,9,0,0,0,1,
      1,0,1,0,18,0,1,
      1,4,1,0,5,0,1,
      8,6,8,6,6,8,8,
      7,7,7,7,7,7,7,
    },
    {
      1,8,8,8,8,8,1,
      1,0,0,0,0,0,1,
      1,0,9,0,0,0,1,
      1,0,1,0,0,0,1,
      1,4,1,0,5,0,1,
      8,6,8,6,6,8,8,
      7,7,7,7,7,7,7,
    }
  };

  ListMaps listMaps;

  // Initialize Map
  ListMapInit(&listMaps, Map);
 
  // ENTITY 1  
  Entity entity1 = {
    .rec = (Rectangle){0, 0, TILESIZE, TILESIZE},
    .textureRec = (Rectangle){0, 0, TILESIZE, TILESIZE},
    .color = RED,
    .alive = true
  };

  // ENTITY 2
  Entity entity2 = {
    .rec = (Rectangle){0, 0, TILESIZE, TILESIZE},
    .textureRec = (Rectangle){32, 0, TILESIZE, TILESIZE},
    .color = BLUE,
    .alive = true
  };

  // SCENE 
  Scene scene = {
    .sceneType = SCENE_MAINMENU,
    .entity1 = &entity1,
    .entity2 = &entity2,
    .listMaps = &listMaps,
    .texture = &texture,
    .font = &pixelFont,
    .level = 0,
    .maxLevel = MAPCOUNT,
    .running = true
  };

  // START THE GAME LOOP
  SetTargetFPS(MAXFPS);
  while(!WindowShouldClose()){
    // UPDATE 
    //const float dt = GetFrameTime();

    SceneUpdate(&scene);

    if(!scene.running) break;

    // DRAWING
    BeginDrawing();
      // clear backgound
      ClearBackground(BACKGROUND_COLOR);

      // DRAW
      SceneDraw(&scene);

      // DEBUGGING
      //DrawFPS(10, 10);
      
      // DRAW HERE
    EndDrawing();
  }

  UnloadTexture(texture);
  UnloadFont(pixelFont);
  FreeListMaps(&listMaps);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
