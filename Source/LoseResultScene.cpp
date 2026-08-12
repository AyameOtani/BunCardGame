#include "LoseResultScene.h"
#include "InputManager.h"
#include "Master.h"
#include "Utility.h"

LoseResultScene::LoseResultScene()
{
	if (mnBagHandle == -1) mnBagHandle = LoadGraph("Resource/Title/Title.png");
	Master::mpSoundManager->PlaySE(SoundManager::SE_LOSS);
}

LoseResultScene::~LoseResultScene()
{
	DeleteGraph(mnBagHandle);
}

void LoseResultScene::Initialize()
{
	int x = Utility::SCREEN_WIDTH / 2;
	int y = Utility::SCREEN_HEIGHT / 2;
	// 中心座標XとY　角度　画像　画像の拡大率　変えるときの拡大率
	mpMouseGraph = new MouseGraph((float)x, (float)y + 430.0f, 0.0f, "Resource/Title/Result.png", 0.38f, 0.43f); // スタートボタン

	m_Score.Initialize(); // スコアの初期化
}

void LoseResultScene::Update()
{

	// ゲームボタンのUpdateと判定
	if (mpMouseGraph)
	{
		mpMouseGraph->Update();

		if (mpMouseGraph->IsClicked()) // 押されたらゲーム画面にいく
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::TITLE);
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
		}
	}

	m_Score.Update(); // スコアの更新

	Scene::Update();
}

void LoseResultScene::Draw()
{
	DrawGraph(0, 0, mnBagHandle, TRUE); // 背景の描画

	if (mpMouseGraph)
	{
		mpMouseGraph->Draw(); //ボタンの描画
	}

	m_Score.Draw(); // スコアの描画

	unsigned int color = GetColor(60, 60, 60); // 黒色

	// フォントを変えたよ　大谷
	DrawStringToHandle(
		800,
		(int)m_Score.GetMoveY() - 300,
		"敗北",
		color,
		Master::mpFontManager->GetResultFontHandle()
	);
	Scene::Draw();
}

void LoseResultScene::Finalize()
{
	if (mpMouseGraph)
	{
		delete mpMouseGraph;
	}
}




