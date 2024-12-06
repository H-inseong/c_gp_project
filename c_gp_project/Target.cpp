#include "Target.h"
#include "pch.h"

Target TargetList[16];

GLUquadricObj* qobj;

/*
void TargetRender() {
	glClearColor(
		(float)BG_Red / 256.0f,
		(float)BG_Green / 256.0f,
		(float)BG_Blue / 256.0f,
		1.0f
	);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);


	//--- 그리기
glEnable(GL_CULL_FACE);
qobj = gluNewQuadric();
gluQuadricDrawStyle(qobj, GLU_FILL);
//GLU_FILL | 솔리드스타일 / GLU_LINE | 와이어프레임 / GLU_SILHOUETTE | 선으로외부모서리만 / GLU_POINT | 점
gluQuadricNormals(qobj, GLU_SMOOTH);
gluQuadricOrientation(qobj, GLU_OUTSIDE);

glm::mat4 model = glm::mat4(1.0f);
unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");

glBindVertexArray(VAO);
int vColorLocation = glGetUniformLocation(shaderProgramID, "vColor");

for (int i = 0; i < TargetCnt; i++) {
	if (TargetList[i].Active) {
		glFrontFace(GL_CW);
		model = glm::mat4(1.0f);

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
				if (TargetList[i].hitRange == j)
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
}

*/

void TargetTime() {
	for (int i = 0; i < TargetCnt; i++) {
		if (TargetList[i].Active) {
			if (TargetList[i].Gravity) {
				TargetList[i].speedy -= 0.0245f;
			}
			TargetList[i].x += TargetList[i].speedx;
			TargetList[i].y += TargetList[i].speedy;
			TargetList[i].z += TargetList[i].speedz;

			if (TargetList[i].Hit) {
				TargetList[i].DeathTime += 0.25;
				if (TargetList[i].DeathTime > 4 && !TargetList[i].Gravity) {
					TargetList[i].Gravity = true;
					TargetList[i].speedx += (float)((rand() % 10) - 5) / 100.0f;
					TargetList[i].speedy += (float)(rand() % 10) / 100.0f + 0.1f;
					TargetList[i].speedz += (float)((rand() % 10) - 5) / 100.0f;
				}
				if (TargetList[i].DeathTime > 60)
					TargetList[i].Active = false;
			}
			else if (!TargetList[i].Invincible) {
				if (TargetList[i].LiveTime < (TargetList[i].score + 10) * 4.0f) {
					TargetList[i].LiveTime += 0.25;
				}
				else {
					TargetList[i].Active = false;
				}
			}
		}
	}
}

void TargetSpawn(int Type, int Cnt, int RandomType,
	int RangeStep, float Score, float Size) {

	int CoordStep = 100;

	int Tagets;
	if (Cnt < TargetCnt) Tagets = Cnt;
	else Tagets = TargetCnt;

	for (int i = 0; i < Tagets; i++) {
		TargetList[i].Active = true;
		TargetList[i].Gravity = false;

		TargetList[i].Hit = false;
		TargetList[i].hitRange = 0;

		//TargetList[i].Hit = true;
		//TargetList[i].hitRange = 2;

		TargetList[i].Type = Type;
		TargetList[i].speedx = 0;
		TargetList[i].speedy = 0;
		TargetList[i].speedz = 0;

		TargetList[i].LiveTime = 0;
		TargetList[i].DeathTime = 0;

		TargetList[i].x = (float)(rand() % CoordStep * 2 - CoordStep) / (float)CoordStep * 4;
		TargetList[i].y = (float)(rand() % CoordStep * 2 - CoordStep) / (float)CoordStep * 2;
		TargetList[i].z = (float)(rand() % CoordStep * 2 - CoordStep) / (float)CoordStep / 4.0f;

		if (Score == 0) {
			TargetList[i].score = rand() % 40 + (RangeStep * 5);
		}
		else TargetList[i].score = Score;
		if (RangeStep == 0) {
			if (RandomType == 0) {
				if (TargetList[i].score > 10)
					TargetList[i].RangeStep = rand() % ((int)TargetList[i].score / 5 - 1) + 2;
				else if (TargetList[i].score > 5) TargetList[i].RangeStep = 2;
				else TargetList[i].RangeStep = 1;
			}
			else {
				TargetList[i].RangeStep = ((int)TargetList[i].score / 5) + 1;
			}
		}
		else TargetList[i].RangeStep = RangeStep;
		if (Size == 0) {
			TargetList[i].size = (float)(rand() % 10) / 100.0f + 0.1f;
		}
		else TargetList[i].size = Size;
	}
}

float ScoreToColor(float score, float Min, float Max, float Step) {
	if (score < Min - Step)
		return 0.0f;
	else if (score < Min)
		return (score + Step - Min) / Step;
	else if (Max == 0 || score < Max)
		return 1.0f;
	else if (score < Max + Step)
		return (Max + Step - score) / Step;
	else
		return 0.0f;
}

float ScoreCaculate(float Score, int RangeStep, int HitStep) {
	if (RangeStep > 1) return Score * (float)HitStep / (float)(RangeStep - 1);
	else return Score;
}

