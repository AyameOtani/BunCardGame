#pragma once
#include "Scene.h" // シーン参照したいからインクルード
#include "DxLib.h"
#include <string>
#include "Button.h"
#include "Unit.h"
#include "MouseGraph.h"
#include "SceneManager.h"
#include "Card.h" // 追加　小池


class Player;
class Enemy;
class Card; // 追加　小池

class GameScene : public Scene
{

	// ターンのやつ追加した
public:

	enum class Turn
	{
		PLAYER_TURN,      // プレイヤーがカードを選べる時間
		PLAYER_ANIMATION, // プレイヤーがカードを使ってその演出が終わるのを待つ時間

		ENEMY_THINK,      // 敵のターン開始  3秒待つ
		ENEMY_CARD_SHOW,  // カード演出　追加 小池
		ENEMY_ACTION,     // 敵が攻撃したり  カードを引いたりする演出の時間

		ENEMY_CARD_HIDE, // 追加 小池　敵のカードを隠す時間


		RESULT_CHECK      // どちらかのHPが0になったか確認する時間
	};

	// ターンをプレイヤーにする
	Turn mTurn = Turn::PLAYER_TURN;


public:
	// コンストラクタ
	GameScene(SceneManager::GScene gscene);
	// デストラクタ
	~GameScene() override;
	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() override;
	// 終了処理
	virtual void Finalize() override;

	// 持続ダメージと残りターンを減らすやつ
	void UpdateUnitStatus(Unit& unit);

	// ノートのスライド
	void ShowItemMessage(const std::string& text, const int& color);

private:  // メンバ変数として定義
	Player* mpPlayer = nullptr;
	Enemy* mpEnemy = nullptr;

	// ボタン関係
	MouseGraph* mpTurnEnd = nullptr; // ターンエンドボタンのポインタ
	MouseGraph* mpStop = nullptr; // ゲーム停止のボタン

	// 停止中のボタン関係
	MouseGraph* mpTitle = nullptr;	 // タイトルボタン
	MouseGraph* mpGame = nullptr;	 // ゲームボタン
	MouseGraph* mpExplain = nullptr; // 説明ボタン
	MouseGraph* mpClose = nullptr; // 閉じるボタン

	// 音量調整のやつ
	MouseGraph* mpHaguruma = nullptr; // 歯車のやつ

	// 音量設定ON/OFF
	bool mbOption = false;

	// 音量設定
	MouseGraph* mpMusicClose = nullptr;

	int mnMusicBag = -1;
	int mnOnpuHandle = -1;

	// 音量バー設定
	int mBarX = 570;
	int mBarWidth = 840;
	int mBarHeight = 40;

	int mBgmBarY = 460;
	int mSeBarY = 650;

	int mnBarDist = 20;
	int mnBarUp = 40;
	int mnBarDown = 65;

	int mBarString = 70;
	int mMusicNote = 15;

	// 音量
	int mBgmVolume = 255;
	int mSeVolume = 255;

	// スクリーン関係
	int mnCardWorkScreen = -1;
	int mnCardFontHandle = -1;


	// フォント関係
	int mnFontBig; // 大きいフォント　カウントで使う
	int mnFontNormal; // ふつうのフォント
	int mnFontSmall; // 小さいやつ


	// 演出や待ち時間用の汎用タイマー
	int mWaitTimer = 0;

	// 画像関係
	int mnBagHandle = -1; // 背景画像　教室

	// 今どのアイテムを選んでいるか
	int mnItemSelect = 1; // アイテム選択のやつ

private: //敵の攻撃用

	//ランダム用	  抽選はCPPに移動した
	int Action;

	//敵の必殺技チャージカウント
	int ChargeCount;
	//プレイヤーのチャージカウント
	int PlayerChargeCount = 0;

	//敵の必殺技チャージ取得
	bool ChargeAttack;

	// 敵行動カード演出 追加 小池
	bool mbEnemyCardShow = false;

	float mfEnemyCardX = 960.0f;
	float mfEnemyCardY = -300.0f;

	float mfEnemyTargetY = 350.0f;

	Card* mpEnemyActionCard = nullptr;

	// 敵カードの回転　追加　小池　5/15

	// カード回転演出用
	float mEnemyCardAngleY = 0.0f;
	bool mbEnemyCardFlipping = false;
	bool mbEnemyCardFront = false;
	bool mbEnemyCardNextFront = true;
	bool mbEnemyCardSwitched = false;

	float mEnemyCardAnimSpeed = 0.22f;
	float mEnemyCardPerspective = 40.0f;

	// 敵カード裏画像
	int mnEnemyCardBack = -1;

	// 敵カードの状態 追加　小池　5/15
	enum class EnemyCardState
	{
		NONE,         // 何もしてない
		FLIP_TO_FRONT,// 裏→表
		SHOW_FRONT,   // 表を表示中
		FLIP_TO_BACK  // 表→裏
	};

	EnemyCardState mEnemyCardState =EnemyCardState::NONE;

	
private:
	int mnHandleMP = -1; // MPの画像ハンドル 背景
	int mnHandleIsMpError = -1; // MP足りない時の画像ハンドル
	int mnHandleDeck = -1; // 山札のハンドル
	int mnHandleTrash = -1; // ゴミ箱のハンドル
	int mnHandleTurnBag = -1; // ターンの背景のハンドル

	// ゲームの初期化が終わったかどうかのフラグ
	bool mbInitialize = false;

	SceneManager::GScene mGScene; // 難易度のやつ
	bool mbStop = false; // 止まるボタンを押しているか


	// ポーズ関係
	bool mbSelectON = false; // ポーズで説明が選ばれたか
	int mnItemEHandle = -1;

	// ターン数の表示の演出
	float mfTurnY = -100.0f;
	float mfTurnBox = -100.0f; // ターンの後ろに表示している画像

	// ノートの画像
	int mnNoteHandle = -1;
	int mnNoteMini = -1; // ノートみにの画像
	
	// アイテム通知
	std::string mItemMessage = "";
	int mnColor = GetColor(60, 60, 60); // デフォルトで黒

	// 初めのmessageがでているか
	bool IsMessage = false;

	float mfItemNoteX = 1920.0f;
	float mfItemTargetX = 1450.0f;

	bool mbItemSlide = false;
	int mItemMessageTimer = 0;

};
	