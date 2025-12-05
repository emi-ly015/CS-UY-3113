/**
* Author: Emily Lopez
* Assignment: Lunar Lander
* Date due: 2025-10-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Entity.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr char BG_COLOUR[]    = "#bababa";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr int   NUMBER_OF_TILES         = 18;
constexpr float TILE_DIMENSION          = 62.0f,
                ACCELERATION_OF_GRAVITY = 15.0f,
                // in m/ms², since delta time is in ms
                FIXED_TIMESTEP          = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f,
      gPlatformSpeed = 100.0f;


Entity *gPlayer = nullptr;
Entity *gTiles  = nullptr;

// background image
constexpr char BG_FP[] = "assets/project3_images/background.png";
Vector2 gBackgroundPosition = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f},
        gBackgroundScale = { (float) SCREEN_WIDTH , (float)SCREEN_HEIGHT };
float gAngle = 0.0f;
Texture2D gBackgroundTexture;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void renderObject(const Texture2D *texture, const Vector2 *position, 
                  const Vector2 *scale)
{
    // Whole texture (UV coordinates)
    Rectangle textureArea = {
        // top-left corner
        0.0f, 0.0f,

        // bottom-right corner (of texture)
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    // Destination rectangle â€“ centred on gPosition
    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        *texture, 
        textureArea, destinationArea, originOffset,
        gAngle, WHITE
    );
}


void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander");

    gBackgroundTexture = LoadTexture(BG_FP);
    /*
        ----------- player -----------
    */

    const std::vector<int> ANIMATIONS = { 0, 1, 2 };

    float sizeRatio  = 48.0f / 60.0f;

    gPlayer = new Entity(
        {ORIGIN.x, ORIGIN.y - 200.0f}, // position
        {70.0f , 150.0f},  // scale
        "assets/project3_images/Rocket (no alien).png",
        ATLAS,
        {1, 3},
        ANIMATIONS
    );

    gPlayer->setColliderDimensions({
        gPlayer->getScale().x / 6.0f,
        gPlayer->getScale().y / 6.0f
    });
    gPlayer->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    gPlayer->setFuel(100.0f);
    gPlayer->setFrameSpeed(7);

    /*
        ----------- TILES -----------
    */
    gTiles = new Entity[NUMBER_OF_TILES];

    // Compute the left‑most x coordinate so that the entire row is centred
    float leftMostX = ORIGIN.x - ((NUMBER_OF_TILES - 2) * TILE_DIMENSION) / 2.0f;
    float bottomY = SCREEN_HEIGHT - TILE_DIMENSION + 12.0f;

    for (int i = 0; i < NUMBER_OF_TILES - 1; i++) 
    {
        gTiles[i].setTexture("assets/project3_images/new_rock.png");
        gTiles[i].setScale({TILE_DIMENSION , TILE_DIMENSION });
        gTiles[i].setColliderDimensions({TILE_DIMENSION + 10.0f, TILE_DIMENSION});
        gTiles[i].setPosition({
            leftMostX + i * TILE_DIMENSION, bottomY + 17
        });

        // mark 3 tiles as the winning platform
        int middle = (NUMBER_OF_TILES - 1) / 2;
        if (i >= middle - 1 && i <= middle + 1)
        {
            gTiles[i].setGameStatus(ACCOMPLISHED);
            gTiles[i].setTexture("assets/project3_images/tiles.png");

        } else
        {
            gTiles[i].setGameStatus(FAILED);
        }
        if (i == 3 )
        {
            gTiles[i].setGameStatus(ACCOMPLISHED);
            gTiles[i].setTexture("assets/project3_images/tiles.png");
        }
        if (i == 2 || i == 13 )
        {
            gTiles[i].setGameStatus(ACCOMPLISHED);
            gTiles[i].setTexture("assets/project3_images/edgeTile.png");
        }
        if (i == 4 || i == 14 )
        {
            gTiles[i].setGameStatus(ACCOMPLISHED);
            gTiles[i].setTexture("assets/project3_images/rightEdge.png");
        }
    }

    // tile for the moving platform 
    
    gTiles[NUMBER_OF_TILES - 1].setTexture("assets/project3_images/Blue Planet.png");
    gTiles[NUMBER_OF_TILES - 1].setScale({TILE_DIMENSION+ 20.0f, TILE_DIMENSION + 10.0f});
    gTiles[NUMBER_OF_TILES - 1].setColliderDimensions({TILE_DIMENSION, TILE_DIMENSION});
    gTiles[NUMBER_OF_TILES - 1].setPosition({ORIGIN.y + TILE_DIMENSION, ORIGIN.y - TILE_DIMENSION});
    gTiles[NUMBER_OF_TILES - 1].setGameStatus(FAILED);
    gTiles[NUMBER_OF_TILES - 1].makeMovingPlatform(40.0f, 50.0f);


    SetTargetFPS(FPS);
}

void processInput() 
{
    Vector2 acceleration = { 0.0f, Entity::ACCELERATION_OF_GRAVITY };

    // we can only move with fuel
    if (gPlayer->hasFuel())
    {
        if      (IsKeyDown(KEY_A)) acceleration.x = -100.0f;
        else if (IsKeyDown(KEY_D))  acceleration.x = 150.0f;
    }
    
    gPlayer->setAcceleration(acceleration);
    

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gPlayer->update(FIXED_TIMESTEP, gTiles, NUMBER_OF_TILES);

        for (int i = 0; i < NUMBER_OF_TILES; i++) 
            gTiles[i].update(FIXED_TIMESTEP, nullptr, 0);


        // prevent player from leaving the screen
        Vector2 posPlayer = gPlayer->getPosition();
        Vector2 scale = gPlayer->getScale();

        if (posPlayer.x < scale.x / 2.0f) posPlayer.x = scale.x / 2.0f;
        if (posPlayer.x > SCREEN_WIDTH - scale.x / 2.0f) 
        posPlayer.x = SCREEN_WIDTH - scale.x / 2.0f;

        gPlayer->setPosition(posPlayer);

        deltaTime -= FIXED_TIMESTEP;
    }

}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    //background
    renderObject(&gBackgroundTexture, &gBackgroundPosition, &gBackgroundScale); 

    gPlayer->render();

    for (int i = 0; i < NUMBER_OF_TILES; i++) gTiles[i].render();

    // fuel UI
    float fuelPercent = gPlayer->getFuel() / 100.0f;

    // background of the fuel bar
    DrawRectangle(20, 20, 200, 20, GRAY);
    // current fuel level
    DrawRectangle(20, 20, 200 * fuelPercent, 20, 
                 fuelPercent > 0.3f ? GREEN : (fuelPercent > 0.1f ? ORANGE : RED));

   // fuel text
    if (gPlayer->getFuel() > 70) DrawText("FUEL: HIGH", 230, 20, 20, WHITE);
    else if (gPlayer->getFuel() > 30) DrawText("FUEL: MEDIUM", 230, 20, 20, WHITE);
    else if (gPlayer->getFuel() > 0) DrawText("FUEL: LOW", 230, 20, 20, WHITE);
    

    if (!gPlayer->hasFuel()) DrawText("Out of fuel :(", 
        SCREEN_WIDTH / 2 - 450, 50, 20, RED);
    
    if (gPlayer->getGameStatus() == FAILED) DrawText("Mission Failed :(", 
        SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 40, WHITE);
        
     else if (gPlayer->getGameStatus() == ACCOMPLISHED) DrawText("Mission Accomplished", 
        SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 40, WHITE);
    

    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}