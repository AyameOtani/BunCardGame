#pragma once
#include "AnimationData.h"
#include "Part.h"
#include <vector>

/// <summary>
/// アニメーションを管理するクラス
/// 開始されたか、終了されたかなどを把握するため
/// </summary>
class Animator
{
private:
	const AnimationData* currentAnim = nullptr;
	int timer = 0;  // 再生時間

public:
	// アニメーションの再生を開始させるための関数
	void Play(const AnimationData* anim);
	// パーツごとに更新するので引数にPartのvectorを入れている
	void Update(std::vector<Part>& parts);


	// 今再生しているアニメーションを返す
	const AnimationData* GetCurrentAnim() const { return currentAnim; }
	// 今再生中のアニメーションのタイプを返す
	AnimationData::AnimType GetCurrentAnimType() const;

	// アニメーション終了チェック
	bool IsFinished() const;

	// アニメーションの所有者をけっていするための関数
	// デフォルトでプレイヤーにしている
	AnimationData::AnimOwner ownerType = AnimationData::AnimOwner::PLAYER;
};
