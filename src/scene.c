#include "scene.h"
#include "game.h"

static bool ButtonClicked(Vector2 mousepos, Rectangle buttonRec){
  if(mousepos.x > buttonRec.x &&
    mousepos.x < buttonRec.x + buttonRec.width &&
    mousepos.y > buttonRec.y &&
    mousepos.y < buttonRec.y + buttonRec.height){ 
    return true;
  }
  return false;
}

// RESET LEVEL 
static void ResetLevel(Scene *scene){
  scene->entity1->rec.x = scene->listMaps->mapTiles[scene->level]->posEntity1.x;
  scene->entity1->rec.y = scene->listMaps->mapTiles[scene->level]->posEntity1.y;

  scene->entity2->rec.x = scene->listMaps->mapTiles[scene->level]->posEntity2.x;
  scene->entity2->rec.y = scene->listMaps->mapTiles[scene->level]->posEntity2.y;

  scene->entity1->alive = true;
  scene->entity2->alive = true;

  scene->entity1->sheildOn = false;
  scene->entity2->sheildOn = false;
}

// GAME PLAY SCENE 

static void UpdateGamePlay(Scene *scene){
  Vector2 mousepos = GetMousePosition();
  if(IsMouseButtonPressed(0)){
    if(ButtonClicked(mousepos, (Rectangle){10, 10, 64, 64})){ 
      scene->sceneType = SCENE_PAUSED;
    } 

  }

  // paused the game
  if(IsKeyPressed(KEY_X)) scene->sceneType = SCENE_PAUSED;
  
  // check if entities alive
  if(!scene->entity1->alive || !scene->entity2->alive) scene->sceneType = SCENE_GAMEOVER;

  // UPDATE ENTITY
  EntityMove(scene->entity1, 1, scene->listMaps->mapTiles[scene->level]);
  EntityMove(scene->entity2, -1, scene->listMaps->mapTiles[scene->level]); 

  // TODO: check this
  // check if entities collide 
  if(EntityCollide(scene->entity1, scene->entity2) && (!scene->entity1->sheildOn && !scene->entity2->sheildOn)){
    scene->level++;
    
    // update maps, scene and level 
    if(scene->level < scene->maxLevel){
      scene->sceneType = SCENE_NEXTLEVEL;
    } else {
      scene->sceneType = SCENE_ENDGAME;  // reset level 
    }
  } 
}

static void DrawGamePlay(Scene *scene){
  // DRAW MAP
  MapDraw(scene->listMaps->mapTiles[scene->level], scene->texture); 

  // DRAW ENTITY 
  EntityDraw(scene->entity1, scene->texture);
  EntityDraw(scene->entity2, scene->texture);

  // paused button 
  DrawTextureRec(*scene->texture, (Rectangle){256, 192, 64, 64}, (Vector2){10, 10}, WHITE);
}

// MAIN MENU SCENE

static void UpdateMainMenu(Scene *scene){
  Vector2 mousepos = GetMousePosition();
  if(IsMouseButtonPressed(0)){
    if(ButtonClicked(mousepos, STARTBUTTON_REC)){ 
      scene->sceneType = SCENE_STARTGAME;
      ResetLevel(scene);
    } else if(ButtonClicked(mousepos, EXITBUTTON_REC)){
      scene->running = false;
    } else if(ButtonClicked(mousepos, ABOUT_REC)){
      scene->sceneType = SCENE_ABOUT;
    }
  }

  if(IsKeyPressed(KEY_X)){
    scene->sceneType = SCENE_STARTGAME;
    ResetLevel(scene);
  }
  if(IsKeyPressed(KEY_C)) scene->running = false;
}

static void DrawMainMenu(Scene *scene){
  DrawTextEx(*scene->font, "Yanji Games", (Vector2){(SCREENWIDTH - TITLE_REC.width) / 2, ((SCREENHEIGHT - TITLE_REC.height) / 2) - FONTSIZE}, FONTSIZE, 1.0f, YANJIBLUE);
  DrawTextureRec(*scene->texture, TITLE_REC, (Vector2){(SCREENWIDTH - TITLE_REC.width) / 2, (SCREENHEIGHT - TITLE_REC.height) / 2}, WHITE);
  
  // draw start exit buttons
  //const int32_t position = (SCREENWIDTH - 272) / 2;
  DrawTextureRec(*scene->texture, START_REC, (Vector2){STARTBUTTON_REC.x, STARTBUTTON_REC.y}, WHITE);
  DrawTextureRec(*scene->texture, EXIT_REC, (Vector2){EXITBUTTON_REC.x, EXITBUTTON_REC.y}, WHITE);

  // about
  DrawTextureRec(*scene->texture, INFO_REC, INFO_REC_POS, WHITE); 
}

