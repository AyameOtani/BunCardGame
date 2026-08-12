#pragma once
#include <string>

struct Part
{
	std::string name;

	float angle = 0.0f;      // 今の角度
	float targetAngle = 0.0f;// 目標角度

	float xOffset = 0.0f;		// 現在の横方向のズレ
	float targetXOffset = 0.0f;	// 目標の横方向のズレ

	float yOffset = 0.0f;		// 現在の縦方向のズレ
	float targetYOffset = 0.0f;	// 目標の縦方向のズレ

	int graphHandle = -1;		// パーツの画像
};

void UpdatePart(Part& p);
void DrawPart(const Part& p, float baseX, float baseY, float size);