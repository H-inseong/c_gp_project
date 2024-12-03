#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Shader.h"
#include "Camera.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

GLuint VAO, VBO;
Shader* shaderProgram;

// �ٸ� ���̴� �ʿ��
//Shader* sec_shaderProgram;

Camera camera(glm::vec3(0.0f, 2.0f, 0.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;


glm::vec3 lightPos(1.0f, 1.0f, 2.0f);

void initBackground() {
    float vertices[] = {
        // Positions          // Normals          // Colors
        -5.0f,  0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -5.0f,  0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         5.0f,  0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,

         5.0f,  0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         5.0f,  0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -5.0f,  0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void processInput(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'W':
    case 'w':
    case 'S':
    case 's':
    case 'A':
    case 'a':
    case 'D':
    case 'd':
        camera.processKeyboardInput(key, deltaTime);
        break;
    case '-':
    case '_':
        camera.decreaseSensitivity();
        break;
    case '+':
    case '=':
        camera.increaseSensitivity();
        break;
    case 'q':
    case 'Q':
        exit(0);
        break;
    default:
        break;
    }
}

void handleMouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) camera.processScrollInput(dir);
    if (dir < 0) camera.processScrollInput(dir);
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

    // main�Լ��� ������ Shader �� ���ڸ� �����̸����� �о��
    // ���̴��� ��� ����
    shaderProgram->use();
    
    //uniform view, projection��� ���޺κ�

    // vertex_shader�� uniform ���� view��   view, ��  camera.getViewMatrix() �� ����� ���� ���� 
    shaderProgram->setMat4("view", glm::value_ptr(view));

    // vertex_shader�� uniform ���� projection�� camera.getProjectionMatrix �� ����� ���� ���� 
    shaderProgram->setMat4("projection", glm::value_ptr(projection));



    // ���� �� ���� ����


    // fragment_shader�� uniform ���� lightPos��
    // lightPos.x, lightPos.y, lightPos.z ���� ����
    shaderProgram->setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);


    // fragment_shader�� uniform ����  viewPos��
    // camera.getPosition().x, camera.getPosition().y, camera.getPosition().z ���� ����
    shaderProgram->setVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);


    // fragment_shader�� uniform ����  lightColor��
    // 2.0f, 2.0f, 2.0f ���� ����
    shaderProgram->setVec3("lightColor", 2.0f, 2.0f, 2.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(3.0));

    // vertex_shader�� uniform ���� model�� model�� ����� ���� ����
    shaderProgram->setMat4("model", glm::value_ptr(model));
    

    // �ٴ� ������
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glutSwapBuffers();
}

void update() {
    float currentFrame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

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

    shaderProgram = new Shader("vertex_shader.glsl", "fragment_shader.glsl");

    //�ٸ� ���̴� �ʿ�� 
    //sec_shaderProgram = new Shader("sec_vertex_shader.glsl", "sec_fragment_shader.glsl");


    initBackground();

    glutDisplayFunc(render);
    glutIdleFunc(update);
    glutKeyboardFunc(processInput);
    glutPassiveMotionFunc(mouseCallback);
    glutMouseWheelFunc(handleMouseWheel);
    keepMouseCentered();
    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();

    delete shaderProgram;
    return 0;
}