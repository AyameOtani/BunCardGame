#pragma once

#include "DxLib.h"
#include <map>

// フォントを制御するやつ
// リザルドとかで使う予定
class FontManager
{
public:
	FontManager();
	~FontManager();
	void Initialize();
	void Finalize();

	enum class FontType
	{
		None,
		Nikumaru,
		Hutehodo,
	};
	

	// ドットフォントで文字を描画する
	// fontHandle : 指定のフォント
	// size : フォントサイズ
	// color : 文字色
	// format : printfと同じように指定
	void FontManagerDrawString(
		 FontType type,
		int x,
		int y,
		int size,
		unsigned int color,
		const char* format,
		...
	);


	//Master::mpGameManager->GetFontManager()->DrawDotString(
	//	500,
	//	500,
	//	100,
	//	GetColor(255, 255, 255),
	//	"タイトル　才能の原石\n\n Enterでゲームシーン"
	//);

	// 指定されたサイズのにくまるフォントハンドルを取得する
	// まだ存在しなければ新しく作成する
	int GetNikumaruFontHandle(int size);
	// にくまるフォントを作成する
	int CreateNikumaruFont(int size);



	// 指定されたサイズのふてほど丸ゴシックフォントハンドルを取得する
	// まだ存在しなければ新しく作成する
	int GetHutehodoFontHandle(int size);
	// ふてほど丸ゴシックフォントを作成する
	int CreateHutehodoFont(int size);




	
	//// アイテムのフォント  20																																																																																																																																																																																																																																																			 // -----------にくまるフォント----------------
	//// フォント取得
	//// リザルドの勝利敗北  180
	//int GetResultFontHandle() const { return mnResultFont; }

	//// スコアのフォント  45
	//int GetScoreFontHandle() const { return mnScoreFont; }

	//// ステージ選択のフォント  80
	//int GetSelectFontHandle() const { return mnSelecFont; }

	//// ステージ選択のフォント  30
	//int GetMusicFontHandle() const { return mnMusicFont; }

	//// ステージ選択のフォント  60
	//int GetTextFontHandle() const { return mnTextFont; }


	//// --------ふてほど丸ゴシック------------
	//// MPのフォント         50
	//int GetMpFontHandle() const { return mnMpFont; }

	//// アイコン  23
	//int GetStatusFontHandle() const { return mnStatusFont; }

	//// HPのフォント  16
	//int GetHpFontHandle() const { return mnHpFont; }

	//int GetItemFontHandle() const { return mnItemFont; }

private:
	FontType mType;

private:
	// key   : フォントサイズ
	// value : フォントハンドル
	std::map<int, int> mNikumaruFontHandles;
	std::map<int, int> mHutehodoFontHandles;

};

