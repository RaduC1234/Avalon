#include "Application.hpp"

extern Application* createApplication();

int main() {
    auto app = createApplication();
    app->run();
    delete app;
}
