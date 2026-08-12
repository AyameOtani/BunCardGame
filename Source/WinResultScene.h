#pragma once
#include "Scene.h"
#include "DxLib.h"
#include "MouseGraph.h"
#include "Score.h"

class WinResultScene : public Scene
{
public:
	WinResultScene();
	~WinResultScene() override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;

private:
	MouseGraph* mpMouseGraph = nullptr;
	Score m_Score; // スコアのやつ

	int mnBagHandle = -1; /// 背景画像
};
