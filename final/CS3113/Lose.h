#include "Scene.h"

class Lose : public Scene {
public:
    Lose();
    Lose(Vector2 origin, const char *bgHexCode);
    ~Lose();

    void initialise() override;
    void update(float deltatime) override;
    void render() override;
    void shutdown() override;
private:
    Texture2D mLoseTexture;
};