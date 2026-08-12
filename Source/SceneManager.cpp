#include "SceneManager.h"
#include "Scene.h"
#include "Master.h"
#include "GameScene.h"
#include "WinResultScene.h"
#include "LoseResultScene.h"
#include "Title.h"
#include "ExplainScene.h" 
#include "SelectScene.h"

SceneManager::SceneManager()
	: mnSceneType(SCENE_TYPE::SCENE_NONE)
	, mnNextSceneType(SCENE_TYPE::SCENE_NONE)
	, mpCurrentScene(nullptr)
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Initialize()
{
	// 初期シーンの変更　　シーンの追加
	mnNextSceneType = SCENE_TYPE::TITLE;

	// シーン遷移をさせる
	ChangeSceneIfNeeded();
}

void SceneManager::Update()
{
	// シーンの更新
	mpCurrentScene->Update();
}

void SceneManager::Draw()
{
	// シーンの描画
	mpCurrentScene->Draw();
}

void SceneManager::Finalize()
{

}

void SceneManager::ChangeSceneIfNeeded()
{
	// 現在シーンと次のシーンが一緒であれば何もしない
	if (mnSceneType == mnNextSceneType)
	{
		return;
	}

	if (mpCurrentScene != nullptr)
	{
		// 現在シーンの終了処理をする
		mpCurrentScene->Finalize();

		// 一旦シーン自体も破棄しておく
		delete mpCurrentScene;
		mpCurrentScene = nullptr;
	}

	// 次シーンにするためシーンタイプ更新
	mnSceneType = mnNextSceneType;


	// mnSceneType に応じてシーンを作成する
	switch (mnSceneType)
	{
	case SCENE_TYPE::TITLE:
		mpCurrentScene = new Title(); // タイトルシーンの作成
		break;

	case SCENE_TYPE::EXPAIN_SCENE:
		mpCurrentScene = new ExplainScene(); // アイテム説明シーンの作成
		break;

	case SCENE_TYPE::GAME_SCENE:
		mpCurrentScene = new GameScene(mNextGScene); // ゲームシーンの作成
		break;

	case SCENE_TYPE::WINRESULT:
		mpCurrentScene = new WinResultScene(); // 勝利シーンの作成
		break;

	case SCENE_TYPE::LOSERESULT:
		mpCurrentScene = new LoseResultScene(); // 敗北シーンの作成
		break;

	case SCENE_TYPE::SELECT_SCENE:
		mpCurrentScene = new SelectScene(); // レベル選択シーンの作成
		break;

	default:
		break;
	}

	// シーンの生成がされているはずなので、初期化処理を呼んでおく
	mpCurrentScene->Initialize();
	
}