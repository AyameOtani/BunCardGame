#pragma once

#include "DxLib.h"

// フォントを制御するやつ
// リザルドとかで使う予定
class FontManager
{
public:
	FontManager();
	~FontManager();
	void Initialize();
	void Finalize();


	// -----------にくまるフォント----------------
	// フォント取得
	// リザルドの勝利敗北  180
	int GetResultFontHandle() const { return mnResultFont; }

	// スコアのフォント  45
	int GetScoreFontHandle() const { return mnScoreFont; }

	// ステージ選択のフォント  80
	int GetSelectFontHandle() const { return mnSelecFont; }

	// ステージ選択のフォント  30
	int GetMusicFontHandle() const { return mnMusicFont; }

	// ステージ選択のフォント  60
	int GetTextFontHandle() const { return mnTextFont; }


	// --------ふてほど丸ゴシック------------
	// MPのフォント         50
	int GetMpFontHandle() const { return mnMpFont; }

	// アイコン  23
	int GetStatusFontHandle() const { return mnStatusFont; }

	// HPのフォント  16
	int GetHpFontHandle() const { return mnHpFont; }

	// アイテムのフォント  20
	int GetItemFontHandle() const { return mnItemFont; }

private:
	// にくまるフォント
	int mnResultFont = -1; // リザルドのフォント
	int mnScoreFont = -1; // スコアのフォント
	int mnSelecFont = -1; // ステージ選択のフォント
	int mnMusicFont = -1; // 音量調節フォント
	int mnTextFont = -1;  // 60のフォント

	// ふてほど丸ゴシック
	int mnStatusFont = -1; // ステータスで使うフォント 25
	int mnMpFont = -1; // Mpの表示
	int mnHpFont = -1; // HPのフォント

	int mnItemFont = -1; // アイテムのフォント
};

