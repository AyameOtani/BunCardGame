#pragma once

#include "Scene.h"
#include "DxLib.h"
#include "Button.h"
#include "MouseGraph.h"
#include "Utility.h"
#include "VolumeBar.h"

class Title : public Scene
{
public:
	Title();
	~Title() override;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	// 次にどの画面にいくかの種類
	enum NextScene
	{
		NONE_SCENE,
		SELECT_SCENE,
		EXPLAIN_SCENE,
	};

	NextScene mNextScene = NONE_SCENE;

private:
	// ボタン
	std::unique_ptr<MouseGraph> mpGameStart;
	std::unique_ptr<MouseGraph> mpExplainGraph;
	std::unique_ptr<MouseGraph> mpOptionButton;
	std::unique_ptr<MouseGraph> mpMusicClose;

	// 音量バー
	std::unique_ptr<VolumeBar> mpBgmVolumeBar;
	std::unique_ptr<VolumeBar> mpSeVolumeBar;

	// 画像ハンドル
	int mnRogoHandle = -1;
	int mnBagHandle = -1;
	int mnCardHandle = -1;
	int mnVolumeSettingsBg = -1;

	// 菊池
	int mnKorukuitaHandle = -1;
	int mnBatuHandle = -1;
	int mnOnpuHandle = -1;

	// 音量設定を開いているか
	bool mbOption = false;

	// タイトル演出
	bool mbMouseButton = false;
	bool mbWhite = false;

	// カード演出
	float mnCardX = 0.0f;
	float mnCardY = 0.0f;
	float mnCardAngle = 3.0f;
	float mnCardRota = 0.01f;

	// カードのターゲット位置
	float targetX;
	float targetY;
	float targetAngle = -0.05f;
	float targetRota = 1.0f;

	// 白いBOXの透明度
	float mfWhiteBoxAlpha = 0.0f;

	// ボタン演出
	float mfStartX;
	float mfStartY;
	float mfExplainX;
	float mfExplainY;

	float mfTargetStartX;
	float mfTargetStartY;
	float mfTargetExplainX;
	float mfTargetExplainY;

	// ロゴ演出
	float mfTurnY;
	bool mbInitialize = false;
	float mfLogoTime = 0.0f;
};