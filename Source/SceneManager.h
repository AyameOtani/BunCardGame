#pragma once

// クラスの前方宣言
class Scene;

class SceneManager
{
public: // enum, struct の定義

	// シーンの種類
	// note: シーンを増やす必要があればここも追加していくこと
	enum SCENE_TYPE
	{
		SCENE_NONE = 0,     // 定義なし

		TITLE,        // タイトル
		ITEM,         //アイテム説明
		SELECT_SCENE, // レベル選択画面
		GAME_SCENE,   // ゲーム画面
		WINRESULT,    // 勝利画面
		LOSERESULT,   // 敗北画面
		EXPAIN_SCENE, // 操作説明画面
	};

	// 難易度の種類
	enum class GScene
	{
		easy,
		normal,
		hard
	};


public: // メンバ関数の定義

	// コンストラクタ
	SceneManager();
	// デストラクタ
	~SceneManager();


	//初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了処理
	void Finalize();


	// シーン遷移(切り替え)が必要な状態なら遷移処理をする
	void ChangeSceneIfNeeded();

	SCENE_TYPE GetCurrentSceneType() const { return mnSceneType; }
	

	// ゲームシーン限定で使うレベル設定できるやつ 大谷
	void SetNextScene(SCENE_TYPE next, GScene scene)
	{
		mnNextSceneType = next;
		mNextGScene = scene; // ゲームシーンを代入
	}

	// 次に遷移するシーンの設定
	void SetNextScene(SCENE_TYPE next) {mnNextSceneType = next; }

	// 現在シーンの取得
	Scene* GetCurrentScene() { return mpCurrentScene; }

private:
	SCENE_TYPE mnSceneType;     // 現在シーンのタイプ
	SCENE_TYPE mnNextSceneType; // 次のシーンのタイプ
	Scene* mpCurrentScene;      // 現在シーンのポインタ

private:
	// 難易度選択のやつ
	GScene mNextGScene = GScene::normal; // 次のシーンの難易度の種類を保存する変数

};