// NEXT LEVEL SCENE
static void UpdateNextLevel(Scene *scene){
  Vector2 mousepos = GetMousePosition();
  if(IsMouseButtonPressed(0)){
    if(ButtonClicked(mousepos, (Rectangle){330, 280, 64, 64})){ 
      scene->sceneType = SCENE_GAMEPLAY;
      ResetLevel(scene);
    } else if(ButtonClicked(mousepos, (Rectangle){250, 280, 64, 64})){ 
      scene->level--;
      scene->sceneType = SCENE_GAMEPLAY;
      ResetLevel(scene);
    } else if(ButtonClicked(mousepos, (Rectangle){410, 280, 64, 64})){ 
      scene->sceneType = SCENE_MAINMENU;
    }
  }

  // go to next level 
  if(IsKeyPressed(KEY_X)){ 
    ResetLevel(scene);
    scene->sceneType = SCENE_GAMEPLAY;
  }
  
  // quit
  if(IsKeyPressed(KEY_C)) scene->sceneType = SCENE_MAINMENU;
  
  // retry level 
  if(IsKeyPressed(KEY_Z)){
    scene->level--;
    scene->sceneType = SCENE_GAMEPLAY;
    ResetLevel(scene);
  }
}

static void DrawNextLevel(Scene *scene){
  DrawTextEx(*scene->font, "Complete! Next level", (Vector2){250, 250}, FONTSIZE, 1.0f, WHITE);

  DrawTextureRec(*scene->texture, (Rectangle){64, 192, 64, 64}, (Vector2){250, 280}, WHITE);
  DrawTextureRec(*scene->texture, (Rectangle){128, 192, 64, 64}, (Vector2){330, 280}, WHITE);
  DrawTextureRec(*scene->texture, (Rectangle){0, 192, 64, 64}, (Vector2){410, 280}, WHITE);
}

// GAME OVER SCENE
static void UpdateGameOver(Scene *scene){
  Vector2 mousepos = GetMousePosition();
  if(IsMouseButtonPressed(0)){
    if(ButtonClicked(mousepos, (Rectangle){250, 280, 64, 64})){ 
      scene->sceneType = SCENE_GAMEPLAY;
      ResetLevel(scene);
    } else if(ButtonClicked(mousepos, (Rectangle){330, 280, 64, 64})){ 
      scene->sceneType = SCENE_MAINMENU;
    }
  }

  // retry level 
  if(IsKeyPressed(KEY_X)){ 
    scene->sceneType = SCENE_GAMEPLAY;
    ResetLevel(scene);
  }
  
  // quit
  if(IsKeyPressed(KEY_C)) scene->sceneType = SCENE_MAINMENU; 
}

static void DrawGameOver(Scene *scene){
  DrawTextEx(*scene->font, "Game over. Try Again!", (Vector2){250, 250}, FONTSIZE, 1.0f, WHITE);

  DrawTextureRec(*scene->texture, (Rectangle){64, 192, 64, 64}, (Vector2){250, 280}, WHITE);
  DrawTextureRec(*scene->texture, (Rectangle){0, 192, 64, 64}, (Vector2){330, 280}, WHITE);
}

// END GAME SCENE 
static void UpdateEndGame(Scene *scene){
  Vector2 mousepos = GetMousePosition();
  if(IsMouseButtonPressed(0)){
    if(ButtonClicked(mousepos, (Rectangle){250, 280, 64, 64})){ 
      scene->sceneType = SCENE_MAINMENU;
      scene->level = 0;
    } else if(ButtonClicked(mousepos, (Rectangle){330, 280, 64, 64})){ 
      scene->sceneType = SCENE_ABOUT;
      scene->level = 0;
    }
  }

  // TODO: redo the level key pressed 

  // quit  
  if(IsKeyPressed(KEY_X)){
    scene->sceneType = SCENE_MAINMENU;
    scene->level = 0;
  }
  
  // about
  if(IsKeyPressed(KEY_C)){ 
    scene->sceneType = SCENE_ABOUT; 
    scene->level = 0;
  }
}

static void DrawEndGame(Scene *scene){
  DrawTextEx(*scene->font, "Game Complete!", (Vector2){250, 250}, FONTSIZE, 1.0f, WHITE);

  DrawTextureRec(*scene->texture, (Rectangle){0, 192, 64, 64}, (Vector2){250, 280}, WHITE);
  DrawTextureRec(*scene->texture, (Rectangle){192, 192, 64, 64}, (Vector2){330, 280}, WHITE);
}

// PAUSE GAME SCENE
static void UpdatePaused(Scene *scene){
  Vector2 mousepos = GetMousePosition();
  if(IsMouseButtonPressed(0)){
    if(ButtonClicked(mousepos, (Rectangle){250, 280, 64, 64})){ 
      scene->sceneType = SCENE_GAMEPLAY;
      ResetLevel(scene);
    } else if(ButtonClicked(mousepos, (Rectangle){330, 280, 64, 64})){ 
      scene->sceneType = SCENE_MAINMENU;
    }
  }

  // return to gameplay 
  if(IsKeyPressed(KEY_X)) scene->sceneType = SCENE_GAMEPLAY;
  
  // quit
  if(IsKeyPressed(KEY_C)) scene->sceneType = SCENE_MAINMENU; 
}

static void DrawPaused(Scene *scene){
  DrawTextEx(*scene->font, "Game Paused!", (Vector2){250, 250}, FONTSIZE, 1.0f, WHITE);

  DrawTextureRec(*scene->texture, (Rectangle){64, 192, 64, 64}, (Vector2){250, 280}, WHITE);
  DrawTextureRec(*scene->texture, (Rectangle){0, 192, 64, 64}, (Vector2){330, 280}, WHITE);
}

// START GAME 
static void UpdateStartGame(Scene *scene){
  if(GetKeyPressed() != 0) scene->sceneType = SCENE_GAMEPLAY;
}

static void DrawStartGame(Scene *scene){
  DrawTextEx(*scene->font, "Hi! Player <3\nBefore we start, I wan't you to know:", (Vector2){100, 120}, FONTSIZE, 1.0f, WHITE);
  
  DrawTextureRec(*scene->texture, (Rectangle){0, 0, TILESIZE, TILESIZE}, (Vector2){100, 215}, WHITE);
  DrawTextureRec(*scene->texture, (Rectangle){32, 0, TILESIZE, TILESIZE}, (Vector2){350, 215}, WHITE);
  DrawTextEx(*scene->font, "This is you", (Vector2){100, 250}, FONTSIZE, 1.0f, WHITE);
  DrawTextEx(*scene->font, "This is you also\n but opposite", (Vector2){350, 250}, FONTSIZE, 1.0f, WHITE);

  DrawTextEx(*scene->font, "Mission, merge each other.\n\nPress any key to start!", (Vector2){100, 350}, FONTSIZE, 1.0f, WHITE);
}

// ABOUT SCENE 
void UpdateAbout(Scene *scene){
  if(GetKeyPressed() != 0) scene->sceneType = SCENE_MAINMENU;
}

void DrawAbout(Scene *scene){
  DrawTextEx(*scene->font, "By Yanji Game\nThis game is written in C Programming Language.\nGame Programmer: Christian Vaydal\n\nPress any key to exit.", (Vector2){100, 120}, FONTSIZE, 1.0f, WHITE);
}

void SceneUpdate(Scene *scene){
  switch(scene->sceneType){
    case SCENE_GAMEPLAY:
      UpdateGamePlay(scene);
      break;
    case SCENE_MAINMENU:
      UpdateMainMenu(scene);
      break;
    case SCENE_PAUSED:
      UpdatePaused(scene);
      break;
    case SCENE_NEXTLEVEL:
      UpdateNextLevel(scene);
      break;
    case SCENE_GAMEOVER:
      UpdateGameOver(scene);
      break;
    case SCENE_ENDGAME:
      UpdateEndGame(scene);
      break;
    case SCENE_STARTGAME:
      UpdateStartGame(scene);
      break;
    case SCENE_ABOUT:
      UpdateAbout(scene);
      break;
  }
}

void SceneDraw(Scene *scene){
  switch(scene->sceneType){
    case SCENE_GAMEPLAY:
      DrawGamePlay(scene);
      break;
    case SCENE_MAINMENU:
      DrawMainMenu(scene);
      break;
    case SCENE_PAUSED:
      DrawPaused(scene);
      break;
    case SCENE_NEXTLEVEL:
      DrawNextLevel(scene);
      break;
    case SCENE_GAMEOVER:
      DrawGameOver(scene);
      break;
    case SCENE_ENDGAME:
      DrawEndGame(scene);
      break;
    case SCENE_STARTGAME:
      DrawStartGame(scene);
      break;
    case SCENE_ABOUT:
      DrawAbout(scene);
      break;
  }
}
