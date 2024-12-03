#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GL/freeglut.h>
/*
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
*/
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Camera.h"
#include "Target.h"

// 화면 크기
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// 카메라 객체
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));

GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
char* filetobuf(const char* file);

void TimerFunction(int value);
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
	int hittarget = 0, hitorign = 0;
	switch (key)
	{
	case 'w':
		camera.processKeyboardInput('W', deltaTime);
		break;
	case 'a':
		camera.processKeyboardInput('A', deltaTime);
		break;
	case 's':
		camera.processKeyboardInput('S', deltaTime);
		break;
	case 'd':
		camera.processKeyboardInput('D', deltaTime);
		break;
	case '-':
		camera.decreaseSensitivity();
		break;
	case '=':
		camera.increaseSensitivity();
		break;
	case 'r':
		TargetSpawn(rand() % 3, 16, 0, 0, 0, 1);
		break;
	case 'h':
		hittarget = rand() % TargetCnt;
		hitorign = hittarget - 1;
		while ((!TargetList[hittarget].Active || TargetList[hittarget].Hit) && hittarget != hitorign)
		{
			hittarget = (hittarget + 1) % TargetCnt;
		}
		TargetList[hittarget].Hit = true;
		TargetList[hittarget].hitRange = rand() % TargetList[hittarget].RangeStep;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
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
	glm::mat4 model = glm::mat4(1.0f);

    // 단색으로 바닥 그리기
    glBindVertexArray(VAO);
    //glUseProgram(0); // 고정 파이프라인 사용
	glUseProgram(shaderProgramID);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));

	//------
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "model");
	int vColorLocation = glGetUniformLocation(shaderProgramID, "aColor");
	//-----
	glFrontFace(GL_CCW);
	glUniform4f(vColorLocation, 0.6f, 0.8f, 0.9f, 1.0f);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glColor3f(0.6f, 0.8f, 0.9f); // 하늘색 바닥
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glEnd();

    //-----

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
			model = glm::translate(model, glm::vec3(0,2,0));

			if (TargetList[i].Hit && TargetList[i].DeathTime > 4) {
				model = glm::translate(model, glm::vec3(TargetList[i].x,
					TargetList[i].y + ((4.0f - TargetList[i].DeathTime) / 60.0f),
					TargetList[i].z));
			}
			else {
				model = glm::translate(model, glm::vec3(TargetList[i].x, TargetList[i].y, TargetList[i].z));
			}

			model = glm::scale(model, glm::vec3(TargetList[i].size, TargetList[i].size, TargetList[i].size));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

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
						glUniform4f(vColorLocation,
							ScoreToColor(HitScore, 50, 0, 25),
							ScoreToColor(HitScore, 25, 0, 25),
							ScoreToColor(HitScore, 50, 0, 25),
							1.0f);
					else
						glUniform4f(vColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
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
						glUniform4f(vColorLocation,
							1.0f,
							ScoreToColor(slotScore, 10, 0, 15),
							ScoreToColor(slotScore, 20, 0, 15),
							ScoreToColor(slotScore, 0, 0, 15)
						);
						break;
					case 2:
						glUniform4f(vColorLocation,
							ScoreToColor(slotScore, 20, 0, 15),
							ScoreToColor(slotScore, 10, 0, 15),
							1.0f,
							ScoreToColor(slotScore, 0, 0, 15)
						);
						break;
					default:
						glUniform4f(vColorLocation,
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
    glutSwapBuffers();
}

void update() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {

	TargetSpawn(0, 16, 0, 4, 0, 1);

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

	//------
	make_shaderProgram();

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

	glutTimerFunc(25, TimerFunction, 1);

    glutMainLoop();

    return 0;
}


//-----------
void make_vertexShaders()
{
	vertexSource = filetobuf("vertex_shader.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment_shader.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_shaderProgram()
{
	make_vertexShaders();
	make_fragmentShaders();

	shaderProgramID = glCreateProgram();

	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgramID);
}
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;
	return buf;
}

void TimerFunction(int value)
{
	TargetTime();
	glutPostRedisplay();
	glutTimerFunc(25, TimerFunction, 1);
}