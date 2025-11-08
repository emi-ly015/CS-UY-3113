#include "LevelA.h"

#ifndef LEVELB_H
#define LEVELB_H

class LevelB : public Scene {
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 6, 5, 0, 0, 0, 0, 0, 9, 12, 0, 4,
        0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 8, 7, 0, 0,
        0, 9, 12, 0, 0, 0, 13, 0, 11, 0, 8, 7, 0, 0,
        0, 8, 7, 0, 0, 0, 11, 0, 11, 0, 8, 7, 0, 0,
        0, 8, 7, 0, 0, 0, 11, 0, 11, 0, 8, 7, 0, 0
    };

public:
    static constexpr float TILE_DIMENSION       = 75.0f,
                        ACCELERATION_OF_GRAVITY = 981.0f,
                        END_GAME_THRESHOLD      = 800.0f;

    LevelB();
    LevelB(Vector2 origin, const char *bgHexCode);
    ~LevelB();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif