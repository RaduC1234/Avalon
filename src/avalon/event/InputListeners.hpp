#ifndef INPUT_LISTENERS_HPP
#define INPUT_LISTENERS_HPP

#include <vector>

class InputListeners {
private:

    InputListeners()
            : keyPressed(348, false), // GLFW_KEY_LAST 348
              scrollX(0), scrollY(0), xPos(0), yPos(0), lastX(0), lastY(0),
              mouseButtonPressed(7, false), // GLFW_BUTTON_LAST 7
              dragging(false) {}

public:

    std::vector<bool> keyPressed;
    double scrollX, scrollY;
    double xPos, yPos, lastX, lastY;
    std::vector<bool> mouseButtonPressed;
    bool dragging;

    // Static method to get the singleton instance
    static InputListeners& getInstance() {
        static InputListeners instance;
        return instance;
    }

    float getX() const {
        return static_cast<float>(xPos);
    }

    float getY() const {
        return static_cast<float>(yPos);
    }

    float getDX() const {
        return static_cast<float>(lastX - xPos);
    }

    float getDY() const {
        return static_cast<float>(lastY - yPos);
    }

    float getScrollX() const {
        return static_cast<float>(scrollX);
    }

    float getScrollY() const {
        return static_cast<float>(scrollY);
    }

    bool isDragging() const {
        return dragging;
    }

    bool mouseButtonDown(int button) const {
        if (button < mouseButtonPressed.size()) {
            return mouseButtonPressed[button];
        } else {
            return false;
        }
    }

    bool keyPressedDown(int key) const {
        if (key < keyPressed.size()) {
            return keyPressed[key];
        } else {
            return false;
        }
    }

#define KEY_PRESSED(key) (InputListeners::getInstance().keyPressedDown(key))
#define MOUSE_PRESSED(key) (InputListeners::getInstance().mouseButtonDown(key))

};

#endif // INPUT_LISTENERS_HPP
