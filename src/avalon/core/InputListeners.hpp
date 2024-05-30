#ifndef AVALON_INPUTLISTENER_H
#define AVALON_INPUTLISTENER_H

#include <vector>

class KeyListener {
private:
    KeyListener() = default;
    ~KeyListener() = default;

    std::vector<bool> keyPressed = std::vector<bool>(GLFW_KEY_LAST, false);

public:
    static KeyListener &getInstance() {
        static KeyListener instance;
        return instance;
    }

    // delete copy and move constructors and assign operators
    KeyListener(KeyListener const &) = delete;             // Copy construct
    KeyListener(KeyListener &&) = delete;                  // Move construct
    KeyListener &operator=(KeyListener const &) = delete;  // Copy assign
    KeyListener &operator=(KeyListener &&) = delete;      // Move assign

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if(action == GLFW_PRESS) {
            getInstance().keyPressed[key] = true;
        } else if(action == GLFW_RELEASE) {
            getInstance().keyPressed[key] = false;
        }
    }

};

class MouseListener {
private:
    MouseListener() = default;

    ~MouseListener() = default;

    double scrollX = 0, scrollY = 0;
    double xPos = 0, yPos = 0, lastX = 0, lastY = 0;
    std::vector<bool> mouseButtonPressed = std::vector<bool>(GLFW_MOUSE_BUTTON_LAST, false);
    bool dragging = false;
public:
    static MouseListener &getInstance() {
        static MouseListener instance;
        return instance;
    }

    // delete copy and move constructors and assign operators
    MouseListener(MouseListener const &) = delete;             // Copy construct
    MouseListener(MouseListener &&) = delete;                  // Move construct
    MouseListener &operator=(MouseListener const &) = delete;  // Copy assign
    MouseListener &operator=(MouseListener &&) = delete;      // Move assign

    //https://www.glfw.org/docs/3.3/input_guide.html#input_key

    //If you wish to be notified when the cursor moves over the window, set a cursor position callback.
    static void mousePosCallback(GLFWwindow* window, double xPos, double yPos) {
        getInstance().lastX = getInstance().xPos;
        getInstance().lastY = getInstance().yPos;
        getInstance().xPos = xPos;
        getInstance().yPos = yPos;
        getInstance().dragging = getInstance().mouseButtonPressed[0] || getInstance().mouseButtonPressed[1] ||
                                   getInstance().mouseButtonPressed[2];
    }

    //If you wish to be notified when a mouse button is pressed or released, set a mouse button callback.
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS) {
            if (button < getInstance().mouseButtonPressed.size()) {
                getInstance().mouseButtonPressed[button] = true;
            }
        } else if (action == GLFW_RELEASE) {
            if (button < getInstance().mouseButtonPressed.size()) {
                getInstance().mouseButtonPressed[button] = false;
            }
            getInstance().dragging = false;
        }
    }

    //If you wish to be notified when the user scrolls, whether with a mouse wheel or touchpad gesture, set a scroll callback.
    static void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
        getInstance().scrollX = xOffset;
        getInstance().scrollY = yOffset;
    }

    static void endFrame() {
        getInstance().scrollX = 0;
        getInstance().scrollY = 0;
        getInstance().lastX = getInstance().xPos;
        getInstance().lastY = getInstance().yPos;
    }

    static float getX() {
        return (float) getInstance().xPos;
    }

    static float getY() {
        return (float) getInstance().yPos;
    }

    static float getDX() {
        return (float) (getInstance().lastX - getInstance().xPos);
    }

    static float getDY() {
        return (float) (getInstance().lastY - getInstance().yPos);
    }

    static float getScrollX() {
        return (float) getInstance().scrollX;
    }

    static float getScrollY() {
        return (float) getInstance().scrollY;
    }

    static bool isDragging() {
        return getInstance().dragging;
    }

    static bool mouseButtonDown(int button) {
        if (button < getInstance().mouseButtonPressed.size()) {
            return getInstance().mouseButtonPressed[button];
        } else {
            return false;
        }
    }
};

#endif //AVALON_KEYLISTENER_H
