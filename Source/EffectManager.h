#pragma once
#include "CardEffect.h"
#include "Unit.h"
#include "BuffEffect.h"
#include "DebuffEffect.h"

class EffectManager
{
public:

    // 自分と相手を入れられるようにした
    // Unit継承を追加したから一つの関数でできる  大谷
    static void ApplyEffect(const Effect& effect, Unit& self, Unit& target);

    // バフ用
    static void ApplyBuffEffect(const Buff& buff, Unit& self, Unit& target);

    // デバフ用
    static void ApplyDebuffEffect(const Debuff& buff, Unit& self, Unit& target);


};