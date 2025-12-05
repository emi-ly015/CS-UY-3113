/**
* Author: Emily Lopez
* Assignment: [Your game's name here]
* Date due: Dec 5, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "CS3113/Menu.h"
#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/Win.h"
#include "CS3113/Lose.h"
#include "CS3113/ShaderProgram.h"
#include "CS3113/Effects.h"

// Global Constants
constexpr int SCREEN_WIDTH     = 1000,
              SCREEN_HEIGHT    = 600,
              FPS              = 120,
              NUMBER_OF_LEVELS = 4;

constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

enum SceneID { SCENE_MENU, SCENE_LEVEL_A,
    SCENE_LEVEL_B, SCENE_LEVEL_C, SCENE_WIN,
    SCENE_LOSE };

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

LevelA *gLevelA = nullptr;
LevelB *gLevelB = nullptr;
LevelC *gLevelC = nullptr;
Win *gWin = nullptr;
Lose *gLose = nullptr;
Menu *gMenu     = nullptr;

int gLastLevelID = 1;

Music gLev1BGM;
Music gLev2BGM;
Music gLev3BGM;
Sound gShovelSound;
Sound gAxeSound;
Sound gWaterSound;

ShaderProgram gShader;
Effects *gEffects = nullptr;

Vector2 gLightPosition = { 0.0f, 0.0f };

// monochrome effect timing
bool gMonochromeActive = false;
float gMonochromeTimer = 0.0f;
constexpr float MONO_DURATION = 1.0f;

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{   
    gCurrentScene = scene;
    gCurrentScene->initialise();
    if (scene == gLevelA) PlayMusicStream(gLev1BGM);
    else StopMusicStream(gLev1BGM);
    if (scene == gLevelB) PlayMusicStream(gLev2BGM);
    else StopMusicStream(gLev2BGM);
    if (scene == gLevelC) PlayMusicStream(gLev3BGM);
    else StopMusicStream(gLev3BGM);
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Final Project");
    InitAudioDevice();

    gLev1BGM = LoadMusicStream("assets/project5_assets/menuBGM.wav");
    gLev2BGM = LoadMusicStream("assets/project5_assets/level2BGM.wav");
    gLev3BGM = LoadMusicStream("assets/project5_assets/lev3BGM.wav");
    gShovelSound = LoadSound("assets/project5_assets/squick_1.wav");
    gAxeSound = LoadSound("assets/project5_assets/punch_1.wav");
    gWaterSound = LoadSound("assets/project5_assets/blup_1.wav");

    gShader.load("shaders/vertex.glsl", "shaders/fragment.glsl");
    gEffects = new Effects( ORIGIN, (float) SCREEN_WIDTH * 1.5f,
        (float) SCREEN_HEIGHT * 1.5f );

    gMenu = new Menu(ORIGIN, "#d8e5a9");
    gLevelA = new LevelA(ORIGIN, "#9fcfc3ff");
    gLevelB = new LevelB(ORIGIN, "#9fcfc3ff");
    gLevelC = new LevelC(ORIGIN, "#9fcfc3ff");
    gWin = new Win(ORIGIN, "#d8e5a9");
    gLose = new Lose(ORIGIN, "#d8e5a9");

    
    gLevels.push_back(gMenu);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);
    gLevels.push_back(gWin);
    gLevels.push_back(gLose); 

    switchToScene(gLevels[0]);


    SetTargetFPS(FPS);
}

void processInput() 
{
    // for presentation
    if (IsKeyPressed(KEY_Z)) switchToScene(gLevels[SCENE_LEVEL_A]);
    if (IsKeyPressed(KEY_X)) switchToScene(gLevels[SCENE_LEVEL_B]);
    if (IsKeyPressed(KEY_C)) switchToScene(gLevels[SCENE_LEVEL_C]);

    if (IsKeyPressed(KEY_COMMA)) switchToScene(gLevels[SCENE_WIN]);
    if (IsKeyPressed(KEY_PERIOD)) switchToScene(gLevels[SCENE_LOSE]);
    if (IsKeyPressed(KEY_DELETE)) switchToScene(gLevels[SCENE_MENU]);

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED; 
    // menu cannot process player movement
    if (gCurrentScene->getState().player == nullptr) return;

    gCurrentScene->getState().player->resetMovement();

    if      (IsKeyDown(KEY_A)) gCurrentScene->getState().player->moveLeft();
    else if (IsKeyDown(KEY_D)) gCurrentScene->getState().player->moveRight();
    if      (IsKeyDown(KEY_W)) gCurrentScene->getState().player->moveUp();
    else if (IsKeyDown(KEY_S)) gCurrentScene->getState().player->moveDown();


    if (IsKeyPressed(KEY_P)) PlaySound(gShovelSound);
    if (IsKeyPressed(KEY_O)) PlaySound(gAxeSound);
    if (IsKeyPressed(KEY_T)) PlaySound(gWaterSound);

    // p for shovel

    Animation facing = gCurrentScene->getState().player->getFacingAnimation();

    if (IsKeyDown(KEY_P))
    {
        if (facing == WALK_UP) gCurrentScene->getState().
        player->setAnimation(PLAYER_SHOVEL_BACK);
        else if (facing == WALK_DOWN)  gCurrentScene->getState().
        player->setAnimation(PLAYER_SHOVEL_FRONT);
        else if (facing == WALK_LEFT)  gCurrentScene->getState().
        player->setAnimation(PLAYER_SHOVEL_FRONT);
        else if (facing == WALK_RIGHT) gCurrentScene->getState().
        player->setAnimation(PLAYER_SHOVEL_FRONT);
    }
    // o for axe 
    else if (IsKeyDown(KEY_O))
    {
        if (facing == WALK_UP) gCurrentScene->getState().
        player->setAnimation(PLAYER_AXE_BACK);
        else if (facing == WALK_DOWN)  gCurrentScene->getState().
        player->setAnimation(PLAYER_AXE_FRONT);
        else if (facing == WALK_LEFT)  gCurrentScene->getState().
        player->setAnimation(PLAYER_AXE_LEFT);
        else if (facing == WALK_RIGHT) gCurrentScene->getState().
        player->setAnimation(PLAYER_AXE_RIGHT);
    }
    // t for watering can 
    else if (IsKeyDown(KEY_T))
    {
        if (facing == WALK_UP) gCurrentScene->getState().
        player->setAnimation(PLAYER_WATER_BACK);
        else if (facing == WALK_DOWN) gCurrentScene->getState().
        player->setAnimation(PLAYER_WATER_FRONT);
        else if (facing == WALK_LEFT) gCurrentScene->getState().
        player->setAnimation(PLAYER_WATER_LEFT);
        else if (facing == WALK_RIGHT) gCurrentScene->getState().
        player->setAnimation(PLAYER_WATER_RIGHT);
    }
    

    if (GetLength(gCurrentScene->getState().player->getMovement()) > 1.0f) 
        gCurrentScene->getState().player->normaliseMovement();
}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;
    UpdateMusicStream(gLev1BGM);
    UpdateMusicStream(gLev2BGM);
    UpdateMusicStream(gLev3BGM);
    
    if (gCurrentScene == gMenu)
    {
        gCurrentScene->update(deltaTime);
        return;
    }

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);

        if (gCurrentScene->getState().plantEatenEffect)
        {
            gMonochromeActive = true;
            gMonochromeTimer = MONO_DURATION;
            gCurrentScene->getState().plantEatenEffect = false;
        }
        if (gCurrentScene->getState().player != nullptr)
        {
            Vector2 cameraTarget = gCurrentScene->getState().player->getPosition();

            if (gEffects != nullptr) gEffects->update(FIXED_TIMESTEP, &cameraTarget);
            gLightPosition = cameraTarget;
        }
        if (gMonochromeActive)
        {
            gMonochromeTimer -= FIXED_TIMESTEP;
            if (gMonochromeTimer <= 0.0f)
            {
                gMonochromeTimer  = 0.0f;
                gMonochromeActive = false;
            }
        }
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();

    if (gCurrentScene == gMenu || gCurrentScene == gWin ||
        gCurrentScene == gLose)
    {
        gCurrentScene->render();
        gCurrentScene->drawUI();

    } else
    {
        BeginMode2D(gCurrentScene->getState().camera);
        if (gMonochromeActive) gShader.begin();

        gCurrentScene->render();

        if (gMonochromeActive) gShader.end();
        EndMode2D();
        gCurrentScene->drawUI();
    }
    EndDrawing();
}

void shutdown() 
{
    delete gLevelA;
    delete gLevelB;
    delete gLevelC;
    delete gWin;
    delete gLose;
    delete gMenu;
    
    if (gEffects != nullptr)
    {
        delete gEffects;
        gEffects = nullptr;
    }
    gShader.unload();

    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        
        int nextID = gCurrentScene->getState().nextSceneID;
        if (gCurrentScene == gWin && nextID == 0) switchToScene(gLevels[SCENE_MENU]);
        if (nextID > 0)
        {
            // currently on the lose screen and player pressed enter
            if (gCurrentScene == gLose)
            {
                // restart the level player last lost from
                switchToScene(gLevels[gLastLevelID]);
            } 
            // level sending player to the lose screen
            else if (nextID == SCENE_LOSE)
            {
                // remember which level player is coming from 
                for (int i = 0; i < (int)gLevels.size(); i++)
                {
                    if (gLevels[i] == gCurrentScene)
                    {
                        if (i == SCENE_LEVEL_A || i == SCENE_LEVEL_B || 
                            i == SCENE_LEVEL_C) gLastLevelID = i;
                        break;
                    }
                }
                switchToScene(gLevels[SCENE_LOSE]);
            }
            else switchToScene(gLevels[nextID]);
        }
        render();
    }

    shutdown();

    return 0;
}