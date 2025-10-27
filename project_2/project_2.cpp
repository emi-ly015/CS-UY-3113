
/**
* Author: Emily Lopez
* Assignment: Pong Clone
* Date due: 2025-10-13, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "CS3113/cs3113.h"

enum BallCount {ONE = 1, TWO =2 , THREE = 3};

// Global Constants
constexpr int SCREEN_WIDTH  = 1400,
              SCREEN_HEIGHT = 700,
              FPS           = 60,
              SIZE          = 300 / 2,
              SPEED         = 400,
              BALL_SPEED    = 500,
              MAX_BALLS     = 3;

constexpr char    BG_COLOUR[]    = "#000000";
constexpr Vector2 ORIGIN         = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  BASE_SIZE      = { (float) SIZE, (float) SIZE },
                  INIT_POS       = { ORIGIN.x - 600 , ORIGIN.y },
                  PLAYER2_INIT_POS = { ORIGIN.x + 600, ORIGIN.y },
                  BALL_INIT_POS = {ORIGIN.x, ORIGIN.y };

constexpr char PLAYER1_FP[]  = "assets/bara.png";
constexpr char PLAYER2_FP[] = "assets/purple_bara.png";
constexpr char BALL_FP[] = "assets/orange.png";
constexpr char BG_FP[] = "assets/garden_pixel.png";
constexpr char PLAYER1WIN_FP[] = "assets/player1_win.png";
constexpr char PLAYER2WIN_FP[] = "assets/player2_win.png";


// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gAngle         = 0.0f,
          gBallAngle     = 0.0f,
          gPreviousTicks = 0.0f;

Vector2 gPosition  = INIT_POS,
        gMovement  = { 0.0f, 0.0f },
        gScale     = { BASE_SIZE.x , BASE_SIZE.y },

        gPlayer2Position = PLAYER2_INIT_POS,
        gPlayer2Movement = { 0.0f, 0.0f },
        gPlayer2Scale    = { BASE_SIZE.x , BASE_SIZE.y },

        gBallPositions[MAX_BALLS] = {BALL_INIT_POS, BALL_INIT_POS, BALL_INIT_POS},
        gBallMovement[MAX_BALLS] = {{ 1.0f, 0.2f }, { -1.0f, 0.2f }, { 1.0f, 0.0f }},
        gBallScale = { BASE_SIZE.x / 2 , BASE_SIZE.y / 2},

        gBackgroundPosition = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f},
        gBackgroundScale = { (float) SCREEN_WIDTH , (float)SCREEN_HEIGHT },

        gMousePosition = GetMousePosition(),

        // screen dimensions for collision detection
        gTop = { SCREEN_WIDTH / 2.0f, 0.0f },
        gBottom = { SCREEN_WIDTH / 2.0f, (float) SCREEN_HEIGHT },
        gWallScale = { (float) SCREEN_WIDTH, 0.0f };

Texture2D gTexture;
Texture2D gPlayer2Texture;
Texture2D gBallTexture;
Texture2D gBackgroundTexture;
Texture2D gPlayer1WinTexture;
Texture2D gPlayer2WinTexture;

BallCount gTotalBalls = ONE;

unsigned int startTime;

// single-player mode
bool singlePlayerMode = false;
// counts how many active balls 
bool gBallActive[MAX_BALLS] = {true, false, false};

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
bool isColliding(const Vector2 *postionA,  const Vector2 *scaleA, const Vector2 *positionB, const Vector2 *scaleB);

bool isColliding(const Vector2 *postionA,  const Vector2 *scaleA, 
                 const Vector2 *positionB, const Vector2 *scaleB)
{
    float xDistance = fabs(postionA->x - positionB->x) - 
                      ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(postionA->y - positionB->y) - 
                      ((scaleA->y + scaleB->y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}
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
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong Clone");

    gTexture = LoadTexture(PLAYER1_FP);
    gPlayer2Texture = LoadTexture(PLAYER2_FP);
    gBallTexture = LoadTexture(BALL_FP);
    gBackgroundTexture = LoadTexture(BG_FP);
    gPlayer1WinTexture = LoadTexture(PLAYER1WIN_FP);
    gPlayer2WinTexture = LoadTexture(PLAYER2WIN_FP);
    
    SetTargetFPS(FPS);
    
}

void processInput() 
{   
    gMovement = {0.0f, 0.0f};

    // single-player switch
    if (IsKeyDown(KEY_T)){
        singlePlayerMode = true;
        if (singlePlayerMode && gPlayer2Movement.y == 0.0f){
            gPlayer2Movement.y = 1.0f;
        }
    }

    // player 1 movement
    if (IsKeyDown(KEY_W)) gMovement.y = -1;
    else if (IsKeyDown(KEY_S)) gMovement.y = 1;

    if (singlePlayerMode == false)
    {
        gPlayer2Movement =  {0.0f, 0.0f};
        // player 2 movement
        if (IsKeyDown(KEY_UP)) gPlayer2Movement.y = -1;
        if (IsKeyDown(KEY_DOWN)) gPlayer2Movement.y = 1;
    }
    
    // number keys from 1-3 generate that number of balls
    if (IsKeyDown(KEY_ONE)) gTotalBalls = ONE;
    if (IsKeyDown(KEY_TWO)) gTotalBalls = TWO;
    if (IsKeyDown(KEY_THREE)) gTotalBalls = THREE;

    // activate or deactive the current number of balls
    for (int i = 0; i < MAX_BALLS; i++)
    {
       if ( i < gTotalBalls) gBallActive[i] = true;
       else gBallActive[i] = false; 
    }

    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    // delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // old positions
    Vector2 oldPosition1 = gPosition;
    Vector2 oldPosition2 = gPlayer2Position;

    gPosition.y += SPEED * gMovement.y * deltaTime;
    gPlayer2Position.y += SPEED * gPlayer2Movement.y * deltaTime;

    // collision with top or bottom wall for player 1
    if (isColliding(&gPosition, &gScale, &gTop, &gWallScale) || 
        isColliding(&gPosition, &gScale, &gBottom, &gWallScale))
        {
            gPosition = oldPosition1;
        }
    // collision with top or bottom wall for player 2
    if (singlePlayerMode == false && (isColliding(&gPlayer2Position, &gPlayer2Scale, &gTop, &gWallScale) || 
        isColliding(&gPlayer2Position, &gPlayer2Scale, &gBottom, &gWallScale)))
        {
            gPlayer2Position = oldPosition2;
        }
    
    // update ball movement
    for (int i = 0; i < MAX_BALLS; i++) 
    {
        // check how many balls active
        if (gBallActive[i])
        {
            gBallPositions[i].x += BALL_SPEED * gBallMovement[i].x * deltaTime;
            gBallPositions[i].y += BALL_SPEED * gBallMovement[i].y * deltaTime;

            // bounce ball off the top and bottom of the screen
            if (gBallPositions[i].y <= gBallScale.y / 2.0f || 
            gBallPositions[i].y >= SCREEN_HEIGHT - gBallScale.y / 2.0f) gBallMovement[i].y *= -1;
            
            // ball collides with a player
            if (isColliding(&gPosition, &gScale, &gBallPositions[i], &gBallScale) || 
                isColliding(&gPlayer2Position, &gPlayer2Scale, &gBallPositions[i], &gBallScale))
            {
                gBallMovement[i].x *= -1;
                float angle_difference = gBallPositions[i].y - gPosition.y;
                gBallMovement[i].y = angle_difference / (gScale.y / 0.5f);
                
            }

            // game stops when the ball hits a wall
            if (gBallPositions[i].x > SCREEN_WIDTH - gBallScale.x / 2.0f)
            {
                gAppStatus = PLAYER1_WIN;
            } 
            else if(gBallPositions[i].x <= gBallScale.x / 2.0f )
            {
                gAppStatus = PLAYER2_WIN;
            }
            
        }
        
    }
    
    // player 2 moves in an up and down motion when in single player mode
    if (singlePlayerMode)
    {
        if (gPlayer2Position.y + gPlayer2Scale.y / 2 >= SCREEN_HEIGHT) 
        {
            gPlayer2Movement.y = -1.0f;
        } 
        else if (gPlayer2Position.y - gPlayer2Scale.y / 2 <= 0)
        {
            gPlayer2Movement.y = 1.0f;
        }
    }

}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    if (gAppStatus == PLAYER1_WIN){
        // show when player 1 wins
        renderObject(&gPlayer1WinTexture, &gBackgroundPosition, &gBackgroundScale);
    } else if (gAppStatus == PLAYER2_WIN){
        // show when player 2 wins
        renderObject(&gPlayer2WinTexture, &gBackgroundPosition, &gBackgroundScale);
    }
    else {
        //background
        renderObject(&gBackgroundTexture, &gBackgroundPosition, &gBackgroundScale);

        // render player 1
        renderObject(&gTexture, & gPosition, &gScale);

        // render player 2
        renderObject(&gPlayer2Texture, &gPlayer2Position, &gPlayer2Scale);

        // ball
        for (int i = 0; i < MAX_BALLS; i ++)
        {
            if (gBallActive[i])
            {
                renderObject(&gBallTexture, &gBallPositions[i], &gBallScale);
            }
        }

    }

    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow(); 
    UnloadTexture(gTexture);
    UnloadTexture(gPlayer2Texture);
    UnloadTexture(gBallTexture);
    UnloadTexture(gBackgroundTexture);
    UnloadTexture(gPlayer1WinTexture);
    UnloadTexture(gPlayer2WinTexture);
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
    while (gAppStatus == PLAYER1_WIN || gAppStatus == PLAYER2_WIN)
    {
        processInput();
        render();
    }

    shutdown();

    return 0;
}