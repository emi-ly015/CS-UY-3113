/**
* Author: Emily Lopez
* Assignment: Simple 2D Scene
* Date due: 2025-09-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "CS3113/cs3113.h"


enum Direction { LEFT, RIGHT };

// Global Constants
constexpr int SCREEN_WIDTH        = 1450,
              SCREEN_HEIGHT       = 700,
              FPS                 = 60,
              SIZE                = 300;

constexpr float MAX_AMP           = 10.0f,
                RADIUS            = 150.0f, 
                ORBIT_SPEED       = 5.0f;

constexpr char BG_COLOUR[] = "#FFD1DC";
constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE = { (float)SIZE, (float)SIZE };

constexpr char CHIIKAWA_FP[] = "assets/chiikawa.png";
constexpr char HACHIWARE_FP[] = "assets/hachiware.png";
constexpr char USAGI_FP[] = "assets/usagi.png";

// Global Variables
AppStatus gAppStatus   = RUNNING;
float     gScaleFactor   = SIZE,
          gAngle         = 0.0f,
          gAngleRotation = 0.0f, // this is for texture 2 to rotatie
          gAngle2        = 0.0f,
          gAngle3        = 0.0f,
          gPulseTime     = 0.0f,
          gPulseTime2    = 0.0f,
          gVelocity3     = 200.0f;

// position for each character
Vector2   gPosition1      = ORIGIN;
Vector2   gPosition2      = ORIGIN;
Vector2   gPosition3      = ORIGIN;

Vector2   gScale         = BASE_SIZE;
Vector2   gScale2         = BASE_SIZE; // this is used for texture 3
float     gPreviousTicks = 0.0f;
Direction gDirection = RIGHT;  


// textures
Texture2D gTextureChiikawa;
Texture2D gTextureHachiware;
Texture2D gTextureUsagi;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "textures");

    gTextureChiikawa = LoadTexture(CHIIKAWA_FP);
    gTextureHachiware = LoadTexture(HACHIWARE_FP);
    gTextureUsagi = LoadTexture(USAGI_FP);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {

    // delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks; 
    
    // translation 


    // texture 1 will orbit
    // texture 1 will also transform in relation to texture 2
    gAngle  += ORBIT_SPEED * deltaTime;
    gPosition1.x  = gPosition2.x + RADIUS * cos(gAngle);
    gPosition1.y  = gPosition2.y + RADIUS * sin(gAngle);
    
    // texture 2 will translate in a zig-zag shape
    gAngle2  += ORBIT_SPEED * deltaTime;
    gPosition2.x = ORIGIN.x + (RADIUS + 400.0f) * sin(gAngle2);
    gPosition2.y += gScaleFactor * deltaTime;

    // reverse direction once we hit the bottom/top
    if (gPosition2.y < 0){
        gPosition2.y = 0;
        gScaleFactor = -gScaleFactor;
    } else if (gPosition2.y > SCREEN_HEIGHT){
        gPosition2.y = SCREEN_HEIGHT;
        gScaleFactor = -gScaleFactor; 
    }    

    // texture 3 will translate like a sin function
    gAngle3  += ORBIT_SPEED * deltaTime;
    gPosition3.x += gVelocity3 * deltaTime;
    gPosition3.y = ORIGIN.y + RADIUS * sin(gAngle3);

    // reverse direction once we hit the left/right
    if (gPosition3.x < 0){
        gPosition3.x = 0;
        gVelocity3 = -gVelocity3;
    } else if (gPosition3.x > SCREEN_WIDTH){
        gPosition3.x = SCREEN_WIDTH;
        gVelocity3 = -gVelocity3;
    }
    
    // rotation -> done on texture 2
    gAngleRotation += 500.0f * deltaTime * (gDirection == RIGHT ? 1 : -1);
    
    // scaling -> done on texture 3
    gPulseTime2 += 40.0f * deltaTime;
    gScale2 = {
        BASE_SIZE.x + MAX_AMP * cos(gPulseTime2),
        BASE_SIZE.y + MAX_AMP * cos(gPulseTime2)
    };
}

void render()
{
    BeginDrawing();
    
    ClearBackground(ColorFromHex(BG_COLOUR));
    
    // whole texture (UV coordinates)
    Rectangle textureAreaChiikawa = {
        // top-left corner
        0.0f, 0.0f,

        // bottom-right corner
        static_cast<float>(gTextureChiikawa.width),
        static_cast<float>(gTextureChiikawa.height)
    };
    Rectangle textureAreaHachiware = {
        // top-left corner
        0.0f, 0.0f,

        // bottom-right corner
        static_cast<float>(gTextureHachiware.width),
        static_cast<float>(gTextureHachiware.height)
    };
    Rectangle textureAreaUsagi = {
        // top-left corner
        0.0f, 0.0f,

        // bottom-right corner
        static_cast<float>(gTextureUsagi.width),
        static_cast<float>(gTextureUsagi.height)
    };

    // xy coordinates
    Rectangle destinationAreaChiikawa = {
        gPosition1.x,
        gPosition1.y,
        static_cast<float>(gScale.x),
        static_cast<float>(gScale.y)

    };
    Rectangle destinationAreaHachiware = {
        gPosition2.x,
        gPosition2.y,
        static_cast<float>(gScale.x),
        static_cast<float>(gScale.y)

    };
    Rectangle destinationAreaUsagi = {
        gPosition3.x - 200,
        gPosition3.y,
        static_cast<float>(gScale2.x),
        static_cast<float>(gScale2.y)

    };

    // rotational origin?
    Vector2 objectOrigin = {
        static_cast<float>(gScale.x) / 2.0f,
        static_cast<float>(gScale.y) / 2.0f
    };

    DrawTexturePro(
        gTextureChiikawa,
        textureAreaChiikawa,
        destinationAreaChiikawa,
        objectOrigin,
        gAngle,
        WHITE
    );

    DrawTexturePro(
        gTextureHachiware,
        textureAreaHachiware,
        destinationAreaHachiware,
        objectOrigin,
        gAngleRotation,
        WHITE
    );

    DrawTexturePro(
        gTextureUsagi,
        textureAreaUsagi,
        destinationAreaUsagi,
        objectOrigin,
        gAngle3,
        WHITE
    );

    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow(); // Close window and OpenGL context
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