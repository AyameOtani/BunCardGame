#include "InputManager.h"
#include "Master.h"
#include "Ce.h"
#include "Mouse.h" 

Ce::Ce()
{

}

Ce::~Ce()
{

}


void Ce::Initialize()
{
	mnCeHandle = LoadGraph( "Resource/Mouse/Ce.png" ); // 背景画像
}


void Ce::Update()
{
	//マウスがクリックされたらゲームシーンに行く
	if(Mouse::IsTrigger())
	{
		Master::mpSceneManager->SetNextScene( SceneManager::ITEM );
		Master::mpSoundManager->PlaySE( SoundManager::SE_DECIDE );
	}
	Scene::Update();
}

void Ce::Draw()
{
	DrawGraph( 0 , 0 , mnCeHandle , TRUE ); // 操作説明の描画

	Scene::Draw();
}

void Ce::Finalize()
{
	DeleteGraph( mnCeHandle );
}