#include "FontManager.h"

#include <DxLib.h>
#include <cstdarg>
#include <cstdio>

FontManager::FontManager()
    :mType(FontType::None)
{

}

FontManager::~FontManager()
{

}

void FontManager::Initialize()
{
    // にくまるフォントを登録
    int nikumaruResult = AddFontResourceEx(
        "Resource/Font/NikumaruFont/Nikumaru.otf",
        FR_PRIVATE,
        NULL
    );

    if (nikumaruResult == 0)
    {
        printfDx("にくまるフォントの登録に失敗しました。\n");
    }


    // ふてほどフォントを登録
    int hutehodoResult = AddFontResourceEx(
        "Resource/Font/Futehodo/Futehodo-MaruGothic.ttf",
        FR_PRIVATE,
        NULL
    );

    if (hutehodoResult == 0)
    {
        printfDx("ふてほどフォントの登録に失敗しました。\n");
    }
}
int FontManager::GetNikumaruFontHandle(int size)
{
    // すでに同じサイズのフォントが作られているか確認
    auto it = mNikumaruFontHandles.find(size);

    if (it != mNikumaruFontHandles.end())
    {
        // すでに存在するので、そのハンドルを返す
        return it->second;
    }
    // まだ存在しないので新しく作成する
    int handle = CreateNikumaruFont(size);

    if (handle == -1)
    {
        return -1;
    }
    // サイズとフォントハンドルを保存
    mNikumaruFontHandles[size] = handle;
    return handle;
}

int FontManager::CreateNikumaruFont(int size)
{
    int handle = CreateFontToHandle(
        "07にくまるフォント",
        size,
        5
    );
    // フォントの作成に失敗した場合
    if (handle == -1)
    {
        printfDx("にくまるフォントの作成に失敗しました。\n");

        // 標準フォントで代替
        handle = CreateFontToHandle(
            NULL,
            size,
            5
        );
    }
    return handle;
}



int FontManager::GetHutehodoFontHandle(int size)
{
    // すでに同じサイズのフォントが作られているか確認
    auto it = mHutehodoFontHandles.find(size);

    if (it != mHutehodoFontHandles.end())
    {
        // すでに存在するので、そのハンドルを返す
        return it->second;
    }
    // まだ存在しないので新しく作成する
    int handle = CreateHutehodoFont(size);

    if (handle == -1)
    {
        return -1;
    }
    // サイズとフォントハンドルを保存
    mHutehodoFontHandles[size] = handle;
    return handle;
}


int FontManager::CreateHutehodoFont(int size)
{
    int handle = CreateFontToHandle(
        "ふてほど丸ゴシック",
        size,
        5
    );
    // フォントの作成に失敗した場合
    if (handle == -1)
    {
        printfDx("ふてほど丸ゴシックフォントの作成に失敗しました。\n");

        // 標準フォントで代替
        handle = CreateFontToHandle(
            NULL,
            size,
            5
        );
    }
    return handle;
}

// フォントタイプを設定する
int FontManager::ApplyFontType(int _size, FontType _type)
{
    if (_type == FontType::Nikumaru)
    {
       return GetNikumaruFontHandle(_size);
    }
    else
    {
        return GetHutehodoFontHandle(_size);
    }
}



void FontManager::FontManagerDrawString(
    FontType type,
    int x,
    int y,
    int size,
    unsigned int color,
    const char* format,
    ...
)
{
    // 指定されたサイズのフォントを取得
    int fontHandle = ApplyFontType(size, type);
    if (fontHandle == -1) { return; }

    // printf形式で文字列を作成する
    char buffer[1024];

    va_list args;
    va_start(args, format);

    vsnprintf(
        buffer,
        sizeof(buffer),
        format,
        args
    );

    va_end(args);

    // 作成した文字列を描画
    DrawStringToHandle(
        x,
        y,
        buffer,
        color,
        fontHandle
    );
}



void FontManager::Finalize()
{
    // 作成したフォントをすべて削除する
    for (auto& font : mNikumaruFontHandles)
    {
        DeleteFontToHandle(font.second);
    }
    mNikumaruFontHandles.clear();


    // 作成したフォントをすべて削除する
    for (auto& font : mHutehodoFontHandles)
    {
        DeleteFontToHandle(font.second);
    }
    mHutehodoFontHandles.clear();
}