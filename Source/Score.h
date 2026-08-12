#pragma once

#include "DxLib.h"

class Score
{
public:
	Score();
	~Score();

	void Initialize();

	void Update();
	void Draw();

	//----追加した関数----//
	void DrawHpScoreString();      // HPの残数の描画
	void DrawTurnScoreString();    // 経過ターン数の描画
	void DrawUseCardScoreString(); // 使用カード枚数の描画
	void DrawUseItemScoreString(); // 使用アイテム数の描画
	//----ここまで----//

	enum class ScoreRank
	{
		RANK_NONOE, // ランクなし　初期化

		RANK_MAX,	 // 花丸ランク
		RANK_NORMAL, // 二重丸ランク
		RANK_LOW,	 // まるランク
	};

	ScoreRank mScoreRank = ScoreRank::RANK_NONOE; // スコアランク


	float GetMoveY() const { return mfTurnBox; } // Yの位置を取得するやつ
private:

	// もともと読み込んでおく
	int mnScoreMax;
	int mnScoreNormal;
	int mnScoreLow;

	int mnHandle; // スコア画像のハンドル

	//----追加したやつ----//
	int DrawScoreTime = 0; // スコアを描画する時間

	float mfTurnMoveY = -100.0f; // バウンドのやつ 文字のY
	float mfUseCardMoveY = -100.0f; // バウンドのやつ 文字のY
	float mfUseItemMoveY = -100.0f; // バウンドのやつ 文字のY
	//----ここまで----//


	bool mbInitialize = false; // 初期化が終わったか
	bool mbMove = false; // 文字の動きが終わったかのフラグ
	float mfMoveY = -100.0f; // バウンドのやつ 文字のY
	float mfTurnBox = -100.0f; // 勝利の文字

};