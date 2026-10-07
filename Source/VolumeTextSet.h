#pragma once
#include <DxLib.h>
#include "VolumeBar.h"

/// <summary>
/// ゲームシーンとタイトルシーンで記述していた音量の文字などをまとめた
/// 両方同じ処理だから引数いれてない
/// </summary>
class VolumeTextSet
{
public:
	VolumeTextSet();
	~VolumeTextSet();

	//BGMとSEのテキストを描画する関数
	void DrawBgmText(VolumeBar &bgm) const;
	void DrawSeText(VolumeBar& se) const;


	// ゲーム画面を暗くして音量画面の背景を描画する関数
	void DrawVolumeSettingPanel();

private:
	// 描画する文字サイズ
	int mnTextFontSize;
	int mnVolumeFontSize;

	int mnVolumeHandle; // 音量つまみの画像
	int mnVolumeBackground; // 音量画面の背景画像
};