#include "Menu.h"

Menu::Menu()                                      : Scene { {0.0f}, nullptr   } {}
Menu::Menu(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
Menu::~Menu() {}

void Menu::initialise()
{
    // stay on menu screen until we press enter
    mGameState.nextSceneID = 0;

    mMenuTexture = LoadTexture("assets/project5_assets/menu.png");
}

void Menu::update(float deltatime)
{
    if (IsKeyPressed(KEY_ENTER)) mGameState.nextSceneID = 1;
    
}

void Menu::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    Rectangle source = { 0, 0, (float) mMenuTexture.width, 
        (float) mMenuTexture.height };

    Rectangle dest = {mOrigin.x, mOrigin.y,
        (float) mMenuTexture.width * 0.5f, 
        (float) mMenuTexture.height * 0.5f };

    Vector2 origin = { ((float) mMenuTexture.width * 0.5f) / 2.0f,
        ((float) mMenuTexture.height * 0.5f) / 2.0f };

    DrawTexturePro(mMenuTexture, source, dest, origin, 0.0f, WHITE);
}


void Menu::shutdown() 
{
    UnloadTexture(mMenuTexture);
}