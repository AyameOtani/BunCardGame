#pragma once


// 効果の種類
enum class EffectType
{
    DAMAGE,
    BLOCK,
    DRAW,
    ENERGY_MP,  // MP
    APPLY_POISON,      // 毒を付与する
    APPLY_BUFF,        // バフ 攻撃UP等 を付与する これは自分に付与ね
    APPLY_WEAKEN,      // 弱体を付与する
    APPLY_ATTACK_DOWN,  // 攻撃力減少を付与する
    APPLY_CURSE,        // 呪いを付与

    // アイテム関係
    EXTRA_PLAY,  // ハサミ  ２回行動
    KEEP_CARD,   // のり    カード持ち越し
    PEN_RANDOM,  // ペン    ランダム効果
};

// 効果データ
// 種類　値　継続数（ターン数）
struct Effect
{
    EffectType type;
    int value = 0;
    int count = 0;
    bool isSpecial = false;// 必殺の効果かどうか

    // 菊池
    // ある効果の中にダメージ効果があるかどうかのフラグ
    bool parentHasDamage = false;

    bool BlockBuff = false;

    bool MpHeel = false;

    bool mpHeel_1 = false;

    bool DoubleAttack; // ２回攻撃
};
