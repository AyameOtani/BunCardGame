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