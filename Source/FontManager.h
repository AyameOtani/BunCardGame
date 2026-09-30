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

	// まだ存在しなければ新しく作成する
	int GetNikumaruFontHandle(int size);
	// にくまるフォントを作成する
	int CreateNikumaruFont(int size);


	// まだ存在しなければ新しく作成する
	int GetHutehodoFontHandle(int size);
	// ふてほど丸ゴシックフォントを作成する
	int CreateHutehodoFont(int size);


	// 指定のフォントハンドルを返す
	int ApplyFontType(int _size, FontType _type);



private:
	FontType mType;

private:
	// key   : フォントサイズ
	// value : フォントハンドル
	std::map<int, int> mNikumaruFontHandles;
	std::map<int, int> mHutehodoFontHandles;

};

