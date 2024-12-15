#pragma once
struct Target {
	bool Active = false, Hit = false, Invincible = false, Gravity = false;
	int RangeStep = 3, hitRange = 0, Type = 0;
	float x = 0.0f, y = 0.0f, z = 0.0f,
		speedx = 0.0f, speedy = 0.0f, speedz = 0.0f,
		score = 1.0f, scoreDecay = 0.25f, size = 0.2f, LiveTime = 0.0f, DeathTime = 0.0f;
};

extern Target tList[16];
static int TargetCnt = 16;

extern GLUquadricObj* qobj;

void TargetTime();
void TargetPlace(int Num);

void TargetSpawn(int Num, int Type, int RandomType,
	int RangeStep, float Score, float Size);
void TargetStackSpawn(int Type = 0, int Cnt = 1, int RandomType = 0,
int RangeStep = 0, float Score = 0, float Size = 0);
void TargetDispenser(int Type, int RandomType,
	int RangeStep, float Score, float Size);

float ScoreToColor(float score, float Min = 0, float Max = 0, float Step = 10.0f);
float ScoreCaculate(float Score, int RangeStep, int HitStep, float LiveTime);
int CheckCenterTarget(const Camera& camera);
float EvaluateTargetHitScore(const Camera& camera, int targetIndex);

bool TargetHitCheck(const Camera& camera, glm::vec3 TargetPos, float size);