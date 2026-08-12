#pragma once
#include "AnimationData.h"
#include "Part.h"
#include <vector>

class Animator
{
private:
	const AnimationData* currentAnim = nullptr;
	int timer = 0;  // 再生時間

public:
	void Play(const AnimationData* anim);
	void Update(std::vector<Part>& parts);

	// 今再生しているアニメーションを返す
	const AnimationData* GetCurrentAnim() const { return currentAnim; }


	// 今再生中のアニメーションのタイプを返す
	AnimationData::AnimType GetCurrentAnimType() const;

	bool IsFinished() const;    // アニメーション終了チェック

	AnimationData::AnimOwner ownerType = AnimationData::AnimOwner::PLAYER; // デフォルトでプレイヤーにしている
};
