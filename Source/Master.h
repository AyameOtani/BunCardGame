#pragma once

#include <memory> // unique_ptr を使うため
#include "GameManager.h"


class Master
{
public:  // 公開しているから使える　クラスをまたいで使える
	static GameManager* mpGameManager; // ゲームマネージャーのポインタ


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
