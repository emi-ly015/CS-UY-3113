#include "Win.h"

Win::Win()                                      : Scene { {0.0f}, nullptr   } {}
Win::Win(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
Win::~Win() {}

void Win::initialise()
{
    // stay on  screen until we press enter
    mGameState.nextSceneID = 4;

    mWinTexture = LoadTexture("assets/project5_assets/winScreen.png");
}

void Win::update(float deltatime)
{
    if (IsKeyPressed(KEY_N)) mGameState.nextSceneID = 0;
    
}

void Win::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    Rectangle source = { 0, 0, (float) mWinTexture.width, 
        (float) mWinTexture.height };

    Rectangle dest = {mOrigin.x, mOrigin.y,
        (float) mWinTexture.width * 0.75f, 
        (float) mWinTexture.height * 0.75f };

    Vector2 origin = { ((float) mWinTexture.width * 0.75f) / 2.0f,
        ((float) mWinTexture.height * 0.75f) / 2.0f };

    DrawTexturePro(mWinTexture, source, dest, origin, 0.0f, WHITE);
}


void Win::shutdown() 
{
    UnloadTexture(mWinTexture);
}