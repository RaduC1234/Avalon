#include "PlatformUtils.hpp"

#include <GLFW/glfw3.h>

double Time::getTime() {
    return glfwGetTime();
}
