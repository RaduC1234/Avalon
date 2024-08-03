#ifndef AVALON_WINDOW_HPP
#define AVALON_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <utility>

#include "Core.hpp"

#include "../logic/Scene.hpp"

static int glfw_windowCount = 0;

class Window {


    std::string title = "Avalon C++";
    int width;
    int height;
    bool vSync;

    GLFWwindow *glfwWindow = nullptr;
    InputListeners& listener = InputListeners::getInstance();

    Scope<Scene> currentScene;

public:

    Window(std::string  title = "Avalon Window", int width = 1920, int height = 1080, bool vSync = GLFW_TRUE) : title(std::move(title)), width(width), height(height), vSync(vSync) {

        Time::init();

        glfwInit();
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // the window will stay hidden after creation
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // the window will be resizable
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE); // make window maximized

        this->glfwWindow = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);

        if (glfwWindow == NULL) {
            std::cout << "Error creating glfw window." << std::endl;
            glfwTerminate();
            exit(1);
        }

        glfwMakeContextCurrent(this->glfwWindow);

        glfwSetWindowUserPointer(this->glfwWindow, this);

        //https://www.glfw.org/docs/3.3/input_guide.html#input_key

/*            // Window Callbacks
            glfwSetWindowSizeCallback(glfw_window, [](GLFWwindow *window, int width, int height) {
                Window &data = *static_cast<Window *>(glfwGetWindowUserPointer(window));

                //WindowResizeEvent event(width, height);
                //data.getEventManager().publish(event);
            });
*/
        glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow *window) {
            //Window &data = *static_cast<Window *>(glfwGetWindowUserPointer(window));

            glfwTerminate();
            exit(0);
        });

        // Keyboard Callbacks
        glfwSetKeyCallback(glfwWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            Window &data = *static_cast<Window *>(glfwGetWindowUserPointer(window));

            if (action == GLFW_PRESS) {
                data.listener.keyPressed[key] = true;
            } else if (action == GLFW_RELEASE) {
                data.listener.keyPressed[key] = false;
            }
        });
/*
            glfwSetCharCallback(glfwWindow, [](GLFWwindow *window, unsigned int keycode) {
                Window &data = *static_cast<Window *>(glfwGetWindowUserPointer(window));

                //KeyTypedEvent event(keycode);
                //data.getEventManager().publish(event);
            });*/

        // Mouse Callbacks
        glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow *window, int button, int action, int mods) {
            Window &data = *static_cast<Window *>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    if (button < data.listener.mouseButtonPressed.size()) {
                        data.listener.mouseButtonPressed[button] = true;
                    }
                    break;
                }
                case GLFW_RELEASE: {
                    if (button < data.listener.mouseButtonPressed.size()) {
                        data.listener.mouseButtonPressed[button] = false;
                    }
                    break;
                }
            }
        });

        glfwSetScrollCallback(glfwWindow, [](GLFWwindow *window, double xOffset, double yOffset) {
            Window &data = *static_cast<Window *>(glfwGetWindowUserPointer(window));

            data.listener.scrollX = xOffset;
            data.listener.scrollY = yOffset;
        });

        glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow *window, double xPos, double yPos) {
            Window &data = *static_cast<Window *>(glfwGetWindowUserPointer(window));

            data.listener.lastX = data.listener.xPos;
            data.listener.lastY = data.listener.yPos;
            data.listener.xPos = xPos;
            data.listener.yPos = yPos;
            data.listener.dragging = data.listener.mouseButtonPressed[0] || data.listener.mouseButtonPressed[1] ||
                                     data.listener.mouseButtonPressed[2];
        });


        glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow *window, int l_width, int l_height) {
            glViewport(0, 0, l_width, l_height);
        });

        // Make OpenGl the current context
        glfwMakeContextCurrent(glfwWindow);
        glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow *window, int l_width, int l_height) {
            glViewport(0, 0, l_width, l_height);
        });

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(1);
        }

        // enable v-sync
        glfwSwapInterval(1);

        glfwShowWindow(glfwWindow);

    }

    ~Window() {
        if (glfw_windowCount == 1)
            glfwTerminate();
        glfw_windowCount--;
    }

    void onUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(glfwWindow);
    }

    friend class Application;
};

#endif //AVALON_WINDOW_HPP
