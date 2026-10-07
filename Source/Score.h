#pragma once

#include "DxLib.h"

namespace ScoreSetting
{
    constexpr int ScorePointMax = 3;
    constexpr int ScorePointNormal = 2;
    constexpr int ScorePointLow = 1;
}


class Score
{
public:
    Score();
    ~Score();

    void Update();
    void Draw();

    // スコア表示位置を取得
    float GetMoveY() const
    {
        return mfTurnBox;
    }

private:

    enum class ScoreRank
    {
        RANK_NONE,   // ランクなし
        RANK_MAX,    // 花丸ランク
        RANK_NORMAL, // 二重丸ランク
        RANK_LOW     // ただのまるランク
    };

    // Y座標を移動する
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

    // HPの残数の描画
    void DrawHpScoreString();

    // 経過ターン数の描画
    void DrawTurnScoreString();

    // 使用カード枚数の描画
    void DrawUseCardScoreString();

    // 使用アイテム数の描画
    void DrawUseItemScoreString();


    // 現在のスコアランク
    ScoreRank mScoreRank;

    // スコア画像
    int mnScoreMax;
    int mnScoreNormal;
    int mnScoreLow;

    // スコアを描画する時間
    int mnDrawScoreTime;

    // テキストのY座標
    float mfMoveY;
    float mfTurnMoveY;
    float mfUseCardMoveY;
    float mfUseItemMoveY;

    // 勝利の文字のY座標
    float mfTurnBox;

    // 初期化・移動状態
    bool mbInitialize;
    bool mbMove;

    // 各文字の移動速度
    float mfHpVelocity;
    float mfTurnVelocity;
    float mfUseCardVelocity;
    float mfUseItemVelocity;
    float mfTurnBoxVelocity;
};