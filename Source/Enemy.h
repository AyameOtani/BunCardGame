#pragma once
#include "Unit.h"
#include <vector>
#include "Card.h"

class Enemy : public Unit
{
public:
    enum class EnemyType
    {
        TypeA,
        TypeB,
        TypeC,
    };

    Enemy(VECTOR initPos, std::string filename, EnemyType type);
    ~Enemy();

    void Update();
    void Draw();

    // Hpゲージを更新する関数
    void HpGaugeUpdate();
    void HpGaugeDraw(); // 描画わける　エフェクトで隠れないようにGameSceneで呼び出す
    void Initialize(std::string filename);

    // アイコンの表示
    void EnemyIcon();

    //敵カードの位置セット
    void CardSet();
    //敵カードの生成
    void EnemyStartBattle();
    //敵側のカード描画
    // topCard を指定するとそのカードは手札描画時に除外され、そのカードは手札描画時に除外され、最後に topCard を重ねて描きます
    void EnemyCardDraw(int workScreen, int fontHandle, Card* topCard = nullptr);

    int stunTurns = 0; // 0なら動ける  1以上なら動けない   スタンのアイテムのやつね

    // 敵のアニメーション関係
    void PlayMotion(AnimationData::AnimType type) override;
    // パーツごと
    enum PartID
    {
        BODY = 0,
        PART_MAX
    };

    std::vector<Card*> enemyhand;        // 手元のカードを入れるvector

public:
    //敵の数値のゲッター
    // GameSceneに使う
    int GetDamage() { return mnDamage; }
    int GetBlock() { return mnBlock; }
    int GetBuff() { return mnApplyBuff; }
    int GetPoison() { return mnPoison; }
    int GetWeaken() { return mnApplyWeaken; }

private:
    // Hpゲージで使用する変数 //
    int width = 300;                          // Hpゲージの幅
    int height = 25;                          // Hpゲージの高さ
    int gaugeWidth = 0;                       // 現在値に応じたHpゲージの幅
	int gaugeDamegeWidth = 0;                // ダメージ分のゲージの幅
	int displayDamageHp;                           // 表示上のHP　減るときにゆっくり減らす用
	int DamageBarTimer = 0;                  // ダメージバーのタイマー
    float DamageBarSpeed = 2.0f;             // ダメージバーが減る速さ　フレームごとにどれだけ減らすか
	const int DAMAGE_BAR_FRAME = 15;         // ダメージバーの減少フレーム数
    // ------------ /

    //アイコン系
    int mnPoisonHandle = -1; // 毒のエフェクトの画像ハンドル
    int mnShieldHandle = -1; // 防御のエフェクトの画像ハンドル
    int mnCurseHandle = -1; // 呪いのエフェクトの画像ハンドル

    // 敵のランダム行動の時の数値
    // デフォ値設定した
    int mnDamage = 10;
    int mnBlock = 8;
    int mnApplyBuff = 5;
    int mnPoison = 5;
    int mnApplyWeaken = 5;


private:
    EnemyType mType; //敵のタイプ

    // HPバーの最低保障
    int mnHpLow = 5;
};
