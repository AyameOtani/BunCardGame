#pragma once
#include "DxLib.h"
#include <string>

class Button
{
public:

	// 場所を決められるように
	Button(
		int x1, // 左上X
		int y1, // 左上Y
		int x2,	// 右下X
		int y2,	// 右下Y
		int color, // 元のボタンの色
		int changeColor, // 押したときにかえたい色
		std::string memo // 文字入れられる
	);

	void Draw();   // 表示
	void Update(); // 判定

	bool IsClicked(); // マウスが乗っててかつ押されたらの返すやつ

	// ターンエンドボタンが押せる状態か プレイヤーのターンかのやつ
	void SetActive(bool active) { isActive = active; }

private:
	// 定数定義
	static constexpr float DefaultScale = 1.0f;                       // 通常時のスケール
	static constexpr float HoverScale = 1.1f;                         // ホバーの拡大率
	static constexpr float CenterDivisor = 2.0f;                      // 中心計算用の値
	static constexpr float StringHeightOffset = 10.0f;                // 文字の縦方向の位置調整用オフセット

	// 色に関する定数
	static const int InactiveColor = 20;                              // 無効時の背景色の輝度
	static const int InactiveStringColorValue = 80;                   // 無効時の文字色の輝度
	static const int DefaultStringColorMax = 255;                     // 通常時の文字色の最大輝

	// BOXの位置
	int x1;
	int y1;
	int x2;
	int y2;

	// 今マウスが乗っているかのフラグ
	bool isHover;
	bool isActive; // ボタンが有効か

	// 重なっている時に大きく表示するやつ
	float scale;

	int color; // 色
	int changeColor; // 押したときにかえたい色

	std::string memo; // 文字保存
	int stringColor; // 文字の色
};