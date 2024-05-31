#ifndef AVALON_WINDOW_HPP
#define AVALON_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include "InputListeners.hpp"
#include "Utils.hpp"

#include "../render/Shader.hpp"
#include "../logic/Scene.hpp"

#define DEBUG_MODE true

class Window {
private:
    Window() = default;

    ~Window() = default;

    GLFWwindow *glfwWindow = nullptr;
    int width = 1080;
    int height = 1020;
    std::string title = "Avalon C++";
    std::unique_ptr<Scene> currentScene;

public:

    static Window &getInstance() {
        static Window instance;
        return instance;
    }

    // delete copy and move constructors and assign operators
    Window(Window const &) = delete;             // Copy construct
    Window(Window &&) = delete;                  // Move construct
    Window &operator=(Window const &) = delete;  // Copy assign
    Window &operator=(Window &&) = delete;      // Move assign

    void run() {
        init();
        loop();

        glfwTerminate();
    }

private:
    void init() {

        //std::cout << "Running " << glGetString(GL_VERSION) << " of OpenGl\n";

        TimeUtils::init();

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

        // Set Input Callbacks - mouse
        glfwSetCursorPosCallback(glfwWindow, MouseListener::mousePosCallback);
        glfwSetMouseButtonCallback(glfwWindow, MouseListener::mouseButtonCallback);
        glfwSetScrollCallback(glfwWindow, MouseListener::mouseScrollCallback);

        // Set Input Callbacks - Keyboard
        glfwSetKeyCallback(glfwWindow, KeyListener::keyCallback);

        // Make OpenGl the current context
        glfwMakeContextCurrent(glfwWindow);
        glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow *window, int l_width, int l_height) {
            glViewport(0, 0, l_width, l_height);
        });

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(1);
        }

        changeScene(0);

        // enable v-sync
        glfwSwapInterval(1);

        glfwShowWindow(glfwWindow);

        this->currentScene->init();

    }

    void loop() {

        float beginTime = TimeUtils::getTime();
        float endTime;
        float dt = -1.0f;


        while (!glfwWindowShouldClose(glfwWindow)) {

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if (dt >= 0) {
                this->currentScene->update(dt);
            }

            glfwSwapBuffers(glfwWindow);
            glfwPollEvents();

            endTime = TimeUtils::getTime();
            dt = endTime - beginTime;
            beginTime = endTime;
        }
    }

    void changeScene(int scene) {
        switch (scene) {
            case 0: {
                this->currentScene = std::make_unique<LevelEditorScene>();
                break;
            }
            case 1: {
                this->currentScene = std::make_unique<LevelScene>();
                break;
            }
            default:
                return;
        }
    }
};

#endif //AVALON_WINDOW_HPP
