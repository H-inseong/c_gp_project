#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Camera.h"

// 화면 크기
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// 카메라 객체
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));

// 시간 관련 변수
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 마우스 입력 상태
int lastMouseX = SCREEN_WIDTH / 2;
int lastMouseY = SCREEN_HEIGHT / 2;
bool firstMouse = true;
bool isWarpingMouse = false; // 마우스가 중앙으로 이동 중인지 확인

// 배경 데이터
GLuint VAO, VBO;

// 배경 초기화 함수
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

// 키보드 입력 처리 함수
void handleKeyboard(unsigned char key, int x, int y) {
    float currentFrame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (key == 'w') camera.processKeyboardInput('W', deltaTime);
    if (key == 's') camera.processKeyboardInput('S', deltaTime);
    if (key == 'a') camera.processKeyboardInput('A', deltaTime);
    if (key == 'd') camera.processKeyboardInput('D', deltaTime);

    // FOV 및 감도 조절
    if (key == '-') camera.decreaseSensitivity(); // 마우스 감도 감소
    if (key == '=') camera.increaseSensitivity(); // 마우스 감도 증가
}

// 마우스 이동 처리 함수
void handleMouseMotion(int x, int y) {
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    if (isWarpingMouse) {
        isWarpingMouse = false;
        return;
    }

    // 중앙과의 차이를 계산
    float xOffset = x - centerX;
    float yOffset = centerY - y; // 상하 반전

    // 카메라 회전 처리
    camera.processMouseMovement(xOffset, yOffset);

    // 마우스를 다시 중앙으로 이동
    isWarpingMouse = true;
    glutWarpPointer(centerX, centerY);
}

// 마우스 휠 입력 처리 함수
void handleMouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) camera.processScrollInput(-1.0f); // 휠 업
    if (dir < 0) camera.processScrollInput(1.0f);  // 휠 다운
}

// 렌더링 함수
void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 카메라 행렬 계산
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix(static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT);

    // 단색으로 바닥 그리기
    glBindVertexArray(VAO);
    glUseProgram(0); // 고정 파이프라인 사용
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));

    glColor3f(0.6f, 0.8f, 0.9f); // 하늘색 바닥
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
    // FreeGLUT 초기화
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Camera Test");

    // GLEW 초기화
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // 배경 초기화
    initBackground();

    // FreeGLUT 콜백 설정
    glutDisplayFunc(render);
    glutIdleFunc(update);
    glutKeyboardFunc(handleKeyboard);
    glutPassiveMotionFunc(handleMouseMotion);
    glutMouseWheelFunc(handleMouseWheel);

    // 마우스 포인터 비활성화
    glutWarpPointer(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();

    return 0;
}
