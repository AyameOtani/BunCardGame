#pragma once

// タイトル画面のシーン遷移演出を管理するクラス
// 黒板画像の移動回転と画面白飛び演出

class TitleEffectAnimation
{
public:
	TitleEffectAnimation();
	~TitleEffectAnimation();

	void Initialize();
	void Update();
	void Draw() const;


	// アクセサ関係
	void SetCardHandle(int blackboardHandle) { mnBlackboardHandle = blackboardHandle; }
	void StartTransition() { mbIsMouseButton = true; }  // シーン遷移演出を開始

	// 白いBOXの演出中か取得
	bool GetIsWhite() const { return mbIsWhite; }

	// マウスボタンが押されてシーン遷移演出中か取得
	bool GetIsMouseButton() const { return mbIsMouseButton; }


	float GetWhiteBoxAlpha() const { return mfWhiteBoxAlpha; }

private:
	void UpdateSceneTransition();
	void UpdateWhiteBox(); // 白いBOXの透明度を更新


private:
	// 黒板画像関係
	int mnBlackboardHandle;
	bool mbIsMouseButton; // シーン遷移演出が開始されているか
	bool mbIsWhite; // 白いBOXの演出を開始する状態か

	// 黒板画像の今の位置
	float mfBlackboardPositionX;
	float mfBlackboardPositionY;
	float mfBlackboardAngle;
	float mfBlackboardRotation;

	// 黒板の目標値
	float mfTargetPositionX;
	float mfTargetPositionY;
	float mfTargetAngle;
	float mfTargetRotation;

	// 白いBOXの透明度
	float mfWhiteBoxAlpha;
};