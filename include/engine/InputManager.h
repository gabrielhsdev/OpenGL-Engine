#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <unordered_map>
#include <iostream>
#include <cstdint>

enum class KeyState : std::uint8_t { RELEASED, PRESSED, HELD };

class InputManager {
  public:
    std::unordered_map<int, KeyState> m_Keys;
    float MouseX, MouseY;
    float DeltaX, DeltaY;
    bool FirstMouse;

    InputManager() {
        MouseX = 0.0f;
        MouseY = 0.0f;
        DeltaX = 0.0f;
        DeltaY = 0.0f;
        FirstMouse = true; // To avoid jumping on first mouse move
    }

    void KeyPress(int key) {
        if (m_Keys.find(key) == m_Keys.end()) {
            m_Keys[key] = KeyState::PRESSED;
        } else {
            m_Keys[key] = KeyState::HELD;
        }
    }

    void KeyRelease(int key) {
        if (m_Keys.find(key) != m_Keys.end()) {
            m_Keys[key] = KeyState::RELEASED;
        }
    }

    bool IsKeyPressed(int key) const {
        auto it = m_Keys.find(key);
        return it != m_Keys.end() &&
               (it->second == KeyState::PRESSED || it->second == KeyState::HELD);
    }

    void MouseMove(float xpos, float ypos) {
        if (FirstMouse) {
            MouseX = xpos;
            MouseY = ypos;
            FirstMouse = false;
        } else {
            DeltaX = xpos - MouseX;
            DeltaY = ypos - MouseY;
            MouseX = xpos;
            MouseY = ypos;
        }
    }

    void Update() {
        DeltaX = 0.0f;
        DeltaY = 0.0f;
        for (auto& pair : m_Keys) {
            if (pair.second == KeyState::PRESSED) {
                pair.second = KeyState::HELD;
            } else if (pair.second == KeyState::RELEASED) {
                m_Keys.erase(pair.first);
            }
        }
    }
};

#endif // INPUT_MANAGER_H
