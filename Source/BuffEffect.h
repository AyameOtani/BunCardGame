#pragma once

// プレイヤー用のバフ種類
enum class BuffType
{
    ATTACK,  // 攻撃力上昇（カードの DAMAGE に加算する想定）
    DEFENSE, // 防御系（必要なら利用）
    ENERGY   // ターン開始などでエネルギーを回復するバフ
};

struct Buff
{
    BuffType type;
    int value;   // バフ量
    int turns;   // 残りターン数
};