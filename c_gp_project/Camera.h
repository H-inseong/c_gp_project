#pragma once
class Camera {
public:
    Camera(glm::vec3 position);

    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;
    glm::vec3 getUp() const;

    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    void processKeyboardInput(int key, float deltaTime);

    void processMouseMovement(float xOffset, float yOffset);

    void processScrollInput(float yOffset);

    void increaseFOV();
    void decreaseFOV();

    void increaseSensitivity();
    void decreaseSensitivity();

private:
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;

    float mYaw;
    float mPitch;
    float mFOV;
    float mMouseSensitivity;
    float mSpeed;

    void updateCameraVectors();
};
