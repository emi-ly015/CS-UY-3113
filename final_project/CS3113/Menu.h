#include "Scene.h"

class Menu : public Scene {
public:
    Menu();
    Menu(Vector2 origin, const char *bgHexCode);
    ~Menu();

    void initialise() override;
    void update(float deltatime) override;
    void render() override;
    void shutdown() override;
private:
    Texture2D mMenuTexture;
};