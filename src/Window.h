#ifndef AVALON_WINDOW_H
#define AVALON_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include "InputListeners.h"

class Window {
private:
    Window() = default;

    ~Window() = default;

    GLFWwindow *glfwWindow = nullptr;
    int width = 1080;
    int height = 1020;
    std::string title = "Avalon C++";

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
        glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow *window, int width, int height) {
            glViewport(0, 0, width, height);
        });

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(1);
        }

        glfwShowWindow(glfwWindow);
    }

    void loop() {

        while (!glfwWindowShouldClose(glfwWindow)) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(glfwWindow);
            glfwPollEvents();
        }
    }
};


#endif //AVALON_WINDOW_H
