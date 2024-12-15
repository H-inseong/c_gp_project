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

			if (tList[i].Invincible) {
				tList[i].orbitTimer++;
				if (tList[i].orbitTimer > 60) {
					tList[i].orbitTimer = 0;
					tList[i].orbitSpeed *= rand() % 2 * 2 - 1;
				}

				tList[i].orbitAngle += tList[i].orbitSpeed;
				tList[i].x = tList[i].orbitRadius * cos(tList[i].orbitAngle * M_PI);
				tList[i].y = 1.5f;
				tList[i].z = tList[i].orbitRadius * sin(tList[i].orbitAngle * M_PI) + 5;
			}
			else {
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
				else if (tList[i].LiveTime < (tList[i].score + 10) * 16.0f) {
						tList[i].LiveTime += 0.25;
				}
				else {
					tList[i].Active = false;
				}
			}
		}
	}
}

void TargetPlace(int Num) {
	int CoordStep = 100;
	float
		dis_x = 0,
		dis_y = 0,
		dis_z = 0,
		Shell_Size = 0;

	bool Collide = true;
	int tryPlace = 0;

	while (Collide || tryPlace > 1024) {
		tList[Num].x = (float)(rand() % CoordStep * 2 - CoordStep) / (float)CoordStep * 10;
		tList[Num].y = (float)(rand() % CoordStep * 2 - CoordStep) / (float)CoordStep * 2 + 3;
		tList[Num].z = (float)(rand() % CoordStep * 2 - CoordStep) / (float)CoordStep / 4.0f;
		Collide = false;
		tryPlace++;
		for (int i = 0; i < TargetCnt; i++) {
			if (tList[i].Active && i != Num) {
				dis_x = tList[i].x - tList[Num].x,
					dis_y = tList[i].y - tList[Num].y,
					dis_z = tList[i].z - tList[Num].z,
					Shell_Size = tList[i].size + tList[Num].size;
				if (pow(dis_x, 2) + pow(dis_y, 2) + pow(dis_z, 2) < pow(Shell_Size, 2)) {
					Collide = true;
					break;
				}
			}
		}
	}
}

void TargetSpawn(int Num, int Type, int RandomType,
	float Size, int RangeStep, float Score, float ScoreDecay) {
	tList[Num].Active = true;
	tList[Num].Hit = false;
	tList[Num].hitRange = 0;
	tList[Num].LiveTime = 0;
	tList[Num].DeathTime = 0;
	tList[Num].speedx = 0;
	tList[Num].speedy = 0;
	tList[Num].speedz = 0;
	tList[Num].Gravity = false;

	tList[Num].Type = Type;
	tList[Num].scoreDecay = ScoreDecay;

	if (Score == 0) {
		if (RangeStep != 0) tList[Num].score = rand() % 40 + (RangeStep * 5);
		else tList[Num].score = rand() % 40 + 10;
	}
	else tList[Num].score = Score;
	if (RangeStep == 0) {
		if (RandomType == 0) {
			if (tList[Num].score > 10)
				tList[Num].RangeStep = rand() % ((int)tList[Num].score / 5 - 1) + 2;
			else if (tList[Num].score > 5) tList[Num].RangeStep = 2;
			else tList[Num].RangeStep = 1;
		}
		else {
			tList[Num].RangeStep = ((int)tList[Num].score / 5) + 1;
		}
	}
	else tList[Num].RangeStep = RangeStep;
	if (Size == 0) {
		tList[Num].size = (float)(rand() % 10) / 100.0f + 0.1f;
	}
	else tList[Num].size = Size;

	TargetPlace(Num);
}

void TargetStackSpawn(int Type, int Cnt, int RandomType,
	float Size, int RangeStep, float Score, float ScoreDecay) {

	int Tagets;
	if (Cnt < TargetCnt) Tagets = Cnt;
	else Tagets = TargetCnt;

	for (int i = 0; i < Tagets; i++) {
		TargetSpawn(i, Type, RandomType, Size, RangeStep, Score, ScoreDecay);
	}
}

void TargetDispenser(int Type, int RandomType,
	float Size, int RangeStep, float Score, float ScoreDecay) {
	for (int i = 0; i < TargetCnt; i++) {
		if (!tList[i].Active) {
			TargetSpawn(i, Type, RandomType, Size, RangeStep, Score, ScoreDecay);
			break;
		}
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

float ScoreCaculate(float Score, int RangeStep, int HitStep, float LiveTime) {
	if (RangeStep <= 1) Score + 5 - LiveTime;
	else {
		float returnScore = (Score * (float)HitStep / (float)(RangeStep - 1)) + 5 - LiveTime;
		if (returnScore < 0) return returnScore * (LiveTime / 2 + 1);
		else return returnScore;
	}
}

int CheckCenterTarget(const Camera& camera) {
	glm::vec3 camPos = camera.getPosition();
	glm::vec3 camDir = glm::normalize(camera.getFront());

	int hitIndex = -1;
	float closestDistance = std::numeric_limits<float>::max();

	for (int i = 0; i < TargetCnt; i++) {
		if (!tList[i].Active || tList[i].Hit) continue;

		// 타겟 정보를 구 중심과 반지름으로 취급
		glm::vec3 targetCenter(tList[i].x * 2, tList[i].y * 2, tList[i].z * 2);
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
	glm::vec3 targetCenter(tList[targetIndex].x * 2, tList[targetIndex].y * 2, tList[targetIndex].z * 2);
	// 어느 링에 맞았는지 판별
	// j=0 : 가장 바깥쪽 링, j=(RangeStep-1) : 가장 안쪽 링
	int hitRing = -1;
	float hitScore = 0.0f;
	for (int j = tList[targetIndex].RangeStep - 1; j > -1; j--) {
		hitScore = ScoreCaculate(tList[targetIndex].score, tList[targetIndex].RangeStep, j, tList[targetIndex].LiveTime * tList[targetIndex].scoreDecay);
		if (hitScore > 0.0f) {
			float ringRadius = ((float)(tList[targetIndex].RangeStep - j) / (float)tList[targetIndex].RangeStep) * tList[targetIndex].size;
			if (TargetHitCheck(camera, targetCenter, ringRadius)) {
				hitRing = j;
				break;
			}
		}
	}
	if (hitRing == -1) {
		return 0.0f;
	}
	hitScore = ScoreCaculate(tList[targetIndex].score, tList[targetIndex].RangeStep, hitRing, tList[targetIndex].LiveTime * tList[targetIndex].scoreDecay);
	tList[targetIndex].Hit = true;
	tList[targetIndex].hitRange = hitRing;
	return hitScore;
}


bool TargetHitCheck(const Camera& camera, glm::vec3 TargetPos, float size) {
	glm::vec3 camPos = camera.getPosition();
	glm::vec3 camDir = glm::normalize(camera.getFront());
	glm::vec3 oc = camPos - TargetPos;
	float a = glm::dot(camDir, camDir);
	float b = 2.0f * glm::dot(oc, camDir);
	float c = glm::dot(oc, oc) - size * size;
	float discriminant = b * b - 4 * a * c;

	if (discriminant < 0) return false;
	else return true;
}