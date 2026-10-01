#pragma once

#include "DxLib.h"

class Score
{
public:
	Score();
	~Score();

	void Update();
	void Draw();

	void DrawHpScoreString();      // HPの残数の描画
	void DrawTurnScoreString();    // 経過ターン数の描画
	void DrawUseCardScoreString(); // 使用カード枚数の描画
	void DrawUseItemScoreString(); // 使用アイテム数の描画


	enum class ScoreRank
	{
		RANK_NONOE, // ランクなし　初期化
		RANK_MAX,	 // 花丸ランク
		RANK_NORMAL, // 二重丸ランク
		RANK_LOW,	 // まるランク
	};

	ScoreRank mScoreRank; // スコアランク

	float GetMoveY() const { return mfTurnBox; } // Yの位置を取得するやつ

private:

	bool MoveScoreY(float& currentY,
		float targetY,
		float& velocity);

	// もともと読み込んでおく
	int mnScoreMax;
	int mnScoreNormal;
	int mnScoreLow;
	int mnHandle; // スコア画像のハンドル

	int mnDrawScoreTime; // スコアを描画する時間
	float mfTurnMoveY; // バウンドのやつ 文字のY
	float mfUseCardMoveY; // バウンドのやつ 文字のY
	float mfUseItemMoveY; // バウンドのやつ 文字のY


	bool mbInitialize; // 初期化が終わったか
	bool mbMove; // 文字の動きが終わったかのフラグ
	float mfMoveY; // バウンドのやつ 文字のY
	float mfTurnBox; // 勝利の文字


	// テキストをバウンドさせるもの 文字のYの速度
	float mfHpVelocity;
	float mfTurnVelocity;
	float mfUseCardVelocity;
	float mfUseItemVelocity;
	float mfTurnBoxVelocity;
};