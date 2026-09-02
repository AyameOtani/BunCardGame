#include "SelectScene.h"
#include "Utility.h"
#include "Master.h"
#include "GameScene.h"

SelectScene::SelectScene()
	: mpEasy(nullptr)
	, mpNormal(nullptr)
	, mpHard(nullptr)
	, mnEasyHandle(-1)
	, mnNormalHandle(-1)
	, mnHardHandle(-1)
	, mnEasy_NoSelectHandle(-1)
	, mnNormal_NoSelectHandle(-1)
	, mnHard_NoSelectHandle(-1)
	, mnBagHandle(-1)
	, mbSceneChange(false)
	, mfFadeAlpha(0.0f)   
	, mnEasyBagHandle(-1)
	, mnNormalBagHandle(-1)
	, mnHardBagHandle(-1)
{

	if (mnBagHandle == -1) mnBagHandle = LoadGraph("Resource/2D/easy_bag.png");
	mnEasyHandle = -1;
	mnNormalHandle = -1;
	mnHardHandle = -1;
	mnEasyBagHandle = -1;
	mnNormalBagHandle = -1;
	mnHardBagHandle = -1;

	mNextScene = SceneManager::GScene::easy;// デフォルトはeasyにしておく
}


SelectScene::~SelectScene()
{

}

void SelectScene::Initialize()
{
	if (mnBagHandle == -1) mnBagHandle = LoadGraph("Resource/2D/easy_bag.png");
	int x = Utility::SCREEN_WIDTH / 2;
	int y = Utility::SCREEN_HEIGHT / 2 + 200;
	// 中心座標XとY　角度　画像　画像の拡大率　変えるときの拡大率
	mpEasy = new MouseGraph((float)x - 500.0f, (float)y, 0.0f, "Resource/Enemy/easy.png", 0.20f, 0.23f); // スタートボタン
	mpNormal = new MouseGraph((float)x, (float)y, 0.0f, "Resource/Enemy/normal.png", 0.20f, 0.23f); // ノーマル　シルエット
	mpHard = new MouseGraph((float)x + 500.0f, (float)y, 0.0f, "Resource/Enemy/hard.png", 0.20f, 0.23f); // ハード　シルエット

	if (mnEasyHandle == -1) mnEasyHandle = LoadGraph("Resource/Enemy/Enemyeasy.png");
	if (mnNormalHandle == -1) mnNormalHandle = LoadGraph("Resource/Enemy/Enemynormal.png");
	if (mnHardHandle == -1) mnHardHandle = LoadGraph("Resource/Enemy/Enemyhard.png");
	if (mnEasy_NoSelectHandle == -1) mnEasy_NoSelectHandle = LoadGraph("Resource/Enemy/Enemyeasy_NoSelect.png");
	if (mnNormal_NoSelectHandle == -1) mnNormal_NoSelectHandle = LoadGraph("Resource/Enemy/Enemynormal_NoSelect.png");
	if (mnHard_NoSelectHandle == -1) mnHard_NoSelectHandle = LoadGraph("Resource/Enemy/Enemyhard_NoSelect.png");
	
	//難易度ごとの背景   片野
	if (mnEasyBagHandle == -1) mnEasyBagHandle = LoadGraph("Resource/2D/easy_bag.png");
	if (mnNormalBagHandle == -1) mnNormalBagHandle = LoadGraph("Resource/2D/Normal_bag.png");
	if (mnHardBagHandle == -1) mnHardBagHandle = LoadGraph("Resource/2D/hard_bag.png");

	int SiseXbuf, SiseYBuf;
	GetGraphSize(mnEasyBagHandle, &SiseXbuf, &SiseYBuf);
	GetGraphSize(mnNormalBagHandle, &SiseXbuf, &SiseYBuf);
	GetGraphSize(mnHardBagHandle, &SiseXbuf, &SiseYBuf);

}

void SelectScene::Update()
{

	if (mbSceneChange)
	{
		mfFadeAlpha += 6.0f; // フェード速度

		Master::mpSoundManager->StartFadeOut();
		if (mfFadeAlpha >= 254.0f)
		{
			mfFadeAlpha = 254.0f;
			// ここでシーン変える
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::GAME_SCENE,mNextScene);
		}
	}

	if (!mbSceneChange)
	{
		// ゲームボタンのUpdateと判定
		if (mpEasy)
		{
			mpEasy->Update();

			if (mpEasy->IsClicked()) // 押されたらゲーム画面にいく
			{
				mNextScene = SceneManager::GScene::easy;
				Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
				mbSceneChange = true;
			}
		}
		if (mpNormal)
		{
			mpNormal->Update();

			if (mpNormal->IsClicked())
			{
				mNextScene = SceneManager::GScene::normal;
				Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
				mbSceneChange = true;

			}
		}
		if (mpHard)
		{
			mpHard->Update();

			if (mpHard->IsClicked())
			{
				mNextScene = SceneManager::GScene::hard;
				Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
				mbSceneChange = true;

			}
		}
	}
	



	Scene::Update();
}

void SelectScene::Draw()
{
	DrawGraph(0, 0, mnBagHandle, TRUE);


	int x = Utility::SCREEN_WIDTH / 2 - 500;
	int y = Utility::SCREEN_HEIGHT / 2 - 100;

	if (mpEasy)
	{
		mpEasy->Draw(); //ボタンの描画

		//難易度ごとの背景   片野
		DrawExtendGraph(x - 150, y, x + 150, y + 150, mnEasyBagHandle, TRUE);

		if (mpEasy->GetIsHover())
		{
			DrawExtendGraph(x - 100, y, x + 100, y + 200, mnEasyHandle, TRUE);
		}
		else
		{
			DrawExtendGraph(x - 100, y, x + 100, y + 200, mnEasy_NoSelectHandle, TRUE);
		}

	}
	if (mpNormal)
	{
		mpNormal->Draw(); //ボタンの描画

		DrawExtendGraph(x + 350, y, x + 650, y + 150, mnNormalBagHandle, TRUE);

		if (mpNormal->GetIsHover())
		{
			DrawExtendGraph(x + 400, y, x + 600, y + 200, mnNormalHandle, TRUE);
		}
		else
		{
			DrawExtendGraph(x + 400, y, x + 600, y + 200, mnNormal_NoSelectHandle, TRUE);
		}

	}
	if (mpHard)
	{
		mpHard->Draw(); //ボタンの描画

		DrawExtendGraph(x + 850, y, x + 1150, y + 150, mnHardBagHandle, TRUE);

		if (mpHard->GetIsHover())
		{
			DrawExtendGraph(x + 900, y, x + 1100, y + 200, mnHardHandle, TRUE);
		}
		else
		{
			DrawExtendGraph(x + 900, y, x + 1100, y + 200, mnHard_NoSelectHandle, TRUE);
		}

	}

	// 選択の文字
	DrawStringToHandle(
		Utility::SCREEN_WIDTH / 2 - 400,
		Utility::SCREEN_HEIGHT / 2 - 200,
		"ステージを選択しよう",
		GetColor(255, 255, 255),
		Master::mpFontManager->GetSelectFontHandle()
	);

	// シーン移動したら白くフェードのやつ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mfFadeAlpha);
	DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, GetColor(255, 255, 255), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	Scene::Draw();
}

void SelectScene::Finalize()
{
	if (mnBagHandle != -1)
	{
		DeleteGraph(mnBagHandle);
		mnBagHandle = -1;
	}
	if (mnEasyHandle != -1)
	{
		DeleteGraph(mnEasyHandle);
		mnEasyHandle = -1;
	}
	if (mnNormalHandle != -1)
	{
		DeleteGraph(mnNormalHandle);
		mnNormalHandle = -1;
	}
	if (mnHardHandle != -1)
	{
		DeleteGraph(mnHardHandle);
		mnHardHandle = -1;
	}

	if (mnEasy_NoSelectHandle != -1)
	{
		DeleteGraph(mnEasy_NoSelectHandle);
		mnEasy_NoSelectHandle = -1;
	}
	if (mnNormal_NoSelectHandle != -1)
	{
		DeleteGraph(mnNormal_NoSelectHandle);
		mnNormal_NoSelectHandle = -1;
	}
	if (mnHard_NoSelectHandle != -1)
	{
		DeleteGraph(mnHard_NoSelectHandle);
		mnHard_NoSelectHandle = -1;
	}

	if (mnEasyBagHandle != -1)
	{
		DeleteGraph(mnEasyBagHandle);
		mnEasyBagHandle = -1;
	}

	if (mnNormalBagHandle != -1)
	{
		DeleteGraph(mnNormalBagHandle);
		mnNormalBagHandle = -1;
	}

	if (mnHardBagHandle != -1)
	{
		DeleteGraph(mnHardBagHandle);
		mnHardBagHandle = -1;
	}

	/*if (mnSelectHandle != -1)
	{
		DeleteGraph(mnSelectHandle);
		mnSelectHandle = -1;
	}*/

	if (mpEasy)
	{
		delete mpEasy;
		mpEasy = nullptr;
	}
	if (mpNormal)
	{
		delete mpNormal;
		mpNormal = nullptr;
	}
	if (mpHard)
	{
		delete mpHard;
		mpHard = nullptr;
	}

}