#include "Target.h"
#include "pch.h"

Target tList[16];

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
		if (tList[i].Active) {
			if (tList[i].Gravity) {
				tList[i].speedy -= 0.0245f;
			}
			tList[i].x += tList[i].speedx;
			tList[i].y += tList[i].speedy;
			tList[i].z += tList[i].speedz;

			if (tList[i].Hit) {
				tList[i].DeathTime += 0.25;
				if (tList[i].DeathTime > 4 && !tList[i].Gravity) {
					tList[i].Gravity = true;
					tList[i].speedx += (float)((rand() % 10) - 5) / 100.0f;
					tList[i].speedy += (float)(rand() % 10) / 100.0f + 0.1f;
					tList[i].speedz += (float)((rand() % 10) - 5) / 100.0f;
				}
				if (tList[i].DeathTime > 60)
					tList[i].Active = false;
			}
			else if (!tList[i].Invincible) {
				if (tList[i].LiveTime < (tList[i].score + 10) * 4.0f) {
					tList[i].LiveTime += 0.25;
				}
				else {
					tList[i].Active = false;
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
		tList[i].Active = true;
		tList[i].Gravity = false;

		tList[i].Hit = false;
		tList[i].hitRange = 0;

		//TargetList[i].Hit = true;
		//TargetList[i].hitRange = 2;

		tList[i].Type = Type;
		tList[i].speedx = 0;
		tList[i].speedy = 0;
		tList[i].speedz = 0;

		tList[i].LiveTime = 0;
		tList[i].DeathTime = 0;

		tList[i].x = (float)(rand() % CoordStep * 2 - CoordStep) / (float)CoordStep * 4;
		tList[i].y = (float)(rand() % CoordStep * 2 - CoordStep) / (float)CoordStep * 2;
		tList[i].z = (float)(rand() % CoordStep * 2 - CoordStep) / (float)CoordStep / 4.0f;

		if (Score == 0) {
			tList[i].score = rand() % 40 + (RangeStep * 5);
		}
		else tList[i].score = Score;
		if (RangeStep == 0) {
			if (RandomType == 0) {
				if (tList[i].score > 10)
					tList[i].RangeStep = rand() % ((int)tList[i].score / 5 - 1) + 2;
				else if (tList[i].score > 5) tList[i].RangeStep = 2;
				else tList[i].RangeStep = 1;
			}
			else {
				tList[i].RangeStep = ((int)tList[i].score / 5) + 1;
			}
		}
		else tList[i].RangeStep = RangeStep;
		if (Size == 0) {
			tList[i].size = (float)(rand() % 10) / 100.0f + 0.1f;
		}
		else tList[i].size = Size;
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

int CheckCenterTarget(const Camera& camera) {
	glm::vec3 camPos = camera.getPosition();
	glm::vec3 camDir = glm::normalize(camera.getFront());

	int hitIndex = -1;
	float closestDistance = std::numeric_limits<float>::max();

	for (int i = 0; i < TargetCnt; i++) {
		if (!tList[i].Active) continue;

		// 타겟 정보를 구 중심과 반지름으로 취급
		glm::vec3 targetCenter(tList[i].x, tList[i].y, tList[i].z);
		float radius = tList[i].size;

		// 광선 파라메트릭: R(t) = camPos + t * camDir (t >= 0)
		// 구 방정식: |P - C| = r
		// 충돌 판정: (camPos + camDir*t - targetCenter)^2 = r^2
		// 이를 t에 대해 풀면 2차방정식: a*t^2 + b*t + c = 0
		glm::vec3 oc = camPos - targetCenter;
		float a = glm::dot(camDir, camDir);
		float b = 2.0f * glm::dot(oc, camDir);
		float c = glm::dot(oc, oc) - radius * radius;

		float discriminant = b * b - 4 * a * c;
		if (discriminant < 0) {
			continue;
		}

		float sqrtD = sqrt(discriminant);
		float t1 = (-b - sqrtD) / (2.0f * a);
		float t2 = (-b + sqrtD) / (2.0f * a);

		float tHit = -1.0f;
		if (t1 > 0 && t2 > 0) {
			tHit = std::min(t1, t2);
		}
		else if (t1 > 0) {
			tHit = t1;
		}
		else if (t2 > 0) {
			tHit = t2;
		}
		if (tHit > 0 && tHit < closestDistance) {
			closestDistance = tHit;
			hitIndex = i;
		}
	}
	return hitIndex;
}

float EvaluateTargetHitScore(const Camera& camera, int targetIndex) {
	if (targetIndex < 0 || targetIndex >= TargetCnt || !tList[targetIndex].Active) {
		return 0.0f;
	}

	glm::vec3 camPos = camera.getPosition();
	glm::vec3 camDir = glm::normalize(camera.getFront());
	glm::vec3 targetCenter(tList[targetIndex].x, tList[targetIndex].y, tList[targetIndex].z);
	float radius = tList[targetIndex].size;
	glm::vec3 oc = camPos - targetCenter;
	float a = glm::dot(camDir, camDir);
	float b = 2.0f * glm::dot(oc, camDir);
	float c = glm::dot(oc, oc) - radius * radius;

	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		// 이론상 여기 오면 안 되지만 혹시 몰라 0점 반환
		return 0.0f;
	}

	float sqrtD = sqrt(discriminant);
	float t1 = (-b - sqrtD) / (2.0f * a);
	float t2 = (-b + sqrtD) / (2.0f * a);

	float tHit = -1.0f;
	if (t1 > 0 && t2 > 0) tHit = std::min(t1, t2);
	else if (t1 > 0) tHit = t1;
	else if (t2 > 0) tHit = t2;

	if (tHit <= 0) {
		return 0.0f;
	}

	glm::vec3 hitPos = camPos + camDir * tHit;
	float dist = glm::length(hitPos - targetCenter);

	// 어느 링에 맞았는지 판별
	// j=0 : 가장 바깥쪽 링, j=(RangeStep-1) : 가장 안쪽 링
	int hitRing = -1;
	for (int j = 0; j < tList[targetIndex].RangeStep; j++) {
		float ringRadius = ((float)(tList[targetIndex].RangeStep - j) / (float)tList[targetIndex].RangeStep) * tList[targetIndex].size;
		if (dist <= ringRadius) {
			hitRing = j;
			break;
		}
	}
	if (hitRing == -1) {
		return 0.0f;
	}

	float hitScore = ScoreCaculate(tList[targetIndex].score, tList[targetIndex].RangeStep, hitRing);

	tList[targetIndex].Hit = true;
	tList[targetIndex].hitRange = hitRing;

	return hitScore;
}