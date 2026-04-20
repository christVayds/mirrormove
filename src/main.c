#include <stdint.h>
#include <raylib.h>
#include <stdlib.h>

#include "game.h"
#include "entity.h"
#include "map.h"
#include "scene.h"

//#include <emscripten/emscripten.h>

static Scene scene = {0};

void UpdateGame(void);

int main(){
  // INITIALIZE WINDOW
  InitWindow(SCREENWIDTH, SCREENHEIGHT, "Mirror Move");

  // LOAD TEXTURE
  Texture2D texture = LoadTexture("resources/textures/Sprite.png");
  Font pixelFont = LoadFont("resources/fonts/Tiny5-Regular.ttf");

  // AUDIO INIT 
  InitAudioDevice();

  uint32_t Map[MAPCOUNT][MAPSIZE*MAPSIZE] = {  
    { // 0 
      1,8,8,8,8,8,1,
      1,0,0,0,0,0,1,
      1,0,0,0,0,0,1,
      1,0,0,0,0,0,1,
      1,4,0,0,5,0,1,
      1,0,0,0,0,0,1,
      8,8,8,8,8,8,8,
    },
    { // 1 
      1,8,8,8,8,8,1,
      1,0,0,0,0,0,1,
      1,0,0,0,0,0,1,
      1,0,1,0,0,0,1,
      1,4,1,0,5,0,1,
      8,6,8,6,6,8,8,
      7,7,7,7,7,7,7,
    },
    { // 2 
      1,8,8,8,8,8,1,
      1,0,0,0,0,0,1,
      1,0,9,0,0,0,1,
      1,0,1,0,0,0,1,
      1,4,1,0,5,0,1,
      8,6,8,6,6,8,8,
      7,7,7,7,7,7,7,
    }, 
    { // 3
      1,8,8,8,8,8,1,
      1,0,0,0,0,0,1,
      1,0,9,0,0,0,1,
      1,0,1,0,0,0,1,
      1,4,1,9,5,0,1,
      8,6,8,6,6,8,8,
      7,7,7,7,7,7,7,
    },
    { // 4
      1,8,8,8,8,8,1,
      1,17,0,0,0,0,1,
      1,0,9,0,0,0,1,
      1,0,1,0,18,0,1,
      1,4,1,0,5,0,1,
      8,6,8,6,6,8,8,
      7,7,7,7,7,7,7,
    },
    { // 5 
      7,7,7,7,7,7,7,
      7,0,0,0,5,0,7,
      7,0,0,0,0,0,7,
      7,0,0,8,0,0,7,
      7,4,0,0,0,0,7,
      7,6,6,6,6,6,7,
      7,7,7,7,7,7,7,
    },
    { // 6 
      7,8,7,7,7,7,7,
      7,0,14,0,0,0,7,
      7,0,0,5,0,0,7,
      8,0,4,0,0,0,7,
      7,0,0,0,0,0,7,
      7,6,15,6,6,6,7,
      7,7,7,7,7,7,7,
    },
    { // 7 
      7,7,7,7,7,7,7,
      7,0,0,0,0,0,14,
      7,0,0,0,5,0,7,
      8,0,4,0,0,0,7,
      9,0,17,18,0,0,7,
      7,6,6,15,6,6,7,
      7,7,7,9,7,7,7,
    },
    { // 8 
      7,7,7,7,7,7,7,
      7,0,0,0,0,5,8,
      7,0,0,0,0,12,7,
      7,9,4,0,0,0,7,
      7,0,0,0,0,0,7,
      7,6,6,6,6,6,7,
      7,7,7,7,7,7,7,
    },
    { // 9 
      7,7,7,7,7,7,7,
      7,0,0,0,0,5,8,
      7,0,0,0,9,12,7,
      7,0,4,0,0,0,7,
      7,0,0,0,0,0,7,
      7,6,6,6,6,6,7,
      7,7,7,7,7,7,7,
    },
    { // 10 
      7,7,7,7,7,7,7,
      7,0,0,11,0,5,8,
      7,0,0,0,9,12,7,
      7,0,4,0,0,0,7,
      7,0,0,0,12,0,7,
      7,6,6,6,6,6,7,
      7,7,7,7,7,7,7,
    },
  };

  ListMaps listMaps;

  // Initialize Map
  ListMapInit(&listMaps, Map);
 
  // ENTITY 1  
  Entity entity1 = {
    .rec = (Rectangle){0, 0, TILESIZE, TILESIZE},
    .textureRec = (Rectangle){0, 0, TILESIZE, TILESIZE},
    .targetPosition = (Vector2){0, 0},
    .color = RED,
    .alive = true,
    .isMoving = false
  };

  // ENTITY 2
  Entity entity2 = {
    .rec = (Rectangle){0, 0, TILESIZE, TILESIZE},
    .textureRec = (Rectangle){32, 0, TILESIZE, TILESIZE},
    .targetPosition = (Vector2){0, 0},
    .color = BLUE,
    .alive = true,
    .isMoving = false
  };

  // SCENE 
  scene.sceneType = SCENE_MAINMENU;
  scene.entity1 = &entity1;
  scene.entity2 = &entity2;
  scene.listMaps = &listMaps;
  scene.texture = &texture;
  scene.font = &pixelFont;
  scene.level = 0;
  scene.maxLevel = MAPCOUNT;
  scene.running = true;

  // START THE GAME LOOP
  // EXE
  SetTargetFPS(MAXFPS);
  
  while(!WindowShouldClose()){ 
    // UPDATE GAME
    UpdateGame(); 
    if(!scene.running) break; 
  }

  // BROWSER
  //emscripten_set_main_loop(UpdateGame, 0, 1);

  UnloadTexture(texture);
  UnloadFont(pixelFont);
  FreeListMaps(&listMaps);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}

void UpdateGame(void){
  // UPDATE 
  //const float dt = GetFrameTime();

  SceneUpdate(&scene); 

  // DRAWING
  BeginDrawing();
    // clear backgound
    ClearBackground(BACKGROUND_COLOR);

    // DRAW
    SceneDraw(&scene);

    // DEBUGGING
    //DrawFPS(SCREENWIDTH - 100, 10);
      
    // DRAW HERE
  EndDrawing();
}
