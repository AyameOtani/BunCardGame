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
    // フォルダにあるフォントをダウンロード
    // にくまるフォント
    AddFontResourceEx(
		"Resource/Font/NikumaruFont/Nikumaru.otf", // フォントのパス
        FR_PRIVATE, // このプログラムだけで使うってやつらしい
        NULL
    );


    // フォルダにあるフォントをダウンロード
    // でらまるフォント
    AddFontResourceEx(
        "Resource/Font/Futehodo/Futehodo-MaruGothic.ttf", // フォントのパス
        FR_PRIVATE, // このプログラムだけで使うってやつらしい
        NULL
    );



    // リザルド用のフォントの作成
    {
        mnResultFont = CreateFontToHandle(
            "07にくまるフォント",
            180,
            5
        );
        if (mnResultFont == -1)
        {
            printfDx("フォントの読み込みに失敗");
            // 予備でフォント作る
            mnResultFont = CreateFontToHandle(
                NULL,
                160,
                5
            );
        }
    }

    // スコア用のフォントの作成
    {
        mnScoreFont = CreateFontToHandle(
            "07にくまるフォント",
            45,
            5
        );

        if (mnScoreFont == -1)
        {
            printfDx("フォントの読み込みに失敗");
            // 予備でフォント作る
            mnScoreFont = CreateFontToHandle(
                NULL,
                45,
                5
            );
        }
    }

    // 選択画面用のフォントの作成
    {
        mnSelecFont = CreateFontToHandle(
            "07にくまるフォント",
            80,
            5
        );

        if (mnSelecFont == -1)
        {
            printfDx("フォントの読み込みに失敗");
            // 予備でフォント作る
            mnSelecFont = CreateFontToHandle(
                NULL,
                80,
                5
            );
        }
    }

   

    // 音量設定用のフォントの作成
    {
        mnMusicFont = CreateFontToHandle(
            "07にくまるフォント",
            30,
            5
        );

        if (mnMusicFont == -1)
        {
            printfDx("フォントの読み込みに失敗");
            // 予備でフォント作る
            mnMusicFont = CreateFontToHandle(
                NULL,
                30,
                5
            );
        }
    }

   

    // テキスト用のフォントの作成
    {
        mnTextFont = CreateFontToHandle(
            "07にくまるフォント",
            60,
            5
        );

        if (mnTextFont == -1)
        {
            printfDx("フォントの読み込みに失敗");
            // 予備でフォント作る
            mnTextFont = CreateFontToHandle(
                NULL,
                60,
                5
            );
        }
    }


    //ふてほど丸ゴシック
    // MPの文字
    {
        mnMpFont = CreateFontToHandle(
            "ふてほど丸ゴシック",
            50,
            5
        );
        if (mnMpFont == -1)
        {
            printfDx("フォントの読み込みに失敗");
            // 予備でフォント作る
            mnMpFont = CreateFontToHandle(
                NULL,
                50,
                5
            );
        }
    }


    //アイコンの文字サイズ
    {
        mnStatusFont = CreateFontToHandle(
            "ふてほど丸ゴシック",
            20,
            5
        );
        if (mnStatusFont == -1)
        {
            printfDx("フォントの読み込みに失敗");
            // 予備でフォント作る
            mnStatusFont = CreateFontToHandle(
                NULL,
                45,
                5
            );
        }
    }

    //HPの文字サイズ
    {
        mnHpFont = CreateFontToHandle(
            "ふてほど丸ゴシック",
            16,
            5
        );
        if (mnHpFont == -1)
        {
            printfDx("フォントの読み込みに失敗");
            // 予備でフォント作る
            mnHpFont = CreateFontToHandle(
                NULL,
                25,
                5
            );
        }
    }

    //アイテムの文字サイズ
    {
        mnItemFont = CreateFontToHandle(
            "ふてほど丸ゴシック",
            20,
            5
        );
        if (mnItemFont == -1)
        {
            printfDx("フォントの読み込みに失敗");
            // 予備でフォント作る
            mnItemFont = CreateFontToHandle(
                NULL,
                25,
                5
            );
        }
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
        "Nikumaru",
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
        "Futehodo-MaruGothic",
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
    int fontHandle{};
    if (type == FontType::Nikumaru)
    {
        fontHandle = GetNikumaruFontHandle(size);
    }
    else
    {
        fontHandle = GetHutehodoFontHandle(size);
    }

    if (fontHandle == -1)
    {
        return;
    }

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