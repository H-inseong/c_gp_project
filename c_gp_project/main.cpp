#include "pch.h"

GLuint VAO, VBO;
Shader* shaderProgram;

// 다른 셰이더 필요시
//Shader* sec_shaderProgram;

Camera camera(glm::vec3(0.0f, 2.0f, 0.0f));

Crosshair mCrosshair;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;


glm::vec3 lightPos(1.0f, 1.0f, 1.0f);

void TimerFunction(int value);

void initBackground() {
    float vertices[] = {
        // Positions          // Normals          // Colors
        -5.0f,  0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -5.0f,  0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         5.0f,  0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,

         5.0f,  0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f, 
         5.0f,  0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -5.0f,  0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
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
    case 'r':
        TargetSpawn(rand() % 3, 16, 0, 0, 0, 1);
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

    // main함수에 선언한 Shader 의 인자를 파일이름으로 읽어온
    // 쉐이더를 사용 시작
    shaderProgram->use();
    
    //uniform view, projection행렬 전달부분

    // vertex_shader의 uniform 변수 view에   view, 즉  camera.getViewMatrix() 의 행렬의 값을 전달 
    shaderProgram->setMat4("view", glm::value_ptr(view));

    // vertex_shader의 uniform 변수 projection에 camera.getProjectionMatrix 의 행렬의 값을 전달 
    shaderProgram->setMat4("projection", glm::value_ptr(projection));



    // 조명 및 색상 설정


    // fragment_shader의 uniform 변수 lightPos에
    // lightPos.x, lightPos.y, lightPos.z 값을 전달
    shaderProgram->setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);


    // fragment_shader의 uniform 변수  viewPos에
    // camera.getPosition().x, camera.getPosition().y, camera.getPosition().z 값을 전달
    shaderProgram->setVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);


    // fragment_shader의 uniform 변수  lightColor에
    // 2.0f, 2.0f, 2.0f 값을 전달
    shaderProgram->setVec3("lightColor", 2.0f, 2.0f, 2.0f);

    glm::mat4 model = glm::mat4(1.0f);
    shaderProgram->setMat4("model", glm::value_ptr(model));

    // 바닥 렌더링

    //shaderProgram->setVec4("FullBrightColor")
    shaderProgram->setVec4("FullBrightColor", 0.0f, 0.0f, 0.0f, -1.0f);

    glFrontFace(GL_CCW);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    glEnable(GL_CULL_FACE);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    //gluQuadricDrawStyle(qobj, GLU_LINE);

    //GLU_FILL | 솔리드스타일 / GLU_LINE | 와이어프레임 / GLU_SILHOUETTE | 선으로외부모서리만 / GLU_POINT | 점
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluQuadricOrientation(qobj, GLU_OUTSIDE);
    for (int i = 0; i < TargetCnt; i++) {
        if (TargetList[i].Active) {
            glFrontFace(GL_CW);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0, 2, -4));

            if (TargetList[i].Hit && TargetList[i].DeathTime > 4) {
                model = glm::translate(model, glm::vec3(TargetList[i].x,
                    TargetList[i].y + ((4.0f - TargetList[i].DeathTime) / 60.0f),
                    TargetList[i].z));
            }
            else {
                model = glm::translate(model, glm::vec3(TargetList[i].x, TargetList[i].y, TargetList[i].z));
            }

            model = glm::scale(model, glm::vec3(TargetList[i].size, TargetList[i].size, TargetList[i].size));
            shaderProgram->setMat4("model", glm::value_ptr(model));

            for (int j = 0; j < TargetList[i].RangeStep; j++) {
                float TargetSclae = (float)(TargetList[i].RangeStep - j) / (float)TargetList[i].RangeStep;
                float slotScore =
                    ScoreCaculate(TargetList[i].score, TargetList[i].RangeStep, j)
                    - (TargetList[i].LiveTime / 4.0f);

                if (TargetList[i].Hit && TargetList[i].hitRange <= j) {
                    float HitScore =
                        ScoreCaculate(TargetList[i].score, TargetList[i].RangeStep, TargetList[i].Hit)
                        - (TargetList[i].LiveTime / 4.0f);
                    if (TargetList[i].hitRange < j)
                        shaderProgram->setVec4("FullBrightColor",
                            ScoreToColor(HitScore, 50, 0, 25),
                            ScoreToColor(HitScore, 25, 0, 25),
                            ScoreToColor(HitScore, 50, 0, 25),
                            1.0f);
                    else
                        shaderProgram->setVec4("FullBrightColor", 0.0f, 1.0f, 0.0f, 1.0f);
                }
                else {
                    if (slotScore <= -10.0f) {
                        TargetSclae = -1;
                    }
                    else if (slotScore < 0) {
                        TargetSclae = ((float)(TargetList[i].RangeStep - j) + slotScore / 10.0f) / (float)TargetList[i].RangeStep;
                    }

                    switch (TargetList[i].Type)
                    {
                    case 1:
                        shaderProgram->setVec4("FullBrightColor",
                            1.0f,
                            ScoreToColor(slotScore, 10, 0, 15),
                            ScoreToColor(slotScore, 20, 0, 15),
                            ScoreToColor(slotScore, 0, 0, 15)
                        );
                        break;
                    case 2:
                        shaderProgram->setVec4("FullBrightColor",
                            ScoreToColor(slotScore, 20, 0, 15),
                            ScoreToColor(slotScore, 10, 0, 15),
                            1.0f,
                            ScoreToColor(slotScore, 0, 0, 15)
                        );
                        break;
                    default:
                        shaderProgram->setVec4("FullBrightColor",
                            ScoreToColor(slotScore, -10, 20),
                            ScoreToColor(slotScore, 10, 30),
                            ScoreToColor(slotScore, 20),
                            ScoreToColor(slotScore, 0)
                        );
                        break;
                    }
                }
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


                if (TargetList[i].Hit && TargetList[i].hitRange > j) {
                    if (TargetList[i].DeathTime < 4) {
                        float StepScale = j + ((TargetList[i].hitRange - j) * (TargetList[i].DeathTime / 4.0f));
                        TargetSclae = (float)(TargetList[i].RangeStep - StepScale) / (float)TargetList[i].RangeStep;
                        gluSphere(qobj, TargetSclae, 16, 8);
                    }
                }
                else if (TargetSclae > 0) {
                    gluSphere(qobj, TargetSclae, 16, 8);
                }
                glDisable(GL_BLEND);
            }
        }
    }


    glBindVertexArray(0);
    //mGun.draw(camera.getViewMatrix());
    mCrosshair.draw();


    glutSwapBuffers();
}

void update() {
    float currentFrame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    TargetSpawn(0, 16, 0, 4, 0, 1);

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

    //다른 셰이더 필요시 
    //sec_shaderProgram = new Shader("sec_vertex_shader.glsl", "sec_fragment_shader.glsl");


    initBackground();
   
    mCrosshair.init("crosshair2.png");


    glutDisplayFunc(render);
    glutIdleFunc(update);
    glutKeyboardFunc(processInput);
    glutPassiveMotionFunc(mouseCallback);
    glutMouseWheelFunc(handleMouseWheel);
    keepMouseCentered();
    glutSetCursor(GLUT_CURSOR_NONE);

    glutTimerFunc(25, TimerFunction, 1);

    glutMainLoop();

    delete shaderProgram;
    return 0;
}

void TimerFunction(int value)
{
    TargetTime();
    glutPostRedisplay();
    glutTimerFunc(25, TimerFunction, 1);
}