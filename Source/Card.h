#pragma once
#include <vector>
#include "CardEffect.h"
#include <string>
#include "SceneManager.h"


class Card
{
public:

    static const int CARD_COUNT = 17; // カードの枚数  switch増やしたら増やしてくれ　 記述大谷
    bool isKeep; // アイテムの効果で今キープされているかのフラグs


    // カードのコストタイプのenum
    enum class CostType
    {
        COST_0,
        COST_1,
        COST_2,
        COST_3,
    };


    // カードが個別に持ってるデータ
    struct CardData
    {
        int id;                      // ID追加
        std::string name;            // 名前
        std::string cardFilename;    // カードの画像ファイルの名前
        std::string frameName;       // カードの枠のやつ　コンストラクタで変える
        int cost;
        std::vector<Effect> effects; // 効果を複数持てるようにvector

        bool isExhaust;              // 一回きりかどうか
        std::string desciption;      // 効果の説明テキスト
        bool isSpecial = false;      // 必殺カードかどうか
    };


    // 敵のカードのデータ
    struct EnemyCardData
    {
        int id;                       // ID
        std::string enemyName;            // 名前
        std::string frontImageName;   // カードの画像ファイル
        bool mbEnemyFlag = true;      // 敵のカードかのフラグ
        std::string EnemyframeName;   // 枠の名前

        std::string desciption;       // カードの効果の説明
        std::vector<Effect> effects;  // カードの効果　エフェクト

        // カードの表が効果とかの方
        bool mbFront = false;         // 表かのフラグ デフォで裏にしておく
        std::string backImageName;     // カードの表の画像 全部同じ
    };

	// 敵の行動を決めるための変数　攻撃なら1　防御なら2　バフなら3　毒なら4　弱体なら5 追加　小池
    void SetEnemyAction(int action)
    {
        mEnemyAction = action;
    }

    int GetEnemyAction() const
    {
        return mEnemyAction;
    }

    int GetId() const
    {
        return enemyData.id;
    }
   
public:
    // 位置とかのゲッター
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetRot() const { return rot; }
    float GetScale() const { return scale; }

public:

    Card(const CardData& cardData); // 今のコストとか名前を持つため
    Card(const EnemyCardData& enemyData); // 敵のカードのデータ
	int mEnemyAction = 0; // 敵の行動を決めるための変数　攻撃なら1　防御なら2　バフなら3　毒なら4　弱体なら5 追加　小池

    ~Card();
    void Update();
    void LateUpdate(); // 座標の補間とか演出中だからずっと動かす


    // 描画するスクリーン フォントのハンドル どれかがホバーとかドラック中か  選択フェーズか
    void Draw(int workScreen, int fontHandle, bool isFocus, bool isPickOption = false);

    // 当たり判定   カードのOBB判定
    bool CheckHover(float mx, float my);

    // 枠画像のサイズを返す関数 場所指したいから＊
    void GetFrameSize(int* w, int* h);

    // データを全部返すやつ
    const CardData& GetData() const { return data; }

    // コスト関係
    CostType GetCostType() const;             // タイプによって画像とか変えるため
    int GetCost() const { return data.cost; } // コストを返すゲッター

    // カードのデータを返す関数
    //static CardData GetCardDataById(int id, SceneManager::GScene gscene);
    static CardData GetCardDataById(int id);

    //必殺技のカードの関数
    //static CardData SpecialCardDataById(int id, SceneManager::GScene gscene);
    static CardData SpecialCardDataById(int id);

    // 敵のカードの描画
	static EnemyCardData EnemyCardDataById(int id);

    // カードの位置を決める
    void SetCardPosition(float _x, float _y, float _rot);

    // 今注目されているか
    void SetFocus(bool hover, bool dragging);

    // エフェクトを返すゲッター   今使ってない
    const std::vector<Effect>& GetEffects() const { return data.effects; }

    // 演出中かのフラグのゲッター
    bool GetDirection() { return mbDirection; }

    // サイズのゲッター
    void SetScale(float s) { scale = s; }

    // ゴミ箱に行かせるときにやる
    void SetTargetScale(float s) { targetScale = s; }

    // 今の位置を変える関数
    void SetNowPosition(float nx, float ny, float nrot);

    // アイテムの効果のやつ
    // カードのコストを直接書き換えるやつ
    void SetCost(int newCost) { data.cost = newCost; }

    // コスト変更時の枠の見た目のやつ
    void ResetFrameByCost(); 

    // 追加：外部から表裏を強制できるようにする
    void SetShowFront(bool front);
private:
    // 自身のコストとか名前を持つため
    CardData data;

    EnemyCardData enemyData;

	// カードの表と裏のハンドル　敵のカードだけで使う
    int mFrontHandle = -1;
    int mBackHandle = -1;

    // 描画用
    // 初期サイズとか位置とかはCPP　演出用にかえてるよ　大谷
    float x, y, rot;            // XY回転
    float scale;                // サイズ
    float scaleNormal = 0.93f;  // 基準サイズ
    float scaleMax = 0.97f;     // ホバー中のサイズ
    int imgHandle;              // ハンドル
    int frameHandle;            // 枠ハンドル

    // フラグ関係
    bool isHover = false;       // マウスが乗っているか
    bool isDragging = false;    // ドラッグ中か


    // カードの演出用　大谷追加
    float targetX, targetY, targetRot;
    float mfSpeed = 0.07f; // 移動速度
    bool mbDirection = false; // 今カードの演出中か

    float targetScale = 0.93f; // 演出用の目標スケール

	// 敵のカードの表と裏のハンドル 追加　小池　5/15
    int omoteHandle = -1; // 表画像
    int uraHandle = -1;   // 裏画像

    bool showFront = false; // true=表 false=裏

	// カードの回転のやつ　追加　小池　5/15
    float flipScaleX = 1.0f;
    bool isFlipping = false;

	// 追加　小池　5/15
    void StartFlip();
    // 追加：回転制御と状態参照
    void StartFlipReverse();   // 表→裏に回す（逆向きフリップ）
    bool IsFlipping() const;   // 回転中か
    bool IsShowFront() const;  // 現在表向きか
};






