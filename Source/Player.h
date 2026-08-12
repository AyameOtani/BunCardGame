#pragma once
#include "DxLib.h"
#include <vector>
#include "Card.h"
#include "Unit.h"
#include "Enemy.h"
#include "Part.h"
#include "AnimationData.h"
#include "Animator.h"



class Player : public Unit
{
public:

    // MPはプレイヤーだけにしているけど知らん
    int mp = 3;
    int maxMp = 3;


public:
    // アイテム関係
    bool doubleNextCard = false;      // ハサミ用
    int keepNextCardCount = 0;        // のり用
    bool randomizeCostThisTurn = false; // ペン用
    std::string itemEffectText = "";

    // アイテムアイコン
    int itemImageHandles[3]; // 0:ハサミ, 1:のり, 2:ペン
    int itemUsedHandles[3];
    VECTOR itemPositions[3];
    // 追加
    static constexpr float ITEM_SCALE = 0.3f;
    // 追加
    bool usedItems[3] = { false, false, false }; // アイテム使用済みフラグ


    // カード関係
    std::vector<Card*> hand;        // 手元のカードを入れるvector
    std::vector<Card*> deck;        // 山札にあるカードのリスト
    std::vector<Card*> discardPile; // ゴミ箱のカードを入れるvector
    std::vector<Card*> exhaustPile; // 除外  一回きりのカードのvector 
    std::vector<Card*> pickOption;  // 選択として表示する2枚のカード

    static const int HAND_CARD_MAX = 7; // 手札の最大枚数
    static const int FIRST_HAND_CARD = 5; // シャッフルあとに配られるカードの枚数
    static const int DECK_MAX = 20;       // デッキの最大枚数

    Player(VECTOR initPos, std::string filename, SceneManager::GScene gscene);  // コンストラクタ
    ~Player(); // デストラクタ

    void Draw();      // プレイヤーの描画
    void Update(Enemy& enemy); // アイテムの効果のやつやってる　4/21 大谷

    void CardUpdate(Enemy& enemy); // オブマネに追加されないから注意するんだ
    void Initialize(); // 初期化


    // バトル開始時に山札やったり
    void StartBattle();

    // 特定のカードを手札に加える
    // Drawはドロー！の方ね
    void DrawCard(int num);

    // 手ふだを扇状に何らべ手描画するため
    void DrawHand(int workScreen, int fontHandle);

    // カードが増減した場合にきれいになら別処理
    void RefreshCardPositions();

    // ターン終了時の全捨て処理
    void ReshuffleTurnEnd();

    // 山札のデッキが足りない時のリシャッフル処理
    void DiscardIntoDeck();

    // 選択肢を2枚生成する関数
    void GeneratePickOntions();

    // 今カードを選択中かどうかのフラグ
    bool GetIsPicking() { return isPicking; }

    // カードが今演出中かの関数 動いてたらTRUE返す
    bool IsCardMove();




    // HP関係 --------------------------------------
    // Hpゲージを更新する関数
    void HpGaugeUpdate();

    // HPバーの描画　　gameSceneで呼び出す
    void HpGaugeDraw();

	void IconDraw(); // アイコンの描画　gameSceneで呼び出す

    // MPエラーのフラグを返す関数
	bool GetMpError() const { return IsMpError; }

    // 必殺技関係 --------------------------------------
    // 必殺技ゲージを更新する関数
    void SpecialGaugeUpdate();

    // 必殺技ゲージの描画　　gameSceneで呼び出す
    void SpecialGaugeDraw();

    // 必殺ゲージがどこまで貯まったかのゲッターと最大のやつ
    int GetSpecialGauge() { return specialCharge; }
    int GetMaxSpecialGauge() { return specialChargeMax; }
    void SetSpecialGauge(int sp) { specialCharge = sp; }

public:


    // MPのセッター
    // プレイヤーのターンになったらリセットするやつ
    void SetMp(int m) { mp = m; }
    int GetMaxMp() { return maxMp; } // 最大値セット

    // 必殺技関係
    bool GetIsSpecialPicking() const { return isSpecialPicking; }
    void StartSpecialPick();                  // 必殺技選択を開始する関数


    // アニメーション関係
    void PlayMotion(AnimationData::AnimType type) override;

    // パーツごとのやつ
    enum PartID
    {
        LEG_L = 0, // 足
        LEG_R,     // 足後ろ側
        WEAPON,    // 竹刀
        HEAD,      // 頭
        BODY,      // 体
        ARM_R,     // 手
        PART_MAX
    };


private:
    // 今どのカードをつかんでいるか  ドラッグ状態
    int draggingIndex = -1;
    float DraggY = 730.0f; // カードを出すところの基準

    // エラーメッセージ表示フラグ
    bool IsMpError = false;

    // 今選択中かどうかのやつ ゲーム開始時
    bool isPicking = false;

    // Hpゲージで使用する変数 //
    int width = 300;                          // Hpゲージの幅
    int height = 25;                          // Hpゲージの高さ
    int gaugeWidth = 0;                       // 現在値に応じたHpゲージの幅
    int gaugeDamegeWidth = 0;                // ダメージ分のゲージの幅
	int displayDamageHp;                     // 表示上のHP　ダメージを赤くするために実際のHPと分けている
	int BarTimer = 0;                        // ダメージを受けてからの時間　ダメージゲージを減らすタイミングに使う
	int displayHp;                           // 表示上のHP
    float BarDamageSpeed = 2.0f;             // ダメージゲージが減る速さ　フレームごとにどれだけ減らすか
	const int BAR_FRAME = 15;               // ダメージゲージの減少フレーム数
    // --------------------- //

    // 必殺技ゲージで使用する変数 //
    int specialCharge;                         // 必殺技ゲージ　0~100で管理することにする
    int specialChargeMax;                      // 必殺技ゲージの最大値　100にする予定
    int specialWidth = 70;                     // 必殺技ゲージの幅
    int specialHeight = -300;                  // 必殺技ゲージの高さ
    int specialGaugeHeight = 0;                // 現在値に応じた必殺技ゲージの高さ

    bool isSpecialPicking = false;            // 必殺技選択中か
    std::vector<Card*> specialOptions;        // 必殺技の選択肢(3枚)

	int mnPoisonHandle = -1; // 毒のエフェクトの画像ハンドル
	int mnShieldHandle = -1; // 防御のエフェクトの画像ハンドル
    // 一回だけSEを流す
    bool isPickVoicePlayed = false;

    // アイテムのアイコンでっかくするやつ
    float mfItemSizes[3] = { 0.25f, 0.25f, 0.25f };
    // 今mouseが重なっている番号
    int mnHoverItem = -1; // 初期は-1にする

    // カードを選択して...のやつの画像
	int mnCardOperateHandle = -1;

    SceneManager::GScene mGScene; // 難易度のやつ

    // HPバーの最低幅
    int mnHpLow = 5;
};


