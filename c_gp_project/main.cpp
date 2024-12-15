#include "pch.h"

GLuint VAO, VBO;
Shader* shaderProgram;
Shader* target_shaderProgram;

Camera camera(glm::vec3(0.0f, 3.0f, 10.0f));
Background mBackground;
Crosshair mCrosshair;
Gun mGun;

int stage = 1; // 1:5x5사이즈 2:원운동 불사타겟 3:크기변화 타겟
int score = 0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float gunRecoil = 0.0f;
float gunRecoilMax = 0.1f;
float gunRecoilSpeed = 1.0f;

bool firstMouse = true;
bool leftMousePressed = false;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

glm::vec3 lightPos(0.0f, 20.0f, 0.0f);

bool keys[256] = { false };

int GameStage = -2;
// -2: 샌드박스 | -1: 완전 랜덤 모드 | 0: 랜덤모드 |
// 1: 부패 = 0, 단계 = 1, 개수 = 4 | 2: 부패 = 0, 단계 2, 개수 = 8 | 3: 부패 = 0, 단계 4, 개수 = 16 |
// 4: 트래킹 모드, 개수 = 16 | 5: 부패 = 0.25, 단계 4, 개수 = 16 | 6: 부패 0.5, 단계 4, 개수 = 16

int GameProgressCnt = 0;

bool TargetDispenserOn = true;
int TDCoolDown = 0;

int TargetType = 0;
int TargetStep = 4;
float TargetSize = 1;
float TargetScore = 25;
float TargetScoreDecay = 0;

void GameProgress();

