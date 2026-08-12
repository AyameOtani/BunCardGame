#pragma once
#include <vector>

//アニメーションのデータを管理するクラス
class AnimationData
{
public:
	enum class AnimOwner
	{
		PLAYER,
		ENEMY,
	};
	
	// フレームの構造体
	struct AnimFrame
	{
		AnimOwner owner = AnimOwner::PLAYER; // どっちのキャラのパーツを動かすか
		int partIndex = 0;	// どのパーツを動かすか
		float targetAngle = 0;
		float targetX = 0;
		float targetY = 0;
	};



	enum class AnimType
	{
		IDLE,  // 待機
		ATTACK,// 攻撃
		DAMAGE,// ダメージ
		DEATH, // 死亡
		SPECIAL, // 必殺技

		ANIMATION_MAX,
	};

	// ゲッターセッター
	const std::vector<AnimFrame>& GetFrames() const { return frames; }
	void AddFrame(const AnimFrame& frame) { frames.push_back(frame); }

	// タイプのゲッターセッター
	AnimType GetType() const { return type; }
	void SetType(AnimType t) { type = t; }

	// 再生フレームのゲッターセッター
	int GetDuration() const { return duration; }
	void SetDuration(int d) { duration = d; }

	// ループフラグのゲッターセッター
	bool IsLoop() const { return isLoop; }
	void SetLoop(bool loop) { isLoop = loop; }

private:

	AnimType type = AnimType::IDLE; // アニメーションのタイプ
	std::vector<AnimFrame> frames; // パーツを動かす情報
	int duration = 0;			   // 再生フレーム数
	bool isLoop = true; // デフォルトはループする設定にしておく
};
