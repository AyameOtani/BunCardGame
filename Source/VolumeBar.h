#pragma once

// 音量調節バーを管理・描画するクラス
// SEとBGMのバーを管理して音量に応じたバーの描画を行う。
// オブジェクト指向的にするためにクラス化した。
class VolumeBar
{
private:
	// 定数定義
	static constexpr int MaxVolume = 100;		// 音量の最大値（上限）
	static constexpr int FrameColorMax = 255;	// バーの枠線の色（白）

public:
	// 音量バーを生成する
	// x, y      : 音量バーの左上座標
	// width     : 音量バーの幅
	// height    : 音量バーの高さ
	// color     : 音量バーの色
	VolumeBar(int inX, int inY, int inWidth, int inHeight, int inColor);

	// 音量に応じてバーを描画する
	void Draw(int inVolume);

	// マウスが音量バーの範囲内にあるか
	bool IsMouseOver(int inMouseX, int inMouseY) const;

	// マウスのX座標から音量を計算する
	int GetVolumeFromMouse(int inMouseX) const;

private:
	int mnX;	
	int mnY;	
	int mnWidth;
	int mnHeight;
	int mnColor;
};