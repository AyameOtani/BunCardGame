#pragma once

#include "Scene.h"
#include "DxLib.h"
#include "Button.h"
#include "MouseGraph.h"
#include "Utility.h"

class Title : public Scene
{

public:
	Title();
	~Title() override;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	// 次にどっちの画面にいくかのしゅるい
	enum NextScene
	{
		NONE_SCENE,

		SELECT_SCENE,
		EXPLAIN_SCENE,
	};

	NextScene mNextScene = NONE_SCENE; // 次のシーンの種類を保存する変数

private:
	MouseGraph* mpGameStart = nullptr; // マウスと重なって大きくなるやつ
	MouseGraph* mpExplainGraph = nullptr; // 操作説明のやつ
	MouseGraph* mpGearButtom = nullptr; // 音量調整のやつ
	MouseGraph* mpMusicClose = nullptr; // 音量設定の×ボタン

	int mnRogoHandle = -1; // ロゴのはんどる
	int mnBagHandle = -1;  // タイトルの背景
	int mnCardHandle = -1; // カードの画像
	int mnMusicBag = -1; // 音量設定の背景

	// 菊池
	int mnKorukuitaHandle = -1; // コルク板の画像
	int mnBatuHandle = -1; // ×ボタンの画像
	int mnOnpuHandle = -1; // 音符の画像


	// 音量調整のやつ
	bool mbOption = false;
	int mBgmVolume = 100;
	int mSeVolume = 150;

	// 音量調整のバーの位置とか
	int mBarX = 570;
	int mBarWidth = 840;
	int mBarHeight = 40; // バーの高さ
	int mBarString = 70; // バーの上に表示する数字
	int mMusicNote = 15; // 音符の位置
	int mnBarDist = 20; // バーの端も押しやすいように

	// 音量調整のバーのY座標
	int mBgmBarY = 460;
	int mSeBarY = 650;

	// タイトル演出
	bool mbMouseButton = false; // マウスが押されたかのフラグ
	bool mbWhite = false; // 白いBOXを出すかのフラグ

	// タイトル演出のカードの座標とか拡大率とか
	float mnCardX = 0.0f;
	float mnCardY = 0.0f;
	float mnCardAngle = 3.0f;
	float mnCardRota = 0.01f;
	float mfCardSpeed = 0.09f; // 移動する速さ

	// ターゲットの位置
	float targetX = Utility::SCREEN_WIDTH / 2.0f;
	float targetY = Utility::SCREEN_HEIGHT / 2.0f - 30.0f;
	float targetAngle = -0.05f;
	float targetRota = 1.0f;

	// 白いBOXの透明度
	float mfWhiteBoxAlpha = 0.0f;

	// ボタン演出用
	float mfStartX;
	float mfStartY;
	float mfExplainX;
	float mfExplainY;

	// いけちゃ
	float mfTargetStartX;
	float mfTargetStartY;
	float mfTargetExplainX;
	float mfTargetExplainY;

	// ロゴ演出
	//float mfLogoY;
	//float mfTargetLogoY;
	

	// タイトルのロゴ演出   大谷
	float mfTurnY = -800.0f;  // ロゴの初期位置
	bool mbInitialize = false; // 初期化が終わったかどうかのフラグ
	float mfLogoTime = 0.0f; // ロゴのふわふわ時間


	// 音量調節の当たり判定
	int mnBarUp = 40;	//バーの上の判定
	int mnBarDown = 65;	// バーの下の判定
};
