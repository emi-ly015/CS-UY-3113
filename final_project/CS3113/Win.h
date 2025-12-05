#include "Scene.h"

class Win : public Scene {
public:
    Win();
    Win(Vector2 origin, const char *bgHexCode);
    ~Win();

    void initialise() override;
    void update(float deltatime) override;
    void render() override;
    void shutdown() override;
private:
    Texture2D mWinTexture;
};