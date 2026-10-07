#include "Score.h"
#include "Master.h"
#include "GameConstants.h"


// 重力、バネ、減衰を使ってYを目標位置まで移動させる
bool Score::MoveScoreY(
	float& currentY,
	float targetY,
	float& velocity)
{
	float gravity = ScoreSetting::Gravity;
	float power = ScoreSetting::SpringPower;
	float damping = ScoreSetting::Damping;

	velocity += gravity;

	// 目標方向への力を加える
	float force = (targetY - currentY) * power;
	velocity += force;

	velocity *= damping;

	// 速度を現在位置に反映
	currentY += velocity;

	// 目標位置にほぼ到達したら固定する
	if (fabs(velocity) < ScoreSetting::StopDistance &&
		fabs(targetY - currentY) < ScoreSetting::StopDistance)
	{
		currentY = targetY;
		velocity = 0.0f;

		return true;
	}

	// まだ目標位置に到達していない
	return false;
}


// コンストラクタ
Score::Score()
	: mScoreRank(ScoreRank::RANK_NONE)
	, mnScoreMax(-1)
	, mnScoreNormal(-1)
	, mnScoreLow(-1)
	, mnDrawScoreTime(0)
	, mfTurnMoveY(ScoreSetting::TurnMoveYInitialValue)
	, mfUseCardMoveY(ScoreSetting::UseCardMoveYInitialValue)
	, mfUseItemMoveY(ScoreSetting::UseItemMoveYInitialValue)
	, mbInitialize(false)
	, mbMove(false)
	, mfMoveY(ScoreSetting::MoveYInitialValue)
	, mfTurnBox(ScoreSetting::TurnBoxInitialValue)
	, mfHpVelocity(0.0f)
	, mfTurnVelocity(0.0f)
	, mfUseCardVelocity(0.0f)
	, mfUseItemVelocity(0.0f)
	, mfTurnBoxVelocity(0.0f)
{
	LoadScoreImages();

	mbInitialize = true;
}


// デストラクタ
Score::~Score()
{
	DeleteGraph(mnScoreMax);
	DeleteGraph(mnScoreNormal);
	DeleteGraph(mnScoreLow);
}


// スコア画像を読み込む
void Score::LoadScoreImages()
{
	// 花丸の画像
	mnScoreMax = LoadGraph(ResourcePath::ScoreMax);

	if (mnScoreMax == -1)
	{
		printfDx("スコア画像がありません");
	}

	// にじゅうまるの画像
	mnScoreNormal = LoadGraph(ResourcePath::ScoreNormal);

	if (mnScoreNormal == -1)
	{
		printfDx("スコア画像がありません");
	}

	// ただのまるの画像
	mnScoreLow = LoadGraph(ResourcePath::ScoreLow);

	if (mnScoreLow == -1)
	{
		printfDx("スコア画像がありません");
	}
}


// 更新
void Score::Update()
{
	if (!mbInitialize)
	{
		return;
	}

	mnDrawScoreTime++;

	// 後ろの画像を移動させる
	MoveScoreY(
		mfTurnBox,
		ScoreSetting::ScoreTargetY,
		mfTurnBoxVelocity
	);

	// HPの文字を動かす
	if (mnDrawScoreTime >= ScoreSetting::HpScoreStartTime)
	{
		DrawHpScoreString();
	}

	// 経過ターン数の文字を動かす
	if (mnDrawScoreTime >= ScoreSetting::TurnScoreStartTime)
	{
		DrawTurnScoreString();
	}


	// 使用カード枚数の文字を動かす
	if (mnDrawScoreTime >= ScoreSetting::UseCardScoreStartTime)
	{
		DrawUseCardScoreString();
	}


	// 使用アイテム数の文字を動かす
	if (mnDrawScoreTime >= ScoreSetting::UseItemScoreStartTime)
	{
		DrawUseItemScoreString();
	}
}


// 描画
void Score::Draw()
{
	DrawScoreStrings();
	CalculateScoreRank();
	DrawScoreRank();
}


// スコアの文字を全部描画する
void Score::DrawScoreStrings()
{
	DrawScoreString(
		"残りHP",
		Master::mpSaveHp,
		(int)mfMoveY
	);

	DrawScoreString(
		"経過ターン数",
		Master::mpTurnCount,
		(int)mfTurnMoveY
	);

	DrawScoreString(
		"使用カード枚数",
		Master::mpSaveCardCount,
		(int)mfUseCardMoveY
	);

	DrawScoreString(
		"使用アイテム数",
		Master::mpSaveItemCount,
		(int)mfUseItemMoveY
	);
}


// スコアの文字を1つ描画する
void Score::DrawScoreString(
	const char* text,
	int value,
	int y)
{
	int x = ScoreSetting::ScoreX;
	int scoreX = x + ScoreSetting::ScoreNumberXOffset;

	int textSize = ScoreSetting::TextSize;
	unsigned int color = ColorOption::DarkGray;

	FontManager* fontManager =
		Master::mpGameManager->GetFontManager();


	// 項目名
	fontManager->FontManagerDrawString(
		FontManager::FontType::Nikumaru,
		x,
		y,
		textSize,
		color,
		text
	);


	// 数値
	fontManager->FontManagerDrawString(
		FontManager::FontType::Nikumaru,
		scoreX,
		y,
		textSize,
		color,
		"%d",
		value
	);
}


// HPのスコアを計算する
int Score::CalculateHpScore(int hp)
{
	if (hp >= ScoreSetting::HpScoreMax)
	{
		return ScoreSetting::ScorePointMax;
	}

	if (hp >= ScoreSetting::HpScoreNormal)
	{
		return ScoreSetting::ScorePointNormal;
	}

	return ScoreSetting::ScorePointLow;
}


// ターン数のスコアを計算する
int Score::CalculateTurnScore(int turn)
{
	if (turn <= ScoreSetting::TurnScoreMax)
	{
		return ScoreSetting::ScorePointMax;
	}

	if (turn <= ScoreSetting::TurnScoreNormal)
	{
		return ScoreSetting::ScorePointNormal;
	}

	return ScoreSetting::ScorePointLow;
}


// 総合ランクを計算する
void Score::CalculateScoreRank()
{
	int hpScore = CalculateHpScore(Master::mpSaveHp);
	int turnScore = CalculateTurnScore(Master::mpTurnCount);

	if (hpScore == ScoreSetting::ScorePointMax
		&& turnScore == ScoreSetting::ScorePointMax)
	{
		mScoreRank = ScoreRank::RANK_MAX;
	}
	else if (hpScore >= ScoreSetting::ScorePointNormal
		&& turnScore >= ScoreSetting::ScorePointNormal)
	{
		mScoreRank = ScoreRank::RANK_NORMAL;
	}
	else
	{
		mScoreRank = ScoreRank::RANK_LOW;
	}
}


// ランクに対応した画像ハンドルを取得する
int Score::GetScoreRankHandle()
{
	switch (mScoreRank)
	{
	case ScoreRank::RANK_MAX:
		return mnScoreMax;

	case ScoreRank::RANK_NORMAL:
		return mnScoreNormal;

	case ScoreRank::RANK_LOW:
		return mnScoreLow;

	default:
		return -1;
	}
}


// ランク画像を描画する
void Score::DrawScoreRank()
{
	if (!mbMove)
	{
		return;
	}

	int handle = GetScoreRankHandle();

	if (handle == -1)
	{
		return;
	}

	DrawRotaGraph(
		ScreenSize::CenterX,
		ScoreSetting::RankImageY,
		ScoreSetting::RankImageScale,
		0.0f,
		handle,
		TRUE
	);
}


// HPの残数の文字を動かす
void Score::DrawHpScoreString()
{
	MoveScoreY(
		mfMoveY,
		ScoreSetting::ScoreTargetY,
		mfHpVelocity
	);
}


// 経過ターン数の文字を動かす
void Score::DrawTurnScoreString()
{
	float targetY =
		mfMoveY + ScoreSetting::TurnScoreYOffset;

	MoveScoreY(
		mfTurnMoveY,
		targetY,
		mfTurnVelocity
	);
}


// 使用カード枚数の文字を動かす
void Score::DrawUseCardScoreString()
{
	float targetY =
		mfMoveY + ScoreSetting::UseCardScoreYOffset;

	MoveScoreY(
		mfUseCardMoveY,
		targetY,
		mfUseCardVelocity
	);
}


// 使用アイテム数の文字を動かす
void Score::DrawUseItemScoreString()
{
	float targetY =
		mfMoveY + ScoreSetting::UseItemScoreYOffset;

	if (MoveScoreY(
		mfUseItemMoveY,
		targetY,
		mfUseItemVelocity))
	{
		mbMove = true;
	}
}