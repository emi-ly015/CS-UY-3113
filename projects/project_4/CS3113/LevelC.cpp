#include "LevelC.h"

LevelC::LevelC()                                      : Scene { {0.0f}, nullptr   } {}
LevelC::LevelC(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelC::~LevelC() { shutdown(); }

void LevelC::initialise()
{
   mGameState.nextSceneID = 0;

   mGameState.jumpSound = LoadSound("assets/project4_assets/jump_02.wav");

   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/project4_assets/workpls.png",   // texture filepath
      TILE_DIMENSION,              // tile size
      13, 1,                        // texture cols & rows
      mOrigin                      // in-game origin
   );

   /*
      ----------- PROTAGONIST -----------
   */
  std::map<Direction, std::vector<int>> playerAnimationAtlas = {
      {DOWN,  { 4, 12, 20  }},
      {LEFT,  { 6, 14, 22 }},
      {UP,    { 0, 8, 16 }},
      {RIGHT, { 2, 10, 18 }},
   };

   float sizeRatio  = 48.0f / 64.0f;

   mGameState.player = new Entity(
      {mOrigin.x - 400.0f, mOrigin.y - 200.0f}, // position
      {100.0f * sizeRatio, 100.0f},             // scale
      "assets/project4_assets/character.png",              // texture file address
      ATLAS,                                    // single image or atlas?
      { 3, 8 },                                 // atlas dimensions
      playerAnimationAtlas,                    // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.player->setJumpingPower(550.0f);
   mGameState.player->setColliderDimensions({
      mGameState.player->getScale().x / 2.8f,
      mGameState.player->getScale().y / 1.5f
   });
   mGameState.player->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

   /*
      ----------- WANDERER -----------
   */
   std::map<Direction, std::vector<int>> flyerAnimationAtlas = {
      {LEFT,  {6, 7, 8, 9, 10 ,11 }},
      {RIGHT, {0, 1, 2, 3, 4, 5}}
   };

   mGameState.enemy = new Entity(
      { mOrigin.x - 100.0f, mOrigin.y - 200.0f },
      { 100.0f, 100.0f },
      "assets/project4_assets/Brown_Run.png",
      ATLAS,
      { 2, 6 },
      flyerAnimationAtlas,
      NPC
   );

   mGameState.enemy->setAIType(WANDERER);
   mGameState.enemy->setAIState(WALKING);
   mGameState.enemy->setSpeed(75);
   mGameState.enemy->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY}); 
   mGameState.enemy->setDirection(RIGHT);

   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mGameState.player->getPosition(); // camera follows player
   mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 1.0f;                                // default zoom
}

void LevelC::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);

   mGameState.player->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );

   // find where on the grid the player is standing
   int col = (int)(mGameState.player->getPosition().x / TILE_DIMENSION);
   // tile where the player's feet are standing on 
   int row = (int)((mGameState.player->getPosition().y 
   + TILE_DIMENSION) / TILE_DIMENSION);

   int index = row * mGameState.map->getMapColumns() + col;
   int tile  = mGameState.map->getLevelData()[index];

   // standing on tile 4 means the player passed the level
   if (tile == 4 && mGameState.player->isCollidingBottom())
   {
      mGameState.nextSceneID = 4;
   }

   // if the player falls, restart the level
   if (mGameState.player->getPosition().y > 800.0f)
   {
      mGameState.nextSceneID = -1;
   }

   mGameState.enemy->update(
   deltaTime,
   mGameState.player,
   mGameState.map,
   nullptr,
   0
   );

   // enemy walks back and forth on the same platform
   if (mGameState.enemy->getPosition().x < mOrigin.x - 100.0f) 
   mGameState.enemy->setDirection(RIGHT);
   else if (mGameState.enemy->getPosition().x > mOrigin.x + 100.0f)
   mGameState.enemy->setDirection(LEFT);

   if (mGameState.enemy->getDirection() == LEFT)
    mGameState.enemy->moveLeft();
   else mGameState.enemy->moveRight();

   // when a player collides with an enemy, lose a life
   if (mGameState.player->enemyCollision(mGameState.enemy))
   {
      mGameState.nextSceneID = -1;
   }

   Vector2 currentPlayerPosition = { mGameState.player->getPosition().x, mOrigin.y };

   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelC::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.player->render();
   mGameState.enemy->render();
   mGameState.map->render();
}

void LevelC::shutdown()
{
   delete mGameState.player;
   delete mGameState.map;
   delete mGameState.enemy;

   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
}