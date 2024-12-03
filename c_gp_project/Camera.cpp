#include "Camera.h"

Camera::Camera(glm::vec3 position)
    : mPosition(position),
    mFront(0.0f, 0.0f, -1.0f),
    mUp(0.0f, 1.0f, 0.0f),
    mYaw(-90.0f),
    mPitch(0.0f),
    mFOV(90.0f),
    mMouseSensitivity(0.1f),
    mSpeed(2.5f)
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}
glm::vec3 Camera::getPosition() const {
    return glm::vec3(mPosition.x, mPosition.y, mPosition.z);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(mFOV), aspectRatio, 0.1f, 100.0f);
}

void Camera::processKeyboardInput(int key, float deltaTime) {
    float velocity = mSpeed * deltaTime;

    if (key == 'W') mPosition += mFront * velocity;
    if (key == 'w') mPosition += mFront * velocity;
    if (key == 'S') mPosition -= mFront * velocity;
    if (key == 's') mPosition -= mFront * velocity;
    if (key == 'A') mPosition -= mRight * velocity;
    if (key == 'a') mPosition -= mRight * velocity;
    if (key == 'D') mPosition += mRight * velocity;
    if (key == 'd') mPosition += mRight * velocity;
}

void Camera::processMouseMovement(float xOffset, float yOffset) {
    xOffset *= mMouseSensitivity;
    yOffset *= mMouseSensitivity;

    mYaw += xOffset;
    mPitch += yOffset;

    if (mPitch > 89.0f) mPitch = 89.0f;
    if (mPitch < -89.0f) mPitch = -89.0f;

    updateCameraVectors();
}


void Camera::processScrollInput(float yOffset) {
    yOffset > 0 ? increaseFOV() : decreaseFOV();
}

void Camera::increaseFOV() {
    mFOV += 5.0f;
    if (mFOV > 110.0f) mFOV = 110.0f;
}

void Camera::decreaseFOV() {
    mFOV -= 5.0f;
    if (mFOV < 90.0f) mFOV = 90.0f;
}

void Camera::increaseSensitivity() {
    mMouseSensitivity += 0.01f;
    if (mMouseSensitivity > 1.0f) mMouseSensitivity = 1.0f;
}

void Camera::decreaseSensitivity() {
    mMouseSensitivity -= 0.01f;
    if (mMouseSensitivity < 0.01f) mMouseSensitivity = 0.01f; 
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(front);

    // Right 벡터 계산 (Up과 Front의 외적)
    mRight = glm::normalize(glm::cross(mFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    // Up 벡터 재계산 (Right와 Front의 외적)
    mUp = glm::normalize(glm::cross(mRight, mFront));
}
