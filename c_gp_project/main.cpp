#include "pch.h"

GLuint VAO, VBO;
Shader* shaderProgram;

Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));
Background mBackground;
Crosshair mCrosshair;
Gun mGun;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
bool leftMousePressed = false;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

glm::vec3 lightPos(0.0f, 20.0f, 0.0f);

bool keys[256] = { false };

void keyDown(unsigned char key, int x, int y) {
    keys[key] = true;
}
void keyUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void processInput() {
    if (keys['w'] || keys['W']) camera.processKeyboardInput('w', deltaTime);
    if (keys['s'] || keys['S']) camera.processKeyboardInput('s', deltaTime);
    if (keys['a'] || keys['A']) camera.processKeyboardInput('a', deltaTime);
    if (keys['d'] || keys['D']) camera.processKeyboardInput('d', deltaTime);
    if (keys['q'] || keys['Q']) exit(0);
}

void specialKeyCallback(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        camera.increaseSensitivity();
        break;
    case GLUT_KEY_DOWN:
        camera.decreaseSensitivity();
        break;

    case GLUT_KEY_LEFT:
        mCrosshair.previousTexture();
        break;
    case GLUT_KEY_RIGHT:
        mCrosshair.nextTexture();
        break;
    default:
        break;
    }
}


void handleMouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) camera.processScrollInput(dir);
    if (dir < 0) camera.processScrollInput(dir);
}

void mouseButtonCallback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            leftMousePressed = true;
            PlaySound(L"Resource\\gunshot.wav", NULL, SND_FILENAME | SND_ASYNC);
        }
        else if (state == GLUT_UP) {
            leftMousePressed = false;
        }
    }
}

void mouseCallback(int x, int y) {
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    if (firstMouse) {
        lastX = centerX;
        lastY = centerY;
        firstMouse = false;
        glutWarpPointer(centerX, centerY);
        return;
    }

    float xOffset = x - centerX;
    float yOffset = centerY - y;

    camera.processMouseMovement(xOffset, yOffset);
    glutWarpPointer(centerX, centerY);
}

void mouseMotionCallback(int x, int y) {
    if (leftMousePressed) {
        int centerX = SCREEN_WIDTH / 2;
        int centerY = SCREEN_HEIGHT / 2;

        float xOffset = x - centerX;
        float yOffset = centerY - y;

        camera.processMouseMovement(xOffset, yOffset);
        glutWarpPointer(centerX, centerY);
    }
}

void keepMouseCentered() {
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    glutWarpPointer(centerX, centerY);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix((float)SCREEN_WIDTH / SCREEN_HEIGHT);

    shaderProgram->use();

    shaderProgram->setMat4("view", glm::value_ptr(view));
    shaderProgram->setMat4("projection", glm::value_ptr(projection));
    shaderProgram->setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
    shaderProgram->setVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    shaderProgram->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    shaderProgram->setInt("texture1", 0);

    glm::mat4 model = glm::mat4(1.0f);
    shaderProgram->setMat4("model", glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    {
        glm::mat4 backgroundModel = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
        backgroundModel = glm::translate(backgroundModel, glm::vec3(0.0f, 0.0f, 2.0f));
        shaderProgram->setMat4("model", glm::value_ptr(backgroundModel));
        mBackground.draw(view, projection);
    }

    // Gun ·»´õ¸µ
    {
        glm::vec3 cameraPos = camera.getPosition();
        glm::vec3 cameraFront = glm::normalize(camera.getFront());
        glm::vec3 cameraUp = glm::normalize(camera.getUp());
        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

        float forwardOffset = 0.4f;
        float rightOffset = 0.5f;
        float downOffset = -0.5f;

        glm::vec3 gunPos = cameraPos
            + cameraFront * forwardOffset
            + cameraRight * rightOffset
            + cameraUp * downOffset;

        glm::mat4 gunModel = glm::translate(glm::mat4(1.0f), gunPos);
        glm::mat4 lookAtMatrix = glm::lookAt(gunPos, gunPos + cameraFront, cameraUp);
        gunModel = glm::inverse(lookAtMatrix);

        gunModel = glm::rotate(gunModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        gunModel = glm::scale(gunModel, glm::vec3(0.2f));

        shaderProgram->setMat4("model", glm::value_ptr(gunModel));
        mGun.draw(view, projection);
    }
    mCrosshair.draw();

    glutSwapBuffers();
}


void update() {
    float currentFrame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput();
    lightPos = camera.getPosition();
    lightPos.y += 30.f;
    lightPos.z += 5.f;

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Aim Trainer");
    glutFullScreen();

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    shaderProgram = new Shader("vertex_shader.glsl", "fragment_shader.glsl");

    mCrosshair.init();
    mGun.init("Resource\\Gun.obj", "Resource\\Gun.jpg");
    mBackground.init("Resource\\background.obj", "Resource\\background.png");

    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialKeyCallback);

    glutDisplayFunc(render);
    glutIdleFunc(update);

    glutMouseFunc(mouseButtonCallback);
    glutMotionFunc(mouseMotionCallback);
    glutPassiveMotionFunc(mouseCallback);
    glutMouseWheelFunc(handleMouseWheel);
    keepMouseCentered();
    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();

    delete shaderProgram;
    return 0;
}