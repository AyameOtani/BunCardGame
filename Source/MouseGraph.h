#pragma once
#include "DxLib.h"
#include <string>

// マウスと重なっていたら大きくするやつの画像用
class MouseGraph
{
public:

	// コンストラクタ
	MouseGraph(
		float x,			// Xの位置　中心基準
		float y,			// Yの位置　中心基準
		float angle,		// 角度
		std::string filename,// 画像ハンドル
		float rate,	    // 拡大率
		float changerate  // 変えた後の拡大率
	);

	~MouseGraph();

	void Draw(); //描画
	void Update(); // 判定
	bool IsClicked(); // マウスが乗っててかつ押されたらの返すやつ
	bool GetIsHover() { return isHover; } // マウスが乗っているかを取得

	// ターンエンドボタンが押せる状態か プレイヤーのターンかのやつ
	void SetActive(bool active) { isActive = active; }

	void SetPosition(float x, float y);

	void SetAngle(float angle);

private:

	// XとY
	float mx;
	float my;
	// 角度
	float mAngle;
	// ハンドル
	int mnHandle;
	// 拡大率
	float mRate;
	float mChangeRate;


	// 今マウスが乗っているかのフラグ
	bool isHover;
	bool isActive; // ボタンが有効か
};