#pragma once

#include "Object2D.h"
#include "BuffEffect.h"
#include "DebuffEffect.h"	 
#include "Part.h"
#include "AnimationData.h"
#include "Animator.h"
#include "CardEffect.h"


class Unit : public Object2D
{
public:
	Unit(std::string filename, VECTOR initPos);
	~Unit();

	// バフデバフ関係追加
	std::vector<Buff> buffs;
	std::vector<Debuff> debuffs;

	void UnitUpdate();	// HPのクランプとか

	// 今表示しているアイテムのポインタ
	Object2D* pShieldEffect = nullptr;	   //防御エフェクト
	Object2D* pPoisonEffect = nullptr;	   // 毒のエフェクト
	Object2D* pBuffEffect = nullptr;       // バフエフェクト
	Object2D* pDebuffEffect = nullptr;     // デバフエフェクト
	Object2D* pStunEffect = nullptr;        // スタンエフェクト

	// 共通のステータス
	int hp;
	int maxHp;
	int block = 0;
	int curse = 0; // 呪いのカウント


	//アニメーション関係
	virtual void PlayMotion(AnimationData::AnimType type) = 0; // 純粋仮想関数でやってる


public:
	std::vector<Part> parts; // そのキャラを構成するパーツ
	Animator animator;       // そのキャラを動かすアニメーター

	// アニメを再生する指示
	void PlayAnimation(const AnimationData* data)
	{
		animator.Play(data);
	}

	void AnimUpdate()
	{
		animator.Update(parts); // アニメーターがパーツの目標値を更新
		for (auto& p : parts)
		{
			UpdatePart(p);      // パーツが目標値に向かってヌルっと動く
		}
	}
};