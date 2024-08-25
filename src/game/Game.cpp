#include "Avalon.hpp"
#include "LevelScene.hpp"

class Game : public Application {

public:
    Game() : Application() {
        changeScene(CreateScope<LevelScene>());
    }
};

Application* createApplication() {
    return new Game();
}
