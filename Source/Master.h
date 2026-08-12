#pragma once

#include "SceneManager.h"
#include "SoundManager.h"
#include <memory> // unique_ptr を使うため
#include "ResourceManager.h"
#include "AnimationManager.h"
#include "FontManager.h"






//
// ゲーム内でひとつだけしか存在しないものを管理するクラス
// 例えば、
// SceneManager 　　だったり、（シーン管理クラス）
// SoundManager 　　だったり、（サウンド管理クラス）
// ResourceManager  だったり、（リソース（画像やモデル）管理クラス）
// を、どこからでも参照できるようにしておきたいので
// こういったクラスを作る
// 

class Master
{
public:  // 公開しているから使える　クラスをまたいで使える

	static SoundManager* mpSoundManager;   // サウンドマネージャーのポインタ
	static SceneManager* mpSceneManager;   // シーンマネージャーのポインタ[
	static ResourceManager* mpResourceManager; // リソースマネージャーのポインタ
	static AnimationManager* mpAnimationManager; // アニメーションマネージャーのポインタ
	static FontManager* mpFontManager; // フォントマネージャーのポインタ


	// ターン数の表示
	static int mpTurnCount; // ターンのカウント 一戸だからstaticにしたよ　大谷

	// スコア関係
	static int mpSaveHp; // HPの保存
	static int mpSaveCardCount; // 使ったカードのカウント
	static int mpSaveItemCount; // 使ったアイテムのカウント

};



// ---------------メモ---------------------------------------------1231-------------------
// unique_ptr ... uniqueが勇逸のという意味  +   ptrはポインタという意味らしい
// 所有者が必ず一つだけのポインタという意味
// メリットは、同じものを複数の場所がdeleteしないから前みたいに二重にならぬ
// 所有者がMasterに固定される方メモリリークが防げる
// すこーぷを抜けると自動でdeleteだから安全 

// 前は、MasterもUnitもdeleteしていたから持ってるのが２人だたから二重に消えていたからエラー
