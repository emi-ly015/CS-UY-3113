/**
* Author: Emily Lopez
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/Menu.h"

// Global Constants
constexpr int SCREEN_WIDTH     = 1000,
              SCREEN_HEIGHT    = 600,
              FPS              = 120,
              NUMBER_OF_LEVELS = 4;

constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

Menu   *gMenu   = nullptr;
LevelA *gLevelA = nullptr;
LevelB *gLevelB = nullptr;
LevelC *gLevelC = nullptr;

int gLives = 3;
bool gWin = false;
bool gLose = false;

Music gBGM;
Sound gFallSound;
Sound gWinSound;
Sound gLoseSound;


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
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scenes");
    InitAudioDevice();

    gBGM        = LoadMusicStream("assets/project4_assets/Comfortable Mystery.wav");
    gFallSound  = LoadSound("assets/project4_assets/impactfall1.wav");
    gWinSound = LoadSound("assets/project4_assets/Well Done CCBY3.wav");
    gLoseSound = LoadSound("assets/project4_assets/lose sound 1_0.wav");

    SetMusicVolume(gBGM, 0.9f);
    PlayMusicStream(gBGM);

    // add another level and menu screen
    gMenu   = new Menu(ORIGIN, "#f9badbff");
    gLevelA = new LevelA(ORIGIN, "#C0897E");
    gLevelB = new LevelB(ORIGIN, "#f7d3bbff");
    gLevelC = new LevelC(ORIGIN, "#cbabd5ff");

    gLevels.push_back(gMenu);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);

    switchToScene(gLevels[0]);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED; 
    // menu cannot process player movement
    if (gCurrentScene->getState().player == nullptr) return;

    gCurrentScene->getState().player->resetMovement();

    if      (IsKeyDown(KEY_A)) gCurrentScene->getState().player->moveLeft();
    else if (IsKeyDown(KEY_D)) gCurrentScene->getState().player->moveRight();

    if (IsKeyPressed(KEY_W) && 
        gCurrentScene->getState().player->isCollidingBottom())
    {
        gCurrentScene->getState().player->jump();
        PlaySound(gCurrentScene->getState().jumpSound);
    }

    if (GetLength(gCurrentScene->getState().player->getMovement()) > 1.0f) 
        gCurrentScene->getState().player->normaliseMovement();
}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    UpdateMusicStream(gBGM);

    // check if player fell off the platform
        int nextID = gCurrentScene->getState().nextSceneID;

        // when a player falls, the scene ID is set to -1
        if (nextID == -1)
        {
            // sound for falling and hitting entities
            PlaySound(gFallSound);
            gLives--;
            // if the player still has lives, they can restart the scene
            if (gLives > 0)
            {
                if (gCurrentScene == gLevelA) switchToScene(gLevelA);
                else if (gCurrentScene == gLevelB) switchToScene(gLevelB);
                else if (gCurrentScene == gLevelC) switchToScene(gLevelC);
                    
            } else {
                // if there are no lives left, we restart the game
                PlaySound(gLoseSound);
                gLose = true;
                gLives = 3;
                switchToScene(gMenu);
                    
            }
        }
    // completing level 3 displays winning text
    if (gCurrentScene == gLevelC && nextID == 4)
    {
        PlaySound(gWinSound);
        gWin = true;
        switchToScene(gMenu);
        gLives = 3; 
    }
}

void render()
{
    BeginDrawing();

    // no camera on menu screen. text will not load otherwise
    if (gCurrentScene == gMenu)
    {
        gCurrentScene->render();
    }
    else {
        BeginMode2D(gCurrentScene->getState().camera);
        gCurrentScene->render();
        EndMode2D();
    }

    if (gWin)
        DrawText("You win!", SCREEN_WIDTH/ 2 - 90, SCREEN_HEIGHT/ 2 - 260, 50, WHITE);
    else if (gLose)
        DrawText("You lose! :(", SCREEN_WIDTH/ 2 - 140, SCREEN_HEIGHT/ 2 - 260, 50, WHITE);

    EndDrawing();
} 

void shutdown() 
{
    delete gMenu;
    delete gLevelA;
    delete gLevelB;
    delete gLevelC;

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

        if (gCurrentScene->getState().nextSceneID > 0)
        {   
            int id = gCurrentScene->getState().nextSceneID;
            // prevent win/lose text from reappearing when starting a new game
            if (gCurrentScene == gMenu && id ==1)
            {
                gLose = false;
                gWin  = false;
                gLives = 3;
            }
            switchToScene(gLevels[id]);
        }

        render();
    }

    shutdown();

    return 0;
}