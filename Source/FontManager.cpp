#include "FontManager.h"

FontManager::FontManager()
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

void FontManager::Finalize()
{
    DeleteFontToHandle(mnResultFont);
    DeleteFontToHandle(mnScoreFont);
    DeleteFontToHandle(mnSelecFont);
    DeleteFontToHandle(mnMusicFont);
    DeleteFontToHandle(mnTextFont);
    DeleteFontToHandle(mnStatusFont);
    DeleteFontToHandle(mnMpFont);
    DeleteFontToHandle(mnHpFont);
}