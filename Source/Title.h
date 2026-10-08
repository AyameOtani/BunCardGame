#pragma once
#include "Scene.h"
#include "DxLib.h"
#include "Button.h"
#include "MouseGraph.h"
#include "Utility.h"
#include "TitleEffectAnimation.h"
#include "TitleOption.h"

// タイトル画面のクラス
// 音量関係や設定関係を別のクラスに移動したのですっきりした
class Title : public Scene
{
public:
	Title();
	~Title() override;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

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

private:
	// スマートポインタ
	// ボタン関係
	std::unique_ptr<MouseGraph> mpGameStart;
	std::unique_ptr<MouseGraph> mpExplainGraph;
	std::unique_ptr<MouseGraph> mpOptionButton;

	// カード・シーン遷移演出
	std::unique_ptr<TitleEffectAnimation> mpTitleEffectAnimation;
	// 音量設定
	std::unique_ptr<TitleOption> mpTitleOption;


	// 画像ハンドル
	int mnRogoHandle;
	int mnBagHandle;
	int mnCardHandle;

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