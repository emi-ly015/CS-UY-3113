#include "Scene.h"

#ifndef LEVELC_H
#define LEVELC_H

constexpr int LEVELC_WIDTH = 28,
              LEVELC_HEIGHT = 17;

class LevelC : public Scene {
private:
    unsigned int mLevelData[LEVELC_WIDTH * LEVELC_HEIGHT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2, 4, 4, 4, 4, 4, 4, 4, 4, 6, 0, 0, 0, 2, 4, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 2, 3, 14, 14, 14, 14, 14, 14, 14, 14, 13, 0, 0, 2, 3, 14, 14, 5, 4, 4, 6, 0, 0, 0, 0, 0, 0,
        0, 7, 17, 15, 14, 22, 14, 46, 47, 47, 48, 13, 0, 0, 7, 46, 47, 47, 48, 16, 14, 5, 6, 0, 0, 0, 0,
        0, 2, 3, 14, 20, 14, 14, 23, 49, 50, 50, 51, 13, 0, 0, 7, 49, 14, 14, 51, 14, 14, 14, 5, 6, 0, 0, 0,
        0, 7, 14, 31, 32, 32, 33, 14, 49, 50, 50, 51, 13, 0, 0, 7, 49, 14, 14, 51, 14, 14, 14, 14, 5, 6, 0, 0,
        0, 8, 9, 34, 30, 30, 35, 17, 43, 52, 53, 45, 13, 0, 0, 7, 43, 52, 53, 45, 14, 14, 14, 14, 14, 13, 0, 0,
        0, 0, 7, 36, 37, 37, 38, 14, 14, 56, 57, 14, 13, 0, 0, 7, 14, 56, 57, 14, 14, 14, 14, 14, 14, 13, 0, 0,
        0, 0, 7, 17, 15, 16, 15, 14, 14, 14, 14, 14, 13, 0, 0, 7, 31, 32, 32, 33, 16, 14, 14, 14, 11, 12, 0, 0,
        0, 0, 8, 9, 14, 14, 14, 14, 14, 14, 14, 14, 5, 4, 4, 3, 34, 30, 30, 61, 33, 14, 11, 10, 12, 0, 0, 0,
        0, 0, 0, 8, 9, 14, 14, 14, 14, 14, 14, 14, 17, 15, 16, 15, 36, 60, 30, 30, 35, 14, 13, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 7, 15, 22, 21, 14, 19, 14, 14, 11, 10, 10, 9, 25, 36, 37, 37, 38, 11, 12, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 8, 9, 14, 18, 21, 25, 14, 11, 12, 0, 0, 8, 10, 9, 15, 23, 11, 12, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 8, 10, 10, 10, 10, 10, 12, 0, 0, 0, 0, 0, 8, 10, 10, 12, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

public:
    static constexpr float TILE_DIMENSION       = 75.0f,
                        ACCELERATION_OF_GRAVITY = 981.0f,
                        END_GAME_THRESHOLD      = 800.0f;

    LevelC();
    LevelC(Vector2 origin, const char *bgHexCode);
    ~LevelC();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void drawUI() override;
    void shutdown() override;
private:
    bool isDirtTile(Vector2 worldPos);
    bool isWaterTile(EntityDirection* entity, Vector2 worldPos, Direction moveDirection);
    Vector2 getDirtCenter(Vector2 worldPos);
    bool plantExistsAt(Vector2 pos);

    // update helpers for organization
    void updatePlanting();
    void updatePlantGrowth(float deltaTime);
    void updateWatering();
    void updateHarvesting();
    void updateTreesAnimation(float deltaTime);
    void updateTreeInteraction();
    void updateTreeCollection();

    // UI functions
    void drawInventoryBackground();
    void drawInventorySlots();
    void drawInventoryItems();
    void addTree(int col, int row, int type);
    void addToInventory(int itemType, int amount, int &counter);
    void treeCollision(EntityDirection* entity, Vector2& lastSafePos);
    void playerTreeCollision();
    void removeFromInventory(int itemType, int amount, int &counter);

    // split update()
    void levelInput();
    void shopToggle();
    void shopTrading();
    void cowBehavior(float deltaTime);
    void cowBehaviorSingle(EntityDirection* cow, Vector2& lastSafePos,
        float& decisionTimer, bool& eating, float& eatingTimer,
        float deltaTime);

    //split render()
    void renderPlants();
    void renderTrees();
    void renderShop();

    int inventory[8];
    int inventoryCount[8];
    int getNextEmptySlot();

    // player icon top left
    int faceFrame;
    float faceTimer;

    Vector2 playerLastSafePos;

    enum ItemType {
        ITEM_PUMPKIN_CROP, ITEM_CAULIFLOWER_CROP,
        ITEM_PEACH, ITEM_WOOD, ITEM_EGGPLANT_CROP,
        ITEM_CUCUMBER_CROP, ITEM_PUMPKIN_SEED,
        ITEM_CAULIFLOWER_SEED, ITEM_EGGPLANT_SEED,
        ITEM_CUCUMBER_SEED
    };

    // an entire plant + tree class would be unnecessary bc 
    // no AI, movement, or collision
    struct Plant 
    {
        Vector2 position;
        bool watered = false;
        // animation timer
        float growthTimer;
        int growthPhase;
        int plantType;
        bool harvested = false;
    };

    std::vector<Plant> mPlants;
    int pumpkinSeeds;
    int cauliflowerSeeds;
    int eggplantSeeds;
    int cucumberSeeds;

    int pumpkinCrops;
    int cauliflowerCrops;
    int eggplantCrops;
    int cucumberCrops;

    struct Tree
    {
        Vector2 position;
        int frame ;
        float timer;
        bool collected;
        bool animation;
        bool fruitHarvested;
        int treeType;

    };

    std::vector<Tree> mTrees;
    int peaches;
    int wood;

    // shop 
    bool shopOpen = false;
    Vector2 shopWorldPosition;
    float clerkTimer;
    int clerkFrame;
    bool carrotSoldOut = false;
    bool lettuceSoldOut = false;

    // cow 1
    Vector2 cowLastSafePos;
    float cowDecisionTimer;
    bool  cowEating;
    float cowEatingTimer;

    // cow 2
    EntityDirection* cow2 = nullptr;
    Vector2 cow2LastSafePos;
    float cow2DecisionTimer;
    bool cow2Eating;
    float cow2EatingTimer = 0.6f;

    // cow 3
    EntityDirection* cow3 = nullptr;
    Vector2 cow3LastSafePos;
    float cow3DecisionTimer;
    bool cow3Eating;
    float cow3EatingTimer = 0.6f;

    bool chickenIsPet; 
    Vector2 chickenLastSafePos;

    Texture2D mKeysTexture;
    bool keysOpen = false ;

    // level C missions
    bool mission_pumpkin= false;
    bool mission_cauliflower = false;
    bool mission_cucumber = false;
    bool mission_eggplant = false;

    // textures
    Texture2D starTexture;
    Texture2D starBox;
    Texture2D missionDoneTex;

    Texture2D mMissionHeader;
    Texture2D mMissionBox;
    Texture2D mMissionButton;
    bool mMissionOpen = true;

    Texture2D playerFace;
    Texture2D box;

    Texture2D shopTexture;
    Texture2D clerkTexture;
    Texture2D shopDetails;

    Texture2D inventoryTexture;
    Texture2D mSlotTexture;
    int selectedSlot;
    Texture2D mSelectedSlotTexture;
    
    Texture2D mPlantsTexture;
    Texture2D peachTreeTexture;
    Texture2D normalTreeTexture;
    Texture2D peachIcon;
    Texture2D woodIcon;
};
#endif