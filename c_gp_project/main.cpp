#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Camera.h"

// ȭ�� ũ��
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// ī�޶� ��ü
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));

// �ð� ���� ����
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ���콺 �Է� ����
int lastMouseX = SCREEN_WIDTH / 2;
int lastMouseY = SCREEN_HEIGHT / 2;
bool firstMouse = true;
bool isWarpingMouse = false; // ���콺�� �߾����� �̵� ������ Ȯ��

// ��� ������
GLuint VAO, VBO;

// ��� �ʱ�ȭ �Լ�
void initBackground() {
    float vertices[] = {
        // x, y, z
        -10.0f, 0.0f, -10.0f,
        -10.0f, 0.0f,  10.0f,
         10.0f, 0.0f,  10.0f,
         10.0f, 0.0f, -10.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Ű���� �Է� ó�� �Լ�
void handleKeyboard(unsigned char key, int x, int y) {
    float currentFrame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (key == 'w') camera.processKeyboardInput('W', deltaTime);
    if (key == 's') camera.processKeyboardInput('S', deltaTime);
    if (key == 'a') camera.processKeyboardInput('A', deltaTime);
    if (key == 'd') camera.processKeyboardInput('D', deltaTime);

    // FOV �� ���� ����
    if (key == '-') camera.decreaseSensitivity(); // ���콺 ���� ����
    if (key == '=') camera.increaseSensitivity(); // ���콺 ���� ����
}

// ���콺 �̵� ó�� �Լ�
void handleMouseMotion(int x, int y) {
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    if (isWarpingMouse) {
        isWarpingMouse = false;
        return;
    }

    // �߾Ӱ��� ���̸� ���
    float xOffset = x - centerX;
    float yOffset = centerY - y; // ���� ����

    // ī�޶� ȸ�� ó��
    camera.processMouseMovement(xOffset, yOffset);

    // ���콺�� �ٽ� �߾����� �̵�
    isWarpingMouse = true;
    glutWarpPointer(centerX, centerY);
}

// ���콺 �� �Է� ó�� �Լ�
void handleMouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) camera.processScrollInput(-1.0f); // �� ��
    if (dir < 0) camera.processScrollInput(1.0f);  // �� �ٿ�
}

// ������ �Լ�
void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ī�޶� ��� ���
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix(static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT);

    // �ܻ����� �ٴ� �׸���
    glBindVertexArray(VAO);
    glUseProgram(0); // ���� ���������� ���
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));

    glColor3f(0.6f, 0.8f, 0.9f); // �ϴû� �ٴ�
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glEnd();

    glutSwapBuffers();
}

void update() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // FreeGLUT �ʱ�ȭ
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Camera Test");

    // GLEW �ʱ�ȭ
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // ��� �ʱ�ȭ
    initBackground();

    // FreeGLUT �ݹ� ����
    glutDisplayFunc(render);
    glutIdleFunc(update);
    glutKeyboardFunc(handleKeyboard);
    glutPassiveMotionFunc(handleMouseMotion);
    glutMouseWheelFunc(handleMouseWheel);

    // ���콺 ������ ��Ȱ��ȭ
    glutWarpPointer(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();

    return 0;
}
