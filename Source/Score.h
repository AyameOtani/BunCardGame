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
		RANK_NONOE,  // ランクなし　初期化
		RANK_MAX,    // 花丸ランク
		RANK_NORMAL, // 二重丸ランク
		RANK_LOW,    // まるランク
	};

	ScoreRank mScoreRank; // スコアランク

	// テキストのY座標を移動させる
	float GetMoveY() const { return mfTurnBox;}

private:

	// Y座標を移動させる
	bool MoveScoreY(
		float& currentY,
		float targetY,
		float& velocity
	);

	// スコア画像を読み込む
	void LoadScoreImages();

	// スコアの文字を描画する
	void DrawScoreStrings();

	// スコアの文字を1つ描画する
	void DrawScoreString(
		const char* text,
		int value,
		int y
	);

	// HPのスコアを計算する
	int CalculateHpScore(int hp);

	// ターン数のスコアを計算する
	int CalculateTurnScore(int turn);

	// 総合ランクを計算する
	void CalculateScoreRank();

	// ランクに対応した画像ハンドルを取得する
	int GetScoreRankHandle();

	// ランク画像を描画する
	void DrawScoreRank();


	// スコア画像
	int mnScoreMax;
	int mnScoreNormal;
	int mnScoreLow;

	int mnDrawScoreTime; // スコアを描画する時間

	float mfTurnMoveY;    // 経過ターン数のY
	float mfUseCardMoveY; // 使用カード枚数のY
	float mfUseItemMoveY; // 使用アイテム数のY

	bool mbInitialize; // 初期化が終わったか
	bool mbMove;       // 文字の動きが終わったか

	float mfMoveY;   // HPのY
	float mfTurnBox; // 勝利の文字のY


	// 文字をバウンドさせる速度
	float mfHpVelocity;
	float mfTurnVelocity;
	float mfUseCardVelocity;
	float mfUseItemVelocity;
	float mfTurnBoxVelocity;
};