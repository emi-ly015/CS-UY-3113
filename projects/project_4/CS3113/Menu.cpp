#include "Menu.h"

Menu::Menu()                                      : Scene { {0.0f}, nullptr   } {}
Menu::Menu(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
Menu::~Menu() {}

void Menu::initialise()
{
    // stay on menu screen until we press enter
    mGameState.nextSceneID = 0;
}

void Menu::update(float deltatime)
{
    if (IsKeyPressed(KEY_ENTER)) mGameState.nextSceneID = 1;
    
}

void Menu::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawRectangle(mOrigin.x - 400, mOrigin.y - 100, 800, 200, WHITE);
    DrawText("Project 4: Rise of the AI", mOrigin.x - 300, mOrigin.y - 60, 50, BLACK);
    DrawText("press ENTER to start", mOrigin.x - 125, mOrigin.y + 20, 20, GRAY);
}
void Menu::shutdown() {}