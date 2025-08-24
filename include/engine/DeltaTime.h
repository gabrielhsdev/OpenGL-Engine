#pragma once
#include <GLFW/glfw3.h>

struct Time {
    float deltaTime;
    float elapsedTime;
};

class DeltaTime { // rename to avoid collisions
  public:
    DeltaTime() = default;

    Time getTime() const {
        return time;
    }

    void calculateDeltaTime() {
        currentFrame = static_cast<float>(glfwGetTime());
        time.deltaTime = currentFrame - lastFrame;
        time.elapsedTime += time.deltaTime;
        lastFrame = currentFrame;
    }

  private:
    Time time{0.0f, 0.0f};
    float currentFrame = 0.0f;
    float lastFrame = 0.0f;
};