void keyDown(unsigned char key, int x, int y) {
    keys[key] = true;

    if (GameStage == -2) {
        switch (key)
        {
        case 'o':
            TargetType = rand() % 3;
            TargetDispenserOn = false;
            TDCoolDown = 0;
            TargetStackSpawn(TargetType, 16, 0, TargetSize, TargetStep, TargetScore, TargetScoreDecay);
            break;
        case 'p':
            TargetType = rand() % 3;
            TargetDispenserOn = !TargetDispenserOn;
            TDCoolDown = 0;
            for (int i = 0; i < TargetCnt; i++) {
                tList[i].Active = false;
            }
            break;
        case 'l':
            if (TargetSize == 0) {
                TargetSize = 1;
            }
            else TargetSize = 0;
            break;
        case 'k':
            if (TargetStep < 5) {
                TargetStep++;
            }
            else TargetStep = 0;
            break;
        case 'n':
            if (TargetScore == 0) {
                TargetScore = 10;
            }
            else if (TargetScore < 50) {
                TargetScore += 5;
            }
            else TargetScore = 0;
            break;
        case 'm':
            if (TargetScoreDecay == 0) {
                TargetScoreDecay = 0.125;
            }
            else if (TargetScoreDecay == 0.125) {
                TargetScoreDecay = 0.25;
            }
            else if (TargetScoreDecay == 0.25) {
                TargetScoreDecay = 0.5;
            }
            else if (TargetScoreDecay == 0.5) {
                TargetScoreDecay = 1.0;
            }
            else TargetScoreDecay = 0;
            break;
        default:
            break;
        }
    }
    else if (GameStage == -1) {
        switch (key)
        {
        case 'r':
            TargetSize = (float)(rand() % 10) / 100.0f + 0.1f;
            TargetStep = rand() % 4 + 1;
            TargetScore = (rand() % 8 + 2) * 5;
            TargetScoreDecay = 1 / pow(2, rand() % 3);
            break;
        case 'o':
            TargetType = 2;
            TargetDispenserOn = false;
            TDCoolDown = 0;
            TargetStackSpawn(TargetType, 16, 0, TargetSize, TargetStep, TargetScore, TargetScoreDecay);
            break;
        case 'p':
            TargetType = 2;
            TargetDispenserOn = !TargetDispenserOn;
            TDCoolDown = 0;
            for (int i = 0; i < TargetCnt; i++) {
                tList[i].Active = false;
            }
            break;
        case 'l':
            TargetSize = (float)(rand() % 10) / 100.0f + 0.1f;
            break;
        case 'k':
            TargetStep = rand() % 4 + 1;
            break;
        case 'n':
            TargetScore = (rand() % 8 + 2) * 5;
            break;
        case 'm':
            TargetScoreDecay = 1 / pow(2, rand() % 3);
            break;
        default:
            break;
        }
    }
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
            gunRecoil = gunRecoilMax;
            int targetIndex = CheckCenterTarget(camera);
            if (targetIndex != -1) {
                // 점수 처리
                float score = EvaluateTargetHitScore(camera, targetIndex);
                if (score > 0) {
                    GameProgressCnt--;
                    if (GameStage > 0 && GameProgressCnt <= 0) GameProgress();
                }
                std::cout << "Hit target " << targetIndex << " with score: " << score << std::endl;
            }

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
    glm::mat4 model = glm::mat4(1.0f);

    target_shaderProgram->use();
    target_shaderProgram->setMat4("view", glm::value_ptr(view));
    target_shaderProgram->setMat4("projection", glm::value_ptr(projection));
    target_shaderProgram->setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
    target_shaderProgram->setVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    target_shaderProgram->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    glEnable(GL_CULL_FACE);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    //gluQuadricDrawStyle(qobj, GLU_LINE);

    //GLU_FILL | 솔리드스타일 / GLU_LINE | 와이어프레임 / GLU_SILHOUETTE | 선으로외부모서리만 / GLU_POINT | 점
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluQuadricOrientation(qobj, GLU_OUTSIDE);
    for (int i = 0; i < TargetCnt; i++) {
        if (tList[i].Active) {
            glFrontFace(GL_CW);
            model = glm::mat4(1.0f);

            if (tList[i].Hit && tList[i].DeathTime > 4) {
                model = glm::translate(model, glm::vec3(tList[i].x,
                    tList[i].y + ((4.0f - tList[i].DeathTime) / 60.0f),
                    tList[i].z));
            }
            else {
                model = glm::translate(model, glm::vec3(tList[i].x, tList[i].y, tList[i].z));
            }

            model = glm::scale(model, glm::vec3(tList[i].size));
            target_shaderProgram->setMat4("model", glm::value_ptr(model));

            for (int j = 0; j < tList[i].RangeStep; j++) {
                float TargetSclae = (float)(tList[i].RangeStep - j) / (float)tList[i].RangeStep;
                float slotScore = ScoreCaculate(tList[i].score, tList[i].RangeStep, j, (tList[i].LiveTime * tList[i].scoreDecay));

                if (tList[i].Hit && tList[i].hitRange <= j) {
                    float HitScore =
                        ScoreCaculate(tList[i].score, tList[i].RangeStep, tList[i].hitRange, (tList[i].LiveTime * tList[i].scoreDecay));
                    if (tList[i].hitRange < j) {
                        switch (tList[i].Type)
                        {
                        case 1:
                            target_shaderProgram->setVec4("FullBrightColor",
                                1.0f,
                                ScoreToColor(HitScore, 10, 0, 15),
                                ScoreToColor(HitScore, 20, 0, 15),
                                ScoreToColor(HitScore, 0, 0, 15)
                            );
                            break;
                        case 2:
                            target_shaderProgram->setVec4("FullBrightColor",
                                ScoreToColor(HitScore, 20, 0, 15),
                                ScoreToColor(HitScore, 10, 0, 15),
                                1.0f,
                                ScoreToColor(HitScore, 0, 0, 15)
                            );
                            break;
                        default:
                            target_shaderProgram->setVec4("FullBrightColor",
                                ScoreToColor(HitScore, -10, 20),
                                ScoreToColor(HitScore, 10, 30),
                                ScoreToColor(HitScore, 20),
                                ScoreToColor(HitScore, 0)
                            );
                            break;
                        }
                    }
                    else if (tList[i].hitRange == tList[i].RangeStep - 1 && j == tList[i].RangeStep - 1) {
                        target_shaderProgram->setVec4("FullBrightColor",
                            ScoreToColor(HitScore, 50, 0, 25),
                            ScoreToColor(HitScore, 25, 0, 25) / 2.0f + 0.5f,
                            ScoreToColor(HitScore, 50, 0, 25),
                            1.0f);
                    }
                    else
                        target_shaderProgram->setVec4("FullBrightColor", 0.0f, 1.0f, 0.0f, 1.0f);
                }
                else {
                    if (slotScore <= -10.0f) {
                        TargetSclae = -1;
                    }
                    else if (slotScore < 0) {
                        TargetSclae = ((float)(tList[i].RangeStep - j) + slotScore / 10.0f) / (float)tList[i].RangeStep;
                    }

                    switch (tList[i].Type)
                    {
                    case 1:
                        target_shaderProgram->setVec4("FullBrightColor",
                            1.0f,
                            ScoreToColor(slotScore, 10, 0, 15),
                            ScoreToColor(slotScore, 20, 0, 15),
                            ScoreToColor(slotScore, 0, 0, 15)
                        );
                        break;
                    case 2:
                        target_shaderProgram->setVec4("FullBrightColor",
                            ScoreToColor(slotScore, 20, 0, 15),
                            ScoreToColor(slotScore, 10, 0, 15),
                            1.0f,
                            ScoreToColor(slotScore, 0, 0, 15)
                        );
                        break;
                    default:
                        target_shaderProgram->setVec4("FullBrightColor",
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

                if (TargetSclae > 0) {
                    if (tList[i].Hit && tList[i].hitRange > j) {
                        if (tList[i].DeathTime < 4) {
                            float StepScale = j + ((tList[i].hitRange - j) * (tList[i].DeathTime / 4.0f));
                            TargetSclae = (float)(tList[i].RangeStep - StepScale) / (float)tList[i].RangeStep;
                            gluSphere(qobj, TargetSclae, 16, 8);
                        }
                    }
                    else {
                        gluSphere(qobj, TargetSclae, 16, 8);
                    }
                }
                glDisable(GL_BLEND);
            }
        }
    }

    glFrontFace(GL_CCW);
    shaderProgram->use();

    shaderProgram->setMat4("view", glm::value_ptr(view));
    shaderProgram->setMat4("projection", glm::value_ptr(projection));
    shaderProgram->setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
    shaderProgram->setVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    shaderProgram->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shaderProgram->setInt("texture1", 0);
    shaderProgram->setMat4("model", glm::value_ptr(model));
    {
        glm::mat4 backgroundModel = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
        backgroundModel = glm::translate(backgroundModel, glm::vec3(0.0f, 0.0f, 5.0f));
        shaderProgram->setMat4("model", glm::value_ptr(backgroundModel));
        mBackground.draw(view, projection);
    }
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
        gunModel = glm::rotate(gunModel, glm::radians(gunRecoil*200.0f), glm::vec3(1.0f, 0.0f, 1.0f));
        gunModel = glm::scale(gunModel, glm::vec3(0.2f));

        shaderProgram->setMat4("model", glm::value_ptr(gunModel));
        mGun.draw(view, projection);
    }
    mCrosshair.draw();

    glutSwapBuffers();
}

void GameProgress() {
    switch (GameStage)
    {
    case 0:
        TargetType = 0;
        TargetStep = 1;
        TargetScoreDecay = 0;
        GameProgressCnt = 4;
        GameStage++;
        break;
    case 1:
        TargetType = 0;
        TargetStep = 2;
        TargetScoreDecay = 0;
        GameProgressCnt = 8;
        GameStage++;
        break;
    case 2:
        TargetType = 0;
        TargetStep = 4;
        TargetScoreDecay = 0;
        GameProgressCnt = 16;
        GameStage++;
        break;
    case 3:
        TargetType = 2;
        TargetStep = 4;
        TargetScoreDecay = 0;
        GameProgressCnt = 16;
        GameStage++;
        break;
    case 4:
        TargetType = 1;
        TargetStep = 4;
        TargetScoreDecay = 0.25;
        GameProgressCnt = 16;
        GameStage++;
        break;
    case 5:
        TargetType = 1;
        TargetStep = 4;
        TargetScoreDecay = 0.5;
        GameProgressCnt = 16;
        GameStage++;
        break;
    case 6:
        TargetType = 0;
        TargetStep = 0;
        TargetSize = 0;
        TargetScore = 0;
        TargetScoreDecay = 1;
        break;
    default:
        break;
    }
}

void update(int value) {
    float currentFrame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput();
    lightPos = camera.getPosition();
    lightPos.y += 30.f;
    lightPos.z += 5.f;

    if (gunRecoil > 0.0f) {
        gunRecoil -= gunRecoilSpeed * deltaTime;
        if (gunRecoil < 0.0f) {
            gunRecoil = 0.0f;
        }
    }

    TargetTime();
    if (TargetDispenserOn) {
        TDCoolDown++;
        if (TDCoolDown > 60) {
            TDCoolDown = 0;
            TargetDispenser(TargetType, 0, TargetSize, TargetStep, TargetScore, TargetScoreDecay);
        }
    }

    glutPostRedisplay();

    glutTimerFunc(25, update, 1);
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
    target_shaderProgram = new Shader("target_vertex_shader.glsl", "target_fragment_shader.glsl");

    mCrosshair.init();
    mGun.init("Resource\\Gun.obj", "Resource\\Gun.jpg");
    mBackground.init("Resource\\background.obj", "Resource\\background.png");

    GameStage = 0;
    GameProgress();

    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialKeyCallback);

    glutDisplayFunc(render);
    //glutIdleFunc(update);
    glutTimerFunc(25, update, 1);

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