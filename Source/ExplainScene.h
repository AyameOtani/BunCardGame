#pragma once

#include "Scene.h"
#include "DxLib.h"
#include "MouseGraph.h"

class ExplainScene : public Scene
{

public:
	ExplainScene();
	~ExplainScene() override;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	

private:
	int mnItemEHandle = -1; // 背景画像
	int mnGameButton = -1; // ゲームへ進むボタン
	int mnTitleButton = -1; // タイトルに戻るボタン

	int mnBagHandle = -1;

	MouseGraph* mpGameStart = nullptr;
	MouseGraph* mpBackTitle = nullptr;

	bool mbSelectScene = false; // 選択画面に行くか
	bool mbTitleScene = false;  // タイトルに行くか
	float mfFadeAlpha = 0.0f; // フェードのやつ

};
