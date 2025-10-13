#include "CS3113/cs3113.h"
#include <math.h>

// Global Constants
constexpr int SCREEN_WIDTH  = 1600 / 2,
              SCREEN_HEIGHT = 900 / 2,
              FPS           = 60,
              SIZE          = 500 / 2,
              SPEED         = 200;

constexpr char    BG_COLOUR[]    = "#F8F1C8";
constexpr Vector2 ORIGIN         = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  BASE_SIZE      = { (float) SIZE, (float) SIZE },
                  LINK_INIT_POS  = { ORIGIN.x - 200.0f, ORIGIN.y },
                  RUPEE_INIT_POS = { ORIGIN.x + 250.0f, ORIGIN.y };

// Images owned by Nintendo — please don't sue me.
constexpr char LINK_FP[]  = "assets/link.png";
constexpr char RUPEE_FP[] = "assets/rupee.png";

// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gAngle         = 0.0f,
          gPreviousTicks = 0.0f;

Vector2 gLinkPosition  = LINK_INIT_POS,
        gLinkScale     = BASE_SIZE,
        gLinkMovement  = { 0.0f, 0.0f },

        gRupeePosition = RUPEE_INIT_POS,
        gRupeeMovement = { 0.0f, 0.0f },
        gRupeeScale    = { BASE_SIZE.x / 2.0f , BASE_SIZE.y / 2.0f * 1.6f},

        gMousePosition = GetMousePosition();

Texture2D gLinkTexture;
Texture2D gRupeeTexture;


unsigned int startTime;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA, const Vector2 *positionB, const Vector2 *scaleB);



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

    // Destination rectangle – centred on gPosition
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
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

    gLinkTexture = LoadTexture(LINK_FP);
    gRupeeTexture = LoadTexture(RUPEE_FP);
    

    SetTargetFPS(FPS);
    
}

void processInput() 
{   
    gLinkMovement = { 0.0f, 0.0f };
    gMousePosition = GetMousePosition();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        if (gLinkPosition.x < gMousePosition.x) gLinkMovement.x = 1;
        if (gLinkPosition.x > gMousePosition.x) gLinkMovement.x = -1;
        if (gLinkPosition.y < gMousePosition.y) gLinkMovement.y = 1;
        if (gLinkPosition.y > gMousePosition.y) gLinkMovement.x = -1;
    }

    // holding the key down
    if (IsKeyDown(KEY_A))       gLinkMovement.x = -1.0f; // left
    else if (IsKeyDown(KEY_D))  gLinkMovement.x = 1.0f; // right

    if (IsKeyDown(KEY_W))  gLinkMovement.y = -1.0f; // down
    else if (IsKeyDown(KEY_S))  gLinkMovement.y = 1.0f; // up

    // prevents diagonal movement from being faster but need to addd two functions
    //if (GetLength(&gLinkMovement) > 1.0f) Normalise(&gLinkMovement);

    // printf("%s\n", IsKeyPressed(KEY_Q) ? "Yes, theyre pressing Q" : " ");
    if ( IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    gLinkPosition = {
        gLinkPosition.x + SPEED * deltaTime * gLinkMovement.x,
        gLinkPosition.y + SPEED * deltaTime * gLinkMovement.y
    };

    if (isColliding(
        &gLinkPosition,  &gLinkScale,
        &gRupeePosition, &gRupeeScale
    )) printf("Collision @ %us in game time.\n", (unsigned) time(NULL) - startTime);
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    // Render Link
    renderObject(&gLinkTexture, &gLinkPosition, &gLinkScale);

    // Render the rupee
    renderObject(&gRupeeTexture, &gRupeePosition, &gRupeeScale);

    EndDrawing();
}

void shutdown() { CloseWindow(); }

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


bool isColliding(const Vector2 *positionA,  const Vector2 *scaleA, 
                 const Vector2 *positionB, const Vector2 *scaleB)
{
    float xDistance = fabs(positionA->x - positionB->x) - ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(positionA->y - positionB->y) - ((scaleA->y + scaleB->y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}