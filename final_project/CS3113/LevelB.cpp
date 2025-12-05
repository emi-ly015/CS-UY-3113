#include "LevelB.h"

LevelB::LevelB()                                      : Scene { {0.0f}, nullptr   } {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelB::~LevelB() { shutdown(); }

void LevelB::initialise()
{
   // reseting level specific states for switching 
   // during presentation

   mission_wheat = false;
   mission_carrot = false;
   mission_lettuce = false;

   wheatCrops = 0;
   turnipCrops  = 0;
   peaches = 0;
   wood = 0;

   carrotSoldOut  = false;
   lettuceSoldOut = false;
   shopOpen = false;
   mMissionOpen = true;
   keysOpen = false;
   mGameState.plantEatenEffect = false;

   mPlants.clear();
   mTrees.clear();

   mGameState.nextSceneID = 0;
   mGameState.shovelSound = LoadSound("assets/project5_assets/squick_1.wav");
   mGameState.axeSound = LoadSound("assets/project5_assets/punch_1.wav");
   mGameState.waterSound = LoadSound("assets/project5_assets/blup_1.wav");

   mMissionHeader = LoadTexture("assets/project5_assets/UI_button.png");
   mMissionButton = LoadTexture("assets/project5_assets/mission_toggle_buttion.png");
   mMissionBox   = LoadTexture("assets/project5_assets/mission_details.png");
   missionDoneTex = LoadTexture("assets/project5_assets/missionDone.png");

   playerFace = LoadTexture("assets/project5_assets/emotes3.png");
   box = LoadTexture("assets/project5_assets/emote_box.png");

   starBox = LoadTexture("assets/project5_assets/starBox.png");
   starTexture = LoadTexture("assets/project5_assets/level2_stars.png");

   inventoryTexture =LoadTexture("assets/project5_assets/inventory.png");
   mSlotTexture = LoadTexture("assets/project5_assets/default_button.png");
   mSelectedSlotTexture = LoadTexture("assets/project5_assets/selected_button.png");
   mPlantsTexture = LoadTexture("assets/project5_assets/plants2.png");

   peachTreeTexture = LoadTexture("assets/project5_assets/modified_peach.png");
   normalTreeTexture = LoadTexture("assets/project5_assets/tree_fall_animation_sprite_sheet.png");
   peachIcon = LoadTexture("assets/project5_assets/peach.png");
   woodIcon  = LoadTexture("assets/project5_assets/wood_piece.png");

   shopTexture  = LoadTexture("assets/project5_assets/shop.png");
   clerkTexture = LoadTexture("assets/project5_assets/person.png");
   shopDetails = LoadTexture("assets/project5_assets/level1_shop.png");

   mKeysTexture = LoadTexture("assets/project5_assets/keys.png");

   // shop is initialized on top of the mountain
   shopWorldPosition = { 
      9.2 * TILE_DIMENSION + TILE_DIMENSION/2,
      -0.2 * TILE_DIMENSION + TILE_DIMENSION/2
   };

   for (int i = 0; i < 8; i++)
   {
      inventory[i] = -1;
      inventoryCount[i] = 0;
   }
   // give starter seeds
   addToInventory(ITEM_WHEAT_SEED,  4, wheatSeeds);
   addToInventory(ITEM_TURNIP_SEED, 4, turnipSeeds);
   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      LEVELB_WIDTH, LEVELB_HEIGHT,                   // map grid cols & rows
      (unsigned int *) mLevelData,                 // grid data
      "assets/project5_assets/actual_final_map.png", // texture filepath
      TILE_DIMENSION,                              // tile size
      61, 1,                                       // texture cols & rows
      mOrigin                                      // in-game origin
   );


   /*
   -------- TREES  --------
   */

   // level 1: 4 peach trees, 8 wood trees
   addTree(5, 4, 0);
   addTree(7, 5, 0);
   addTree(7, 3, 0);
   addTree(4, 9, 0);

   addTree(12, 7, 1);
   addTree(11, 10, 1);
   addTree(37, 13, 1);
   addTree(43, 6, 1);
   addTree(46, 8, 1);
   addTree(39, 5, 1);
   addTree(42, 3, 1);
   addTree(41, 9, 1);
   /*
      ----------- PLAYER -----------
   */
   std::map<Animation, std::vector<int>> playerAnimationAtlas = 
   {
      {WALK_LEFT,           { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {WALK_RIGHT,          { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {WALK_UP,             { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {WALK_DOWN,           { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {PLAYER_IDLE,         { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {PLAYER_SHOVEL_FRONT, { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {PLAYER_SHOVEL_BACK,  { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {PLAYER_AXE_FRONT,    { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {PLAYER_AXE_BACK,     { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {PLAYER_AXE_RIGHT,    { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {PLAYER_AXE_LEFT,     { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {PLAYER_WATER_FRONT,  { 1, 2, 3, 4, 5, 6, 7, 8 }},
      {PLAYER_WATER_BACK,   { 1, 2, 3, 4, 5, 6, 7, 8  }},
      {PLAYER_WATER_RIGHT,  { 1, 2, 3, 4, 5, 6, 7, 8  }},
      {PLAYER_WATER_LEFT,   { 1, 2, 3, 4, 5, 6, 7, 8  }}
   };

   std::map<Animation, std::string> playerTextures = 
   {
      {WALK_LEFT,  "assets/project5_assets/player_walk_left2.png"  },
      {WALK_RIGHT, "assets/project5_assets/player_walk_right2.png" },
      {WALK_UP,  "assets/project5_assets/player_walk_up2.png" },
      {WALK_DOWN, "assets/project5_assets/player_walk_down2.png"},
      {PLAYER_IDLE,   "assets/project5_assets/player_idle2.png"},
      
      {PLAYER_SHOVEL_FRONT,   "assets/project5_assets/player_shovel_front2.png"},
      {PLAYER_SHOVEL_BACK,   "assets/project5_assets/player_shovel_back.png"},

      {PLAYER_AXE_FRONT,   "assets/project5_assets/player_axe_front.png"},
      {PLAYER_AXE_BACK,   "assets/project5_assets/player_axe_back.png"},
      {PLAYER_AXE_LEFT,   "assets/project5_assets/player_axe_left.png"},
      {PLAYER_AXE_RIGHT,   "assets/project5_assets/player_axe_right.png"},

      {PLAYER_WATER_FRONT,   "assets/project5_assets/player_water_front.png"},
      {PLAYER_WATER_BACK,   "assets/project5_assets/player_water_back.png"},
      {PLAYER_WATER_LEFT,   "assets/project5_assets/player_water_left.png"},
      {PLAYER_WATER_RIGHT,   "assets/project5_assets/player_water_right.png"}
   };

   std::map<Animation, Vector2> playerSpritesheetDimensions = 
   {
      {WALK_LEFT,  { 1, 8 }},
      {WALK_RIGHT, { 1, 8 }},
      {WALK_UP,  { 1, 8 }},
      {WALK_DOWN, { 1, 8 }},
      {PLAYER_IDLE,   { 1, 8 }},
      {PLAYER_SHOVEL_FRONT,  { 1, 8 }},
      {PLAYER_SHOVEL_BACK,  { 1, 8 }},
      {PLAYER_AXE_FRONT,  { 1, 8 }},
      {PLAYER_AXE_BACK,  { 1, 8 }},
      {PLAYER_AXE_RIGHT,  { 1, 8 }},
      {PLAYER_AXE_LEFT,  { 1, 8 }},
      {PLAYER_WATER_FRONT,  { 1, 8 }},
      {PLAYER_WATER_BACK,  { 1, 8 }},
      {PLAYER_WATER_RIGHT,  { 1, 8 }},
      {PLAYER_WATER_LEFT,  { 1, 8 }},
   };

   float sizeRatio  = 48.0f / 64.0f;

   mGameState.player = new Entity(
      {mOrigin.x - 300.0f, mOrigin.y },       // position
      {150.0f * sizeRatio, 120.0f},                   // scale
      playerTextures,                                 // texture file address
      ATLAS,                                          // single image or atlas?
      playerSpritesheetDimensions,                    // atlas dimensions
      playerAnimationAtlas,                           // actual atlas
      PLAYER                                          // entity type
   );

   mGameState.player->setFrameSpeed(7);  
   mGameState.player->setJumpingPower(550.0f);
   mGameState.player->setColliderDimensions({
      mGameState.player->getScale().x / 3.5f,
      mGameState.player->getScale().y / 3.0f
   });
   mGameState.player->setSpeed(75.0f);

   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mGameState.player->getPosition(); // camera follows player
   mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 0.8f;                                // default zoom

   /*
      ----------- SHOP CLERK -----------
   */
   mGameState.clerk = new Entity(
      { shopWorldPosition.x, shopWorldPosition.y + 20.0f },
      { 90.0f, 120.0f },
      "assets/project5_assets/person.png",
       NPC
   );
   /*
      ----------- BUTTERFLY - FLYING AI -----------
   */
   std::map<Direction, std::vector<int>> flyerAnimationAtlas = {
      {RIGHT_DIRECTION, {0, 1, 2, 3, 4, 5, 6,7 }},
      {LEFT_DIRECTION, {0, 1, 2, 3, 4, 5, 6,7 }}

   };

   mGameState.butterfly = new EntityDirection (
      { mOrigin.x - 300.0f , mOrigin.y + 150.0f  },
      { 40.0f, 40.0f },
      "assets/project5_assets/butterfly.png",
      ATLAS,
      { 8, 1 },
      flyerAnimationAtlas,
      NPC_DIRECTION
   );
   mGameState.butterfly->setAIType(FLYER_DIRECTION);
   mGameState.butterfly->setAIState(WALKING_DIRECTION);
   mGameState.butterfly->setSpeed(10);
   mGameState.butterfly->setAcceleration({0.0f, 0.0f});

   /*
      ----------- COW - WANDERER AI -----------
   */
   std::map<Direction, std::vector<int>> cowAnimationAtlas = {
      { RIGHT_DIRECTION,  { 0, 1, 2, 3, 4, 5, 6, 7 }},
      { LEFT_DIRECTION, { 8, 9, 10, 11, 12, 13, 14, 15 }}, 
      { DOWN_DIRECTION,    { 0, 1, 2, 3, 4, 5, 6, 7 }},
      { UP_DIRECTION,  { 8, 9, 10, 11, 12, 13, 14, 15 }},
      { EAT_RIGHT_DIRECTION, { 16, 17, 18, 19 }},
      { EAT_LEFT_DIRECTION, { 24, 25, 26, 27 }}
   };
   mGameState.cow = new EntityDirection(
      { mOrigin.x, mOrigin.y + 150.0f },
      { 80.0f, 80.0f },
      "assets/project5_assets/cow.png",
      ATLAS,
      { 4, 8 },
      cowAnimationAtlas,
      NPC_DIRECTION
   );
   mGameState.cow->setAIType(WANDERER_DIRECTION);
   mGameState.cow->setAIState(WALKING_DIRECTION);
   mGameState.cow->setSpeed(40);
   // fencing prevents crop eating
   mGameState.cow->setColliderDimensions({
      mGameState.cow->getScale().x * 0.6f,
      mGameState.cow->getScale().y * 0.6f
   });
   cowLastSafePos = mGameState.cow->getPosition();

   // second cow
   cow2 = new EntityDirection(
      { mOrigin.x + 200.0f, mOrigin.y + 150.0f },
      { 80.0f, 80.0f },
      "assets/project5_assets/cow.png",
      ATLAS,
      { 4, 8 },
      cowAnimationAtlas,
      NPC_DIRECTION
   );
   cow2->setAIType(WANDERER_DIRECTION);
   cow2->setAIState(WALKING_DIRECTION);
   cow2->setSpeed(40);
   cow2->setColliderDimensions({
      cow2->getScale().x * 0.6f,
      cow2->getScale().y * 0.6f
   });
   cow2LastSafePos = cow2->getPosition();

   /*
      ----------- CHICKEN - WANDERER AI -----------
   */
   std::map<Direction, std::vector<int>> chickenAnimationAtlas  = {
      {IDLING_DIRECTION, {0, 1, 2, 3, 4, 5, 6}},
      { RIGHT_DIRECTION, {8, 9, 10, 11, 12, 13, 14, 15}},
      {LEFT_DIRECTION, {16, 17, 18, 19, 20, 21, 22, 23}},
      {PET, {24, 25, 26, 27, 28, 29, 30}}
   };
   mGameState.chicken = new EntityDirection
   {
      { mOrigin.x - 350.0f , mOrigin.y - 200.0f },
      { 50.0f, 90.0f },
      "assets/project5_assets/chicken.png",
      ATLAS,
      { 4, 8 },
      chickenAnimationAtlas,
      NPC_DIRECTION

   };
   // remains as pet due to level 1 mission
   mGameState.chicken->setAIType(FOLLOWER_DIRECTION);
   chickenIsPet = true;
   mGameState.chicken->setAIState(IDLE_DIRECTION);
   mGameState.chicken->setSpeed(40);
   mGameState.chicken->setDirection(IDLING_DIRECTION);
  
}

void LevelB::update(float deltaTime)
{
   // crops/ plants
   updatePlanting();
   updatePlantGrowth(deltaTime);
   updateWatering();
   updateHarvesting();

   // trees
   updateTreesAnimation(deltaTime);
   updateTreeInteraction();
   updateTreeCollection();

   // level specific input (shop, trading, feeding)
   levelInput();
   cowBehavior(deltaTime);

   if (IsKeyPressed(KEY_M)) mMissionOpen = !mMissionOpen;
   if (IsKeyPressed(KEY_L)) keysOpen = !keysOpen;
   
   for (int i = 0; i < 9; i++) if (IsKeyPressed(KEY_ONE + i))
      selectedSlot = i + 1;
   
   playerLastSafePos = mGameState.player->getPosition();
   mGameState.player->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );
   mGameState.butterfly->flyDirection(mOrigin.x - 800.0f, 
      mOrigin.x + 800.0f);

   mGameState.butterfly->update(
      deltaTime,
      nullptr, 
      mGameState.map, 
      nullptr,
      0
   );
   playerTreeCollision();
   
   mGameState.cow->update(
      deltaTime,
      nullptr,
      mGameState.map,
      nullptr,
      0
   );

   if (cow2) cow2->update(
      deltaTime, nullptr,
      mGameState.map, nullptr, 0
   );

   // before the chicken is a pet, pass nullptr so it doesn't follow anything
   Entity* followTarget = chickenIsPet ? mGameState.player : nullptr;

   mGameState.chicken->update(
      deltaTime,
      followTarget,
      mGameState.map,
      nullptr,
      0
   );

   Vector2 currentPlayerPosition = mGameState.player->getPosition();
   panCamera(&mGameState.camera, &currentPlayerPosition);

   faceTimer += deltaTime;
   if (faceTimer >= 0.25)
   {
      faceFrame++;
      faceTimer -= 0.25; 

      if (faceFrame >= 9) faceFrame = 0;
   }

   // clerk animation
   clerkTimer += deltaTime;
   if (clerkTimer >= 0.25f)
   {
      clerkTimer = 0.0f;
      clerkFrame++;
      if (clerkFrame > 2) clerkFrame = 0;
   }
   // win condition
   if (mission_wheat && mission_carrot && mission_lettuce) mGameState.nextSceneID = 3;


   // lose condition
   bool noSeeds = wheatSeeds <= 0 && turnipSeeds <= 0 &&
      carrotSeeds <= 0 && lettuceSeeds <= 0;

   // check if any plants are still alive/not harvested
   bool hasActivePlants = false;
   for (const Plant& plant : mPlants)
   {
      if (!plant.harvested)
      {
         hasActivePlants = true;
         break;
      }
   }
   if (noSeeds && !hasActivePlants) mGameState.nextSceneID = 5;
}

void LevelB::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.map->render();
   
   renderPlants();
   renderTrees();
   renderShop();
   
   mGameState.butterfly->render();
   mGameState.cow->render();
   if (cow2) cow2->render();
   mGameState.chicken->render();
   mGameState.player->render();
}

void LevelB::drawUI()
{
   /*
      ----------- INVENTORY  -----------
   */
   Rectangle inventorySource = { 0, 0, (float) inventoryTexture.width,
   (float) inventoryTexture.height };
   Rectangle inventoryDest = { 250.0f, 475.0f, 500.0f, 135.0f };

   DrawTexturePro( inventoryTexture,
      inventorySource, inventoryDest,
      { 0,0 }, 0, WHITE );
   drawInventoryItems();

   /*
      ----------- STAR BOX -----------
   */
   Rectangle starBoxSource = { 0, 0, (float) starBox.width, 
         (float) starBox.height };
   Rectangle starBoxDest = { 170.0f, 0.0f, 250, 150};
   DrawTexturePro(
      starBox, starBoxSource,
      starBoxDest, { 0,0 }, 0, WHITE );

   Rectangle starSource = { 0, 0, (float) starTexture.width, 
         (float) starTexture.height };
   Rectangle starDest = { 214.0f, 22.0f, 170, 60};
   DrawTexturePro(
      starTexture, starSource,
      starDest, { 0,0 }, 0, WHITE );

   /*
      ----------- CHARACTER BOX -----------
   */
   Rectangle charBoxSource = { 0, 0, (float) box.width, 
         (float) box.height };
   Rectangle charBoxDest = { -40.0f, 0.0f, 250, 150};

   DrawTexturePro(
      box, charBoxSource,
      charBoxDest, { 0,0 }, 0, WHITE );

   int frameWidth  = playerFace.width / 9;
   int frameHeight = playerFace.height;

   Rectangle faceSrc = {
      (float) (faceFrame * frameWidth), 0,
      (float) frameWidth, (float) frameHeight };
   // position inside the char box
   Rectangle faceDst = {
      charBoxDest.x + 90.0f,
      charBoxDest.y + 45.0f,
      64.0f, 64.0f };

   DrawTexturePro(
      playerFace, faceSrc,
      faceDst, {0,0}, 0, WHITE );

   /*
      ----------- MISSION DETAILS -----------
   */
   if (mMissionOpen)
   {
      Rectangle boxSource = { 0, 0, (float) mMissionBox.width, 
         (float) mMissionBox.height };
      Rectangle boxDest = { 740.0f, 95.0f, 240, 300 };

      DrawTexturePro(
         mMissionBox,
         boxSource, boxDest, { 0,0 },
         0, WHITE );
      // -------- Missions Text --------
      float textX = boxDest.x + 55;
      float textY = boxDest.y + 50;
      if (mission_wheat) DrawTexture(missionDoneTex, boxDest.x + 30, textY, BROWN);
      DrawText("Plant & harvest", textX, textY, 18, BROWN);
      textY += 22;
      DrawText("3 wheat", textX, textY, 18, BROWN);
      textY += 45;

      if (mission_carrot) DrawTexture(missionDoneTex, boxDest.x + 30, textY, BROWN);
      DrawText("Plant & harvest", textX, textY, 18, BROWN);
      textY += 22;
      DrawText("2 carrot", textX, textY, 18, BROWN);
      textY += 45;

      if (mission_lettuce) DrawTexture(missionDoneTex, boxDest.x + 30, textY, BROWN);
      DrawText("Plant & harvest", textX, textY, 18, BROWN);
      textY += 22;
      DrawText("4 lettuce", textX, textY, 18, BROWN);
      textY += 45;
   }

   /*
      ----------- MISSION HEADER -----------
   */
   Rectangle headerSource = { 3, 2, 90, 27 };
   Rectangle headerDest = { 750.0f , 70.0f , 200, 60 };
   DrawTexturePro(
      mMissionHeader,
      headerSource, headerDest,
      { 0, 0 }, 0, WHITE );

   DrawText("MISSION", headerDest.x + 36,
         headerDest.y + 16, 24,
         (Color){ 120, 90, 70, 255 });

   /*
      ----------- TOGGLE BUTTON -----------
   */
   Rectangle arrowSource = { 0, 0, 32, 32 };
   Rectangle arrowDest = { 890.0f , 53.0f , 96, 96 };

   DrawTexturePro(
      mMissionButton,
      arrowSource, arrowDest,
      { 0, 0 }, 0, WHITE );

   /*
      ----------- KEYS -----------
   */
   Rectangle settingSource = { 3, 2, 90, 27 };
   Rectangle settingDest = { 755.0f , 3.0f , 200, 60 };
   DrawTexturePro(
      mMissionHeader, settingSource,
      settingDest, { 0, 0 }, 0, WHITE
   );
   DrawText("KEYS",
         settingDest.x + 60,
         settingDest.y + 16,
         24, (Color) { 120, 90, 70, 255 });

   if (keysOpen)
   {
      Rectangle keySource = { 0, 0, (float) mKeysTexture.width, 
         (float) mKeysTexture.height };

      Rectangle keyDest = {mOrigin.x, mOrigin.y - 20.0f,
         ((float) mKeysTexture.width) * 0.5f,
         ((float) mKeysTexture.height ) * 0.5f
      };
      Vector2 origin = { keyDest.width / 2.0f, keyDest.height / 2.0f };

      DrawTexturePro(mKeysTexture, keySource, keyDest, origin, 0.0f, WHITE);
   }
   
   /*
      ----------- SHOP DETAILS -----------
   */
   if (shopOpen)
   {  

      Rectangle shopSource = {0, 0,
      (float) shopDetails.width,
      (float) shopDetails.height
      };
      Rectangle shopDest = { 750, 400,
         ((float) shopDetails.width) / 2.0f,
         ((float) shopDetails.height) / 2.0f
      };

      DrawTexturePro(shopDetails, shopSource, shopDest,
         { (float) shopDetails.width/2,
         (float) shopDetails.height/2} ,
         0, WHITE);

      int textX = (int)(shopDest.x - shopDest.width / 2 + 10);
      int textY = (int)(shopDest.y - shopDest.height / 2 + 10);
      if (!carrotSoldOut)
      {
         DrawText("Trade 5 wheat crops",
            textX + 105.0f, textY - 25, 14, WHITE);
         DrawText("Press B to purchase",
            textX + 105.0f, textY - 5, 14, WHITE);
      } else DrawText("CARROT : SOLD OUT", textX + 105.0f, textY - 5, 14, RED);
      if (!lettuceSoldOut)
      {
         DrawText("Trade 5 turnip crops",
            textX + 105, textY + 50, 14, WHITE);
         DrawText("Press V to purchase",
            textX + 105, textY + 70, 14, WHITE);
      } else DrawText("LETTUCE: SOLD OUT", textX + 105, textY + 70, 14, RED);
   }

}

void LevelB::shutdown()
{
   delete mGameState.player;
   delete mGameState.map;
   delete mGameState.butterfly; 
   delete mGameState.cow;
   if (cow2) delete cow2;
   delete mGameState.chicken;
   delete mGameState.clerk;
}

bool LevelB::isDirtTile(Vector2 worldPos)
{
   int tile = mGameState.map->getTileAtPosition(worldPos);

   // dirt tiles exist at tiles 30-42
   return (tile >= 30 && tile <= 42);
}

bool LevelB::isWaterTile(EntityDirection* entity, Vector2 worldPos, Direction moveDirection)
{
   Vector2 collider = entity->getColliderDimensions();
   float tileSize = mGameState.map->getTileSize();

   // how far ahead to check
   float yOffset = tileSize * 0.6f;
   float xOffset = collider.x * 1.2f;

    // shift probe depending on direction of movement
   if (moveDirection == DOWN_DIRECTION) worldPos.y += yOffset;
   else if (moveDirection == UP_DIRECTION) worldPos.y -= yOffset;

   // center , far left, far right, mid left, mid right
   Vector2 probes[] = {
      { worldPos.x, worldPos.y },
      { worldPos.x - xOffset,   worldPos.y },
      { worldPos.x + xOffset,   worldPos.y },
      { worldPos.x - xOffset/2, worldPos.y },
      { worldPos.x + xOffset/2, worldPos.y }
   };

   for (Vector2 probe : probes)
   {
      if (mGameState.map->getTileAtPosition(probe) == 0) return true;  
   }
   return false;
}

Vector2 LevelB::getDirtCenter(Vector2 worldPos)
{
   float size = mGameState.map->getTileSize();
   float left = mGameState.map->getLeftBoundary();
   float top = mGameState.map->getTopBoundary();

   int tileX = floor((worldPos.x - left) / size);
   int tileY = floor((worldPos.y - top) / size);

   return { left + tileX * size + size * 0.5f,
      top  + tileY * size + size * 0.5f };

}

bool LevelB::plantExistsAt(Vector2 pos)
{
   for (int i = 0; i < mPlants.size(); i++)
   {
      if (mPlants[i].harvested) continue;

      if (fabs(mPlants[i].position.x - pos.x) < 1.0f &&
         fabs(mPlants[i].position.y - pos.y) < 1.0f) return true;
   }

   return false;
}

void LevelB::updatePlanting()
{
   if (!IsKeyPressed(KEY_P)) return;

   if (isDirtTile(mGameState.player->getPosition()))
   {
      Vector2 center = getDirtCenter(mGameState.player->getPosition());
      if (!plantExistsAt(center))
     {
         int slotIndex = selectedSlot - 1;
         if (slotIndex < 0 || slotIndex >= 8) return;

         int typeToPlant = -1;
         int seedItemType = -1;
         int* seedCounterPtr = nullptr;
         int itemType = inventory[slotIndex];

         // wheat plant 
         if (itemType == ITEM_WHEAT_SEED && inventoryCount[slotIndex] > 0)
         {
            typeToPlant = 0;
            seedItemType = ITEM_WHEAT_SEED;
            seedCounterPtr = &wheatSeeds;
         }
         // turnip
         else if (itemType == ITEM_TURNIP_SEED && inventoryCount[slotIndex] > 0)
         {
            typeToPlant = 1;
            seedItemType = ITEM_TURNIP_SEED;
            seedCounterPtr = &turnipSeeds;
         }
         // carrot
         else if (itemType == ITEM_CARROT_SEED && inventoryCount[slotIndex] > 0)
         {
            typeToPlant = 2;
            seedItemType = ITEM_CARROT_SEED;
            seedCounterPtr = &carrotSeeds;
         }
         // lettuce
         else if (itemType == ITEM_LETTUCE_SEED && inventoryCount[slotIndex] > 0)
         {
            typeToPlant = 3;
            seedItemType = ITEM_LETTUCE_SEED;
            seedCounterPtr = &lettuceSeeds;
         }

         if (typeToPlant != -1)
         {
            Plant newPlant;
            newPlant.position = center;
            newPlant.plantType = typeToPlant;
            newPlant.growthPhase = 0;
            newPlant.watered = false;
            newPlant.growthTimer = 0;
            newPlant.harvested  = false;
            mPlants.push_back(newPlant);

            // consume one seed from this stack
            inventoryCount[slotIndex]--;
            if (seedCounterPtr) (*seedCounterPtr)--;

            if (inventoryCount[slotIndex] <= 0)
            {
               inventory[slotIndex]      = -1;
               inventoryCount[slotIndex] = 0;
            }
         }
      }
   }

}  

void LevelB::updatePlantGrowth(float deltaTime)
{
   for (int i = 0; i < mPlants.size(); i++)
   {
      if (mPlants[i].harvested) continue;
      if (mPlants[i].watered)
      {
         mPlants[i].growthTimer += deltaTime;
         // growth happens every 10 seconds
         if (mPlants[i].growthTimer >= 10.0f && mPlants[i].growthPhase < 3)
         {
            mPlants[i].growthPhase++;
            mPlants[i].growthTimer = 0;
         }
      }
   }
}

void LevelB::updateWatering()
{
   if (IsKeyPressed(KEY_T))
   {
      Vector2 tilePos = getDirtCenter(mGameState.player->getPosition());
      for (int i = 0; i < mPlants.size(); i++)
      {
         if (mPlants[i].harvested) continue;
         if (fabs(mPlants[i].position.x - tilePos.x) < 1.0f &&
            fabs(mPlants[i].position.y - tilePos.y) < 1.0f)
         {
            mPlants[i].watered = true;
            break;
         }
      }
   }
}

void LevelB::updateHarvesting()
{
   if (IsKeyPressed(KEY_H))
   {
      Vector2 tilePos = getDirtCenter(mGameState.player->getPosition());
      for (int i =0; i < mPlants.size(); i++)
      {
         if (mPlants[i].harvested) continue;
         // must be grown and standing on it
         if (mPlants[i].growthPhase == 3 &&
            fabs(mPlants[i].position.x - tilePos.x) < 1.0f &&
            fabs(mPlants[i].position.y - tilePos.y) < 1.0f)
         {
            int type = mPlants[i].plantType;
            // harvesting the same crop accumulates in the same slot
            int existingSlot = -1;
            int *globalCounter  = nullptr;

            if (type == 0) { existingSlot = 0; globalCounter = &wheatCrops; }
            if (type == 1) { existingSlot = 1; globalCounter = &turnipCrops; } 
            if (type == 2) { existingSlot = 4; globalCounter = &carrotCrops; } 
            if (type == 3) { existingSlot = 5; globalCounter = &lettuceCrops; } 

            if (existingSlot != -1 && globalCounter != nullptr) 
            addToInventory(existingSlot, 1, *globalCounter);
            
            // give seed back
            if (type == 0) addToInventory(ITEM_WHEAT_SEED,  1, wheatSeeds);
            if (type == 1) addToInventory(ITEM_TURNIP_SEED, 1, turnipSeeds);
            if (type == 2) addToInventory(ITEM_CARROT_SEED, 1, carrotSeeds);
            if (type == 3) addToInventory(ITEM_LETTUCE_SEED, 1, lettuceSeeds);

            mPlants[i].harvested = true;
            // level B mission completion check
            if (wheatCrops >= 3) mission_wheat = true;
            if (carrotCrops >= 2) mission_carrot = true;
            if (lettuceCrops >= 4) mission_lettuce = true;
            break;
         }
      }
   }
}

void LevelB::updateTreesAnimation(float deltaTime)
{
   for (int i = 0; i < mTrees.size(); i++)
   {
      if (!mTrees[i].animation) continue;
      mTrees[i].timer += deltaTime;

      // animation speed 
      if (mTrees[i].timer >= 0.20f)
      {
         mTrees[i].timer = 0.0f;
         mTrees[i].frame ++;
         // last frame reached so animation stops
         if (mTrees[i].frame >= 11)
         {
            mTrees[i].frame = 12;
            mTrees[i].animation = false;
            mTrees[i].fruitHarvested = true; 
         }
      }
   }
}

void LevelB::updateTreeInteraction()
{
   if (IsKeyPressed(KEY_O))
   {
      Vector2 playerPosition = mGameState.player->getPosition();

      for (int i = 0; i < mTrees.size(); i++)
      {
         float distanceX = fabs(mTrees[i].position.x - playerPosition.x);
         float distanceY = fabs(mTrees[i].position.y - playerPosition.y);

         // close enough to hit the tree
         if (distanceX < 60 && distanceY < 80)   
         {
            if (!mTrees[i].fruitHarvested && !mTrees[i].collected)
            {
               mTrees[i].animation = true;
               mTrees[i].frame = 0;
            }
            break;
         }
      }
   }
}

void LevelB::updateTreeCollection()
{
   Vector2 playerPosition = mGameState.player->getPosition();

   for (int i = 0; i < mTrees.size(); i++)
   {
      Tree &tree = mTrees[i];
      if (!tree.fruitHarvested) continue;

      float distanceX = fabs(playerPosition.x - mTrees[i].position.x);
      float distanceY = fabs(playerPosition.y - mTrees[i].position.y);

      // must be close enough to pick up
      if (distanceX > 40 || distanceY > 50) continue;

      //peach tree
      if (tree.treeType == 0)
      {
         addToInventory(2, 3, peaches);
         tree.frame = 12;
      } else if (tree.treeType == 1) addToInventory(3, 1, wood);  

      tree.fruitHarvested = false;
      tree.collected = true;
      
      break;
   }
}

void LevelB::drawInventoryItems()
{
   float spacing = 178.0f / 9.0f;
   float x = 260.0f + spacing;

   int iconW  = mPlantsTexture.width / 6;
   int iconH = mPlantsTexture.height / 7;

   const char* digitText[10] = { "0","1","2","3","4","5","6","7","8","9" };

   for (int i = 0; i < 8; i++)
   {
      Texture2D textureToUse = (selectedSlot == i + 1) ?
      mSelectedSlotTexture : mSlotTexture;

      Rectangle slotSource = { 0, 0, (float) textureToUse.width, 
         (float) textureToUse.height };
      Rectangle slotDest = { x - 4.0f , 502.0f, 50.0f, 50.0f };

      DrawTexturePro(
         textureToUse, slotSource, slotDest,
         {0,0}, 0, WHITE );
      
      Rectangle iconDest = { slotDest.x + 5.0f, slotDest.y + 5.0f, 40, 40 };
      
      /*
         ----------- NORMAL INV. SLOTS -----------
      */
      int type = inventory[i];
      if (type == -1)
      {
         x += 34.0f + spacing;
         continue;
      }
      // wheat
      if (type == 0)
      {
         Rectangle src = { 5.0f * iconW, 0.0f * iconH, 
            (float) iconW, (float) iconH };
         DrawTexturePro(mPlantsTexture, src, iconDest, {0,0}, 0, WHITE);
      }
      // turnip
      else if (type == 1)
      {
         Rectangle src = { 5.0f * iconW, 1.0f * iconH, (float) iconW, (float) iconH };
         DrawTexturePro(mPlantsTexture, src, iconDest, {0,0}, 0, WHITE);
      }
      // peach
      else if (type == 2)
      {
         Rectangle src = { 0, 0, (float) peachIcon.width, 
            (float) peachIcon.height };
         DrawTexturePro(peachIcon, src, iconDest, {0,0}, 0, WHITE);
      }
      // wood
      else if (type == 3)
      {
         Rectangle src = { 0, 0, (float) woodIcon.width, 
            (float) woodIcon.height };
         DrawTexturePro(woodIcon, src, iconDest, {0,0}, 0, WHITE);
      }
      // carrot
      else if (type == 4)
      {
         Rectangle src = { 5.0f * iconW, 2.0f * iconH,
            (float) iconW, (float) iconH };
         DrawTexturePro(mPlantsTexture, src, iconDest, {0,0}, 0, WHITE);
      }
      // lettuce
      else if (type == 5)
      {
         Rectangle src = { 5.0f * iconW, 3.0f * iconH,
            (float) iconW, (float) iconH };
         DrawTexturePro(mPlantsTexture, src, iconDest, {0,0}, 0, WHITE);
      }
      // wheat seed bag
      else if (type == ITEM_WHEAT_SEED)
      {
         Rectangle src = { 0.0f * iconW, 0.0f * iconH,
            (float) iconW, (float) iconH };
         DrawTexturePro(mPlantsTexture, src, iconDest, {0,0}, 0, WHITE);
      }
      // turnip seed bag
      else if (type == ITEM_TURNIP_SEED)
      {
         Rectangle src = { 0.0f * iconW, 1.0f * iconH,
            (float) iconW, (float) iconH };
         DrawTexturePro(mPlantsTexture, src, iconDest, {0,0}, 0, WHITE);
      }
      // carrot seed bag
      else if (type == ITEM_CARROT_SEED)
      {
         Rectangle src = { 0.0f * iconW, 2.0f * iconH,
            (float) iconW, (float) iconH };
         DrawTexturePro(mPlantsTexture, src, iconDest, {0,0}, 0, WHITE);
      }
      // lettuce seed bag
      else if (type == ITEM_LETTUCE_SEED)
      {
         Rectangle src = { 0.0f * iconW, 3.0f * iconH,
            (float) iconW, (float) iconH };
         DrawTexturePro(mPlantsTexture, src, iconDest, {0,0}, 0, WHITE);
      }

      if (inventoryCount[i] < 0) inventoryCount[i] = 0;
      if (inventoryCount[i] > 9) inventoryCount[i] = 9;
      DrawText(digitText[inventoryCount[i]], slotDest.x + 39, 
         slotDest.y + 5, 10, ColorFromHex("#cc9864"));

      x += 34.0f + spacing;
   }
}

int LevelB::getNextEmptySlot()
{
   // reserve slots 0–3 for seed bags
   for (int i = 0; i < 8; i++)
   {
      if (inventory[i] == -1) return i;
   }
   return -1;
}

void LevelB::addTree(int col, int row, int type)
{
   float tileSize = mGameState.map->getTileSize();
   float left = mGameState.map->getLeftBoundary();
   float top = mGameState.map->getTopBoundary();

   float x = left + col * tileSize * 0.5f;
   float y = top  + row * tileSize + tileSize * 0.5f;

   Tree tree; 
   tree.position = { x, y };
   tree.treeType = type;
   tree.frame = 0;
   tree.timer = 0.0f;
   tree.animation = false;
   tree.fruitHarvested = false;
   tree.collected     = false;

   mTrees.push_back(tree);

}

void LevelB::addToInventory(int itemType, int amount, int &counter)
{
   int existingSlot = -1;

   // fill existing stacks first (max 9 per stack)
   for (int slot = 0; slot < 8 && amount > 0; slot++)
   {
      if (inventory[slot] == itemType && inventoryCount[slot] < 9)
      {
         int space = 9 - inventoryCount[slot];
         int toAdd = (amount < space) ? amount : space;

         inventoryCount[slot] += toAdd;
         counter += toAdd;
         amount -= toAdd;
      }
   } 
   // create new stacks in empty slots
   while (amount > 0)
   {
      int newSlot = getNextEmptySlot();
      if (newSlot == -1) return;
      inventory[newSlot] = itemType;
      inventoryCount[newSlot] = 0;

      int toAdd = (amount > 9) ? 9 : amount;
      inventoryCount[newSlot] += toAdd;
      counter += toAdd;
      amount -= toAdd;
   }  
}

void LevelB::treeCollision(EntityDirection* entity, Vector2& lastSafePos)
{
   Vector2 position = entity->getPosition();

   for (int i = 0; i < mTrees.size(); i++)
   {
      Vector2 treePos = mTrees[i].position;

      float distX = fabs(position.x - treePos.x);
      float distY = fabs(position.y - treePos.y);
      if (distX < 55.0f && distY < 75.0f)
      {
         entity->setPosition(lastSafePos);
         Direction direction = entity->getDirection();
         switch (direction)
         {
         case LEFT_DIRECTION:
            entity->setDirection(RIGHT_DIRECTION);
            break;
         case RIGHT_DIRECTION:
            entity->setDirection(LEFT_DIRECTION);;
            break;
         case UP_DIRECTION:
            entity->setDirection(DOWN_DIRECTION);
            break;
         case DOWN_DIRECTION:
            entity->setDirection(UP_DIRECTION);
            break;
         default:
            break;
         }
         
         return;
      }
   }
}

void LevelB::playerTreeCollision()
{
   Vector2 position = mGameState.player->getPosition();

   for (int i = 0; i < mTrees.size(); i++)
   {
      float distX = fabs(position.x - mTrees[i].position.x);
      float distY = fabs(position.y - mTrees[i].position.y);

      if (distX < 45.0f && distY < 40.0f)
      {
         mGameState.player->setPosition(playerLastSafePos);
         return;
      }
   }
}

void LevelB::removeFromInventory(int itemType, int amount, int &counter)
{
   for (int slot = 0; slot < 8 && amount > 0; slot++)

   {
      if (inventory[slot] != itemType) continue;

      int remove = (amount < inventoryCount[slot]) ? amount : inventoryCount[slot];
      inventoryCount[slot] -= remove;
      counter -= remove;
      amount -= remove;

      if (inventoryCount[slot] <= 0)
      {
         inventory[slot]      = -1;
         inventoryCount[slot] = 0;
      }
   }
   if (counter < 0) counter = 0;
}

void LevelB::levelInput()
{
   shopToggle();
   shopTrading();

}

void LevelB::shopToggle()
{
   if (IsKeyPressed(KEY_N)) 
   {
      Vector2 playerPos = mGameState.player->getPosition();

      // must be close enough
      if (fabs(playerPos.x - shopWorldPosition.x) < 120 && 
          fabs(playerPos.y - shopWorldPosition.y) < 200)
      {
         shopOpen = !shopOpen; 
      }
   }
}

void LevelB::shopTrading()
{
   if (shopOpen && !carrotSoldOut && IsKeyPressed(KEY_B))
   {
      if (wheatCrops >= 5)
      {
         removeFromInventory(ITEM_WHEAT_CROP, 5, wheatCrops);
         addToInventory(ITEM_CARROT_SEED, 2, carrotSeeds);
         carrotSoldOut = true;
         
      }
   }
   if (shopOpen && !lettuceSoldOut && IsKeyPressed(KEY_V))
   {
      if (turnipCrops >= 5)
      {
         removeFromInventory(ITEM_TURNIP_CROP, 5, turnipCrops);
         addToInventory(ITEM_LETTUCE_SEED, 2, lettuceSeeds);
         lettuceSoldOut = true;
         
      }
   }
}

void LevelB::cowBehavior(float deltaTime)
{
   // gameState cow
   cowBehaviorSingle (mGameState.cow,
      cowLastSafePos, cowDecisionTimer,
      cowEating, cowEatingTimer, deltaTime );

   // new cow
   cowBehaviorSingle(cow2,cow2LastSafePos,
      cow2DecisionTimer, cow2Eating,
      cow2EatingTimer, deltaTime);
}

void LevelB::cowBehaviorSingle(EntityDirection* cow, Vector2& lastSafePos,
   float& decisionTimer, bool& eating, float& eatingTimer, float deltaTime)
{
   if (!cow) return;
   lastSafePos = cow->getPosition();
   if (!eating)
   {
      decisionTimer += deltaTime;
      if (decisionTimer >= 4.0f)
      {
         decisionTimer = 0.0f;

         // referenced GetRandomValue from https://www.raylib.com/cheatsheet/cheatsheet.html
         int choice = GetRandomValue(0, 9);
         if (choice <= 2) cow->setDirection(LEFT_DIRECTION);
         else if (choice <= 5) cow->setDirection(RIGHT_DIRECTION);
         else if (choice <= 7) cow->setDirection(UP_DIRECTION);
         else cow->setDirection(DOWN_DIRECTION);
      }
   }
   else
   {
      // when eating, keep the cow still
      eatingTimer += deltaTime;
      if (eatingTimer >= 0.6f)
      {
         eating      = false;
         eatingTimer = 0.0f;

         // when done eating, go back to a walk direction
         Direction facing = cow->getDirection();
         if (facing == EAT_LEFT_DIRECTION) cow->setDirection(LEFT_DIRECTION);
         else if (facing == EAT_RIGHT_DIRECTION) cow->setDirection(RIGHT_DIRECTION);
      }
   }
   Vector2 cowFeetPos = { cow->getPosition().x,
      cow->getPosition().y + cow->getColliderDimensions().y / 2.0f
   };
   if (isWaterTile(cow, cowFeetPos, cow->getDirection()))
   {
      cow->setPosition(lastSafePos);
      Direction currentDir = cow->getDirection();
      switch (currentDir)
      {
      case LEFT_DIRECTION:
         cow->setDirection(RIGHT_DIRECTION);
         break;
      case RIGHT_DIRECTION:
         cow->setDirection(LEFT_DIRECTION);
         break;
      case UP_DIRECTION:
         cow->setDirection(DOWN_DIRECTION);
         break;
      case DOWN_DIRECTION:
         cow->setDirection(UP_DIRECTION);
         break;
      default:
         break;
      }
   }
   treeCollision(cow, lastSafePos);
   // only eats a crop if it exists
   if (!eating && isDirtTile(cow->getPosition()))
   {
      Vector2 tileCenter = getDirtCenter(cow->getPosition());
      for (int i = 0; i < mPlants.size(); i++)
      {
         if (mPlants[i].harvested) continue;

         if (fabs(mPlants[i].position.x - tileCenter.x) < 1.0f &&
             fabs(mPlants[i].position.y - tileCenter.y) < 1.0f)
         {
            // destroy the crop
            mPlants[i].harvested = true;

            // decide which eating animation to use based on facing
            Direction facing = cow->getDirection();
            Direction eatDir =
               (facing == LEFT_DIRECTION || facing == DOWN_DIRECTION) 
               ? EAT_LEFT_DIRECTION : EAT_RIGHT_DIRECTION;

            // switch to eating animation and start timer
            cow->setDirection(eatDir);
            eating = true;
            mGameState.plantEatenEffect = true;
            eatingTimer = 0.0f;
            break;
         }
      }
   }

}

void LevelB::renderPlants()
{
   // plants
   int frameWidth  = mPlantsTexture.width / 6; 
   int frameHeight = mPlantsTexture.height / 7;

   for (int i = 0; i < mPlants.size(); i++)
   {
      if (mPlants[i].harvested) continue;
      Rectangle source = {
         (float) ((mPlants[i].growthPhase + 1) * frameWidth),
         (float) (mPlants[i].plantType * frameHeight),
         (float) frameWidth,
         (float) frameHeight
      };

      Rectangle dest = {
         mPlants[i].position.x - 20.0f,
         mPlants[i].position.y,
         (float) frameWidth * 4.0f,
         (float) frameHeight * 4.0f
      };

      // center the sprite around plant position
      Vector2 origin = { frameWidth / 2.0f, frameHeight / 2.0f };

      DrawTexturePro(
         mPlantsTexture,
         source,
         dest,
         origin,
         0.0f,
         WHITE
      );
   }
}

void LevelB::renderTrees()
{
   for (int i = 0; i < mTrees.size(); i++)
   {
      Texture2D texture = (mTrees[i].treeType == 0 ?
         peachTreeTexture : normalTreeTexture);

      int frameWidth  = texture.width / 13;
      int frameHeight = texture.height;

      Rectangle treeSource = {
         (float) (mTrees[i].frame * frameWidth),
         0, (float) frameWidth,
         (float) frameHeight
      };

      Rectangle treeDest = {mTrees[i].position.x,
         mTrees[i].position.y, frameWidth * 2.5f,
         frameHeight * 2.5f
      };

      Vector2 origin = { treeDest.width/2, treeDest.height/2 };
      DrawTexturePro(texture, treeSource, treeDest, origin, 0, WHITE);
   }
}

void LevelB::renderShop()
{
   float height = clerkTexture.height / 3;

   // shop clerk
   Rectangle clerkSource = {
      0, (float) clerkFrame * height,
      (float) clerkTexture.width,
      (float) height
   };
   Rectangle clerkDest = {
      shopWorldPosition.x,
      shopWorldPosition.y + 20.0f,
      clerkTexture.width * 3.0f,
      (clerkTexture.height / 3) * 3.0f
   };
   DrawTexturePro(clerkTexture, clerkSource, clerkDest, 
      {clerkDest.width / 2, clerkDest.height / 2}, 0, WHITE);
   
      Rectangle shopSource = {0, 0,
      (float) shopTexture.width,
      (float) shopTexture.height
      };
      Rectangle shopDest = { shopWorldPosition.x,
         shopWorldPosition.y + 75.0f,
         (100) * 2.0f, (35) * 2.0f
      };

      DrawText("press N to open the shop", clerkDest.x 
         - 60.f , clerkDest.y - 30.f, 10, WHITE);
      DrawTexturePro(shopTexture, shopSource, shopDest, 
         { shopDest.width / 2, shopDest.height / 2 }, 0, WHITE);
}