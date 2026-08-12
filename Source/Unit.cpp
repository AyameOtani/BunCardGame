#include "Unit.h"
#include "Master.h"
#include "Enemy.h"

Unit::Unit(std::string filename, VECTOR initPos)
	: Object2D(filename, initPos)
{

}

Unit::~Unit()
{

}

void Unit::UnitUpdate()
{
    // アニメーターの更新
    animator.Update(parts);

    // 目標値に向かってパーツを動かす
    for (auto& p : parts)
    {
        UpdatePart(p);
    }

    // 今のアニメが終わっている かつ 生きているなら待機アニメ
    if (animator.IsFinished() && hp > 0)
    {
        // 今のアニメが IDLE じゃない時だけ Play を呼ぶ（念のため）
        if (animator.GetCurrentAnimType() != AnimationData::AnimType::IDLE)
        {
            const AnimationData* idleData = Master::mpAnimationManager->GetAnim(AnimationData::AnimType::IDLE);
            animator.Play(idleData);
        }
    }



	// HPのクランプ
	if (hp <= 0)
	{
		hp = 0;
	}

    // 初期化
    VECTOR animOffset = VGet(0.0f, 0.0f, 0.0f);

    if (!parts.empty())
    {
        // 体の位置にエフェクトを追従させる
        float ox = parts[0].targetXOffset;
        float oy = parts[0].targetYOffset;
        animOffset = VGet(ox, oy, 0.0f);
    }


    // シールドエフェクトの管理
    if (pShieldEffect != nullptr)
    {
        if (block <= 0)
        {
            // ブロックがなくなったら削除フラグを立てる
            pShieldEffect->SetDeleteFlag(true);
            pShieldEffect = nullptr; // ポインタを空にした
        }
        else
        {
            // キャラが動いてもエフェクトがついてくるように座標を更新
            pShieldEffect->SetPosition(VAdd(VSub(mvPosition, VGet(0.0f, 20.0f, 0.0f)), animOffset));
        }
    }

    // 毒エフェクトの管理
    if (pPoisonEffect != nullptr)
    {
        bool hasPoison = false;
        for (const auto& d : debuffs)
        {
            if (d.type == DebuffType::DEBUFF_POISON) // 毒のデバフならTRUE
            {
                hasPoison = true;
                break;
            }
        }

        if (!hasPoison)
        {
            pPoisonEffect->SetDeleteFlag(true);
            pPoisonEffect = nullptr;
        }
        else
        {
            //pPoisonEffect->SetPosition(mvPosition);
            pPoisonEffect->SetPosition(VAdd(mvPosition, animOffset));
        }
    }

    // バフのエフェクト
    if (pBuffEffect != nullptr)
    {
        if (buffs.empty())
        {
            pBuffEffect->SetDeleteFlag(true);
            pBuffEffect = nullptr;
        }
        else
        {
            VECTOR buffPos = VAdd(mvPosition, VGet(-60.0f, 00.0f, 0.0f));
            pBuffEffect->SetPosition(VAdd(buffPos, animOffset));
        }
    }


    // デバフのエフェクト
    if (pDebuffEffect != nullptr)
    {
        if (debuffs.empty())
        {
            pDebuffEffect->SetDeleteFlag(true);
            pDebuffEffect = nullptr;
        }
        else
        {
            VECTOR debuffPos = VAdd(mvPosition, VGet(60.0f, 0.0f, 0.0f));
            pDebuffEffect->SetPosition(VAdd(debuffPos, animOffset));
        }
    }

    // スタンのエフェクト
    if (pStunEffect != nullptr)
    {
        Enemy* pEnemy = dynamic_cast<Enemy*>(this);
        if (pEnemy->stunTurns <= 0) // スタンのターンが残っているならTRUE
        {
            pStunEffect->SetDeleteFlag(true);
            pStunEffect = nullptr;
        }
    }
    
}
