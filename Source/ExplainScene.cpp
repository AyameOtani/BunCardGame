#include "InputManager.h"
#include "Master.h"
#include "ExplainScene.h"
#include "Utility.h"

ExplainScene::ExplainScene()
{
    mpGameStart = nullptr;
    mpBackTitle = nullptr;
    mnItemEHandle = -1;
    mnBagHandle = -1;
}

ExplainScene::~ExplainScene()
{
}

void ExplainScene::Initialize()
{
    int x = Utility::SCREEN_WIDTH / 2;
    int y = Utility::SCREEN_HEIGHT / 2;

    if (mnItemEHandle == -1)
    {
        // 背景
        mnItemEHandle = LoadGraph("Resource/Operation/manual.png");
    }

    if (mnBagHandle == -1)
    {
		mnBagHandle = LoadGraph("Resource/Title/Title.png");
    }

    // ボタン（Titleと同じ作り）
    mpGameStart = new MouseGraph((float)x + 600.0f, (float)y + 420.0f, 0.0f,"Resource/Title/Start.png",0.4f, 0.45f); // ゲームへ

    mpBackTitle = new MouseGraph((float)x - 600.0f, (float)y + 420.0f, 0.0f,"Resource/Title/Result.png",0.4f, 0.45f); // タイトルへ

    // タイトルからつなげる
    //Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE, true, 255);

}

void ExplainScene::Update()
{
    // Update
    if (mpGameStart) mpGameStart->Update();
    if (mpBackTitle) mpBackTitle->Update();

    //フェードでかえるやつ
    if (mbSelectScene)
    {
        mfFadeAlpha += 10; // フェード速度

        Master::mpSoundManager->StartFadeOut();
        if (mfFadeAlpha >= 254)
        {
            mfFadeAlpha = 254;
            // ここでシーン変える
            Master::mpSceneManager->SetNextScene(SceneManager::SELECT_SCENE);
        }
    }

    if (mbTitleScene)
    {
        mfFadeAlpha += 10; // フェード速度

        Master::mpSoundManager->StartFadeOut();
        if (mfFadeAlpha >= 254)
        {
            mfFadeAlpha = 254;
            // ここでシーン変える
            Master::mpSceneManager->SetNextScene(SceneManager::TITLE);
        }
    }


    // フェード中はクリックさせないように
    if (!mbSelectScene && !mbTitleScene)
    {
        // クリック判定
        if (mpGameStart->IsClicked())
        {
            mbSelectScene = true; // 選択画面にいくやつ
            Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
        }

        if (mpBackTitle->IsClicked())
        {
            mbTitleScene = true; // タイトル画面にいくやつ
            Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
        }
    }
  

    Scene::Update();
}

void ExplainScene::Draw()
{
    int x = Utility::SCREEN_WIDTH / 2;
    int y = Utility::SCREEN_HEIGHT / 2;

    SetUseZBufferFlag(FALSE);
    SetWriteZBufferFlag(FALSE);

    if (mnBagHandle != -1)
    {
        DrawGraph(0, 0, mnBagHandle, TRUE);
    }

    // 背景
    if (mnItemEHandle != -1)
    {
        //DrawGraph(0, 0, mnItemEHandle, TRUE);
        DrawRotaGraph(x, y -46, 0.82f, 0.0f, mnItemEHandle, TRUE);
    }

    // ボタン
    if (mpGameStart) mpGameStart->Draw();
    if (mpBackTitle) mpBackTitle->Draw();


    // シーン移動したら白くフェードのやつ
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mfFadeAlpha);
    DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, GetColor(255, 255, 255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



    Scene::Draw();
}

void ExplainScene::Finalize()
{
    if (mnItemEHandle != -1)
    {
        DeleteGraph(mnItemEHandle);
    }

    if (mpGameStart)
    {
        delete mpGameStart;
        mpGameStart = nullptr;
    }

    if (mpBackTitle)
    {
        delete mpBackTitle;
        mpBackTitle = nullptr;
    }
}


//#include "InputManager.h"
//#include "Master.h"
//#include "ExplainScene.h"
//#include "Mouse.h" 
//
//ExplainScene::ExplainScene()
//{
//
//}
//
//ExplainScene::~ExplainScene()
//{
//
//}
//
//
//void ExplainScene::Initialize()
//{
//	mnItemEHandle = LoadGraph( "Resource/Mouse/Ce.png" ); // アイテム一覧画像
//}
//
//
//void ExplainScene::Update()
//{
//	//マウスがクリックされたらゲームシーンに行く
//	if ( Mouse::IsTrigger() )
//	{
//		Master::mpSceneManager->SetNextScene( SceneManager::GAME_SCENE );
//		Master::mpSoundManager->PlaySE( SoundManager::SE_DECIDE );
//	}
//	Scene::Update();
//}
//
//void ExplainScene::Draw()
//{
//	DrawGraph( 0 , 0 , mnItemEHandle , TRUE ); // 操作説明の描画
//
//	Scene::Draw();
//}
//
//void ExplainScene::Finalize()
//{
//	DeleteGraph( mnItemEHandle );
//}