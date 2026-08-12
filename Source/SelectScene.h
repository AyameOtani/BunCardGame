#pragma once

#include "Scene.h"
#include "MouseGraph.h"
#include "SceneManager.h"


class SelectScene : public Scene
{
public:
	SelectScene();
	~SelectScene() override;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// クリック判定の画像ハンドル
	MouseGraph* mpEasy = nullptr; // 簡単
	MouseGraph* mpNormal = nullptr; // 普通
	MouseGraph* mpHard = nullptr; // 難しい

	int mnEasyHandle = -1; // 簡単のハンドル
	int mnNormalHandle = -1; // 普通のハンドル
	int mnHardHandle = -1; // 難しいのハンドル
	int mnEasy_NoSelectHandle = -1; // 簡単のシルエットのハンドル
	int mnNormal_NoSelectHandle = -1; // 普通のシルエットのハンドル
	int mnHard_NoSelectHandle = -1; // 難しいのシルエットのハンドル

	int mnBagHandle = -1; // 簡単のハンドル
	int mnEasyBagHandle = -1; // 簡単のハンドル
	int mnNormalBagHandle = -1; // 普通のハンドル
	int mnHardBagHandle = -1; // 難しいのハンドル

	bool mbSceneChange = false; // シーンをかえていいか
	float mfFadeAlpha = 0.0f; // フェードのやつ
	SceneManager::GScene mNextScene; //　次のシーンの保存

	//int mnSelectHandle = -1; // ステージ選択しようの画像
};