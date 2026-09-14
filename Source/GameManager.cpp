#include "GameManager.h"
#include "Master.h"

GameManager::GameManager()
{
	mpSceneManager = std::make_unique<SceneManager>();
	mpSoundManager = std::make_unique<SoundManager>();
	mpResourceManager = std::make_unique<ResourceManager>();
	mpAnimationManager = std::make_unique<AnimationManager>();
	mpFontManager = std::make_unique<FontManager>();
}


GameManager::~GameManager()
{
	Finalize();
}

void GameManager::Initialize()
{
	// アニメーションの初期化
	mpAnimationManager->Initialize();

	// フォントの初期化
	mpFontManager->Initialize();

	// サウンドの初期化
	// シーンより先に初期化する必要がある
	mpSoundManager->Initialize();

	// シーンマネージャーの初期化
	Master::mpGameManager->GetSceneManager()->Initialize();
}

void GameManager::Update()
{
	// サウンドの更新
	mpSoundManager->Update();

	// シーンの更新
	Master::mpGameManager->GetSceneManager()->Update();
}

void GameManager::Finalize()
{
	// シーンの終了処理
	Master::mpGameManager->GetSceneManager()->Finalize();

	// サウンドの終了処理
	mpSoundManager->Finalize();

	// フォントの終了処理
	mpFontManager->Finalize();
}


SceneManager* GameManager::GetSceneManager()
{
	return mpSceneManager.get();
}

SoundManager* GameManager::GetSoundManager()
{
	return mpSoundManager.get();
}

ResourceManager* GameManager::GetResourceManager()
{
	return mpResourceManager.get();
}

AnimationManager* GameManager::GetAnimationManager()
{
	return mpAnimationManager.get();
}

FontManager* GameManager::GetFontManager()
{
	return mpFontManager.get();
}