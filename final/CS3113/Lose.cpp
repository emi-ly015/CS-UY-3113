#include "Lose.h"

Lose::Lose()                                      : Scene { {0.0f}, nullptr   } {}
Lose::Lose(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
Lose::~Lose() {}

void Lose::initialise()
{
    // stay on screen until we press enter
    mGameState.nextSceneID = 0;
    mLoseTexture = LoadTexture("assets/project5_assets/loseScreen.png");

}

void Lose::update(float deltatime)
{
    if (IsKeyPressed(KEY_ENTER)) mGameState.nextSceneID = 1;
}

void Lose::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    Rectangle source = { 0, 0, (float) mLoseTexture.width, 
        (float) mLoseTexture.height };

    Rectangle dest = {mOrigin.x, mOrigin.y,
        (float) mLoseTexture.width * 0.75f, 
        (float) mLoseTexture.height * 0.75f };

    Vector2 origin = { ((float) mLoseTexture.width * 0.75f) / 2.0f,
        ((float) mLoseTexture.height * 0.75f) / 2.0f };

    DrawTexturePro(mLoseTexture, source, dest, origin, 0.0f, WHITE);
}


void Lose::shutdown() 
{
    UnloadTexture(mLoseTexture);
}