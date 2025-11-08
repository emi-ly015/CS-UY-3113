#include "LevelA.h"

#ifndef LEVELC_H
#define LEVELC_H

class LevelC : public Scene {
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 9, 10, 10, 12, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 8, 3, 3, 7, 0, 0, 0, 13, 4,
        0, 9, 12, 0, 0, 8, 3, 3, 7, 0, 13, 0, 11, 0,
        0, 8, 7, 0, 0, 8, 3, 3, 7, 0, 11, 0, 11, 0,
        0, 8, 7, 0, 0, 8, 3, 3, 7, 0, 11, 0, 11, 0
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
    void shutdown() override;
};
#endif