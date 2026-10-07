#pragma once

#include "Scene.h"
#include "DxLib.h"
#include "Button.h"
#include "MouseGraph.h"
#include "Utility.h"
#include "VolumeBar.h"
#include "VolumeSetting.h"

class Title : public Scene
{
public:
	Title();
	~Title() override;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	void DrawVolumeSettings(); // 音量設定の描画
	void DrawVolumeTexts(); // 音量設定のテキストを描画
	void RenderGameScene(); // ゲーム画面に行くまでの描画



	// 次のシーンを取得する関数
	enum NextScene
	{
		NONE_SCENE,
		SELECT_SCENE,
		EXPLAIN_SCENE,
	};

	NextScene mNextScene;

private:
	// Update処理
	void UpdateButtonAnimation(); // ボタンの移動アニメーションを更新
	void UpdateLogoAnimation(); // ロゴの上下アニメーションを更新
	void UpdateButtonInput(); // ボタンのクリック・入力処理を更新
	void UpdateSceneTransition(); // シーン遷移のアニメーションを更新
	void UpdateWhiteBox(); // シーン遷移時の白いBOXの透過処理を更新
	void UpdateVolumeSetting(); // 音量設定の入力処理を更新

private:
	// スマートポインタ
	// ボタン関係
	std::unique_ptr<MouseGraph> mpGameStart;
	std::unique_ptr<MouseGraph> mpExplainGraph;
	std::unique_ptr<MouseGraph> mpOptionButton;
	std::unique_ptr<MouseGraph> mpMusicClose;

	// 音量バー関係
	std::unique_ptr<VolumeBar> mpBgmVolumeBar;
	std::unique_ptr<VolumeBar> mpSeVolumeBar;

	// 音量つまみ
	VolumeSetting mVolumeSet;


	// 画像ハンドル
	int mnRogoHandle;
	int mnBagHandle;
	int mnCardHandle;

	// 菊池
	int mnKorukuitaHandle;
	int mnBatuHandle;

	// 音量設定を開いているか
	bool mbOption;

	// タイトル演出
	bool mbMouseButton;
	bool mbWhite;

	// カード演出
	float mnCardX;
	float mnCardY;
	float mnCardAngle;
	float mnCardRota;

	// カードのターゲット位置 小池
	float targetX;
	float targetY;
	float targetAngle;
	float targetRota;

	// 白いBOXの透明度
	float mfWhiteBoxAlpha;

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
	bool mbInitialize;
	float mfLogoTime;
};