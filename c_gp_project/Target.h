#pragma once
struct Target {
	bool Active = false, Hit = false, Invincible = false, Gravity = false;
	int RangeStep = 3, hitRange = 0, Type = 0;
	float x = 0.0f, y = 0.0f, z = 0.0f,
		speedx = 0.0f, speedy = 0.0f, speedz = 0.0f,
		score = 1.0f, size = 0.25f, LiveTime = 0.0f, DeathTime = 0.0f;
};

extern Target TargetList[16];
static int TargetCnt = 16;

extern GLUquadricObj* qobj;

void TargetTime();
void TargetSpawn(int Type = 0, int Cnt = 1, int RandomType = 0,
	int RangeStep = 0, float Score = 0, float Size = 0);
float ScoreToColor(float score, float Min = 0, float Max = 0, float Step = 10.0f);
float ScoreCaculate(float Score, int RangeStep, int HitStep);