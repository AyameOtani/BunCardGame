#include "DxLib.h"
#include "Texture.h"  // ../は一階層上にあるものを読み込むという意味
#include "TextureAnimation.h"
#include "Collision.h"
#include "SceneManager.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Utility.h"
#include "Mouse.h"




#ifdef  _DEBUG
#else // リソースモード
#endif //  _DEBUG
// ここにDrawStringとかを書くとReleaseモードにしても表示されない。


/*
 @note リファレンス https://dxlib.xsrv.jp/dxfunc.html
*/

// Master クラスの静的メンバ変数定義
SceneManager* Master::mpSceneManager = new SceneManager();
SoundManager* Master::mpSoundManager = new SoundManager();
ResourceManager* Master::mpResourceManager = new ResourceManager();
AnimationManager* Master::mpAnimationManager = new AnimationManager();
FontManager* Master::mpFontManager = new FontManager();
Mouse m_Mouse; // マウス追加


/**
* @fn WinMain
* @brief Main関数
* @param[in] HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
* @return int 0 正常終了／-1 エラー
* @details Main関数
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	// ウインドウモードで起動  falseにすると全画面
	ChangeWindowMode(true);
	// 背景色の設定
	//SetBackgroundColor(255, 233, 214);
	SetBackgroundColor(50, 50, 50);
	// サイズを合わせる
	SetGraphMode(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, 0);  // モニター解像度に合わせる


	// DXライブラリ初期化
	if(DxLib_Init() == -1)
	{
		return -1;
	}

	// アニメーションの初期化
	Master::mpAnimationManager->Initialize();
	// フォントの初期化
	Master::mpFontManager->Initialize();

	// サウンドマネージャーの初期化 （シーンより先にやらないと流れないので注意）
	Master::mpSoundManager->Initialize();   // 全てのサウンドが読み込まれる（BGMやS
	// シーンマネージャーの生成と初期化
	Master::mpSceneManager->Initialize();


	// 描画先画面を裏画面に設定する
	SetDrawScreen(DX_SCREEN_BACK);


	// Texture クラスを使って描画
	VECTOR position {};
	position.x = static_cast<float>(640) / 2;
	position.y = static_cast<float>(480) / 2;
	
	// Zバッファに書き込む準備
	// ここで奥行がちゃんとなる
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		//画面を初期化する
		ClearDrawScreen();

		int time = GetNowCount();

		// 菊池
		// サウンドの更新
		Master::mpSoundManager->Update();


		// 更新
		Master::mpSceneManager->Update();

		// マウスの更新
		m_Mouse.Update();


		// 描画
		Master::mpSceneManager->Draw();



		// スクショのやつ
		if (InputManager::CheckDownKey(KEY_INPUT_F1))
		{
			SaveDrawScreenToJPEG(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT,
				"screenshot1.png");
		}


		// 裏画面の内容を表画面に映す
		ScreenFlip();



		// 17ミリ秒（秒間約60フレームだった場合の１フレーム当たりの経過時間）
		// 経過するまでここで待つ
		while (GetNowCount() - time < 17)
		{
			// 待つだけなので何も処理はしない
		}


		// 削除する必要のあるオブジェクトがあれば削除する
		Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();

		// ループする直前にシーン遷移チェックをいれておく
		Master::mpSceneManager->ChangeSceneIfNeeded();


	}


	// 終了処理
	Master::mpSceneManager->Finalize();
	//★
	delete Master::mpSceneManager; // いらなくなるのでdelete する

	// サウンドの終了処理
	Master::mpSoundManager->Finalize();
	delete Master::mpSoundManager;

	delete Master::mpResourceManager; // リソースマネージャーの削除 
	delete Master::mpAnimationManager;

	Master::mpFontManager->Finalize(); // フォントマネージャーの終了処理
	delete Master::mpFontManager; // フォントマネージャーの削除

	//★
	//delete pAnimation;
	
	// DXライブラリ使用の終了
	DxLib_End();

	// ソフトの終了
	return 0;
}