#pragma once
#pragma once
#include <vector>

// デバフ種類を一か所で定義
enum class DebuffType
{
    DEBUFF_POISON, // デバフの毒　今こいつは毒にかかっているかというデータっぽい
    WEAKEN,
    ATTACK_DOWN,    // 攻撃力デバフ
    DEBUFF_CURSE // 今呪いがあるか
};

struct Debuff
{
    DebuffType type;
    int value;   // 種類ごとに意味を決める（POISON -> 毎ターンダメージ）
    int turns;
};