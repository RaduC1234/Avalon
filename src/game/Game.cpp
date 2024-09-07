#include "Avalon.hpp"
#include "LevelScene.hpp"

class Runtime : public Application {

public:
    Runtime() : Application() {
        changeScene(CreateScope<LevelScene>());
    }
};

Application* createApplication() {
    return new Runtime();
}
