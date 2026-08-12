#include "Score.h"
#include "Master.h"
#include "Utility.h"

Score::Score()
{
	// 花丸の画像
	mnScoreMax = LoadGraph("Resource/Title/ScoreMax.png");
	if (mnScoreMax == -1)
	{
		printfDx("MP画像ない");
	}

	// にじゅうまるの画像
	mnScoreNormal = LoadGraph("Resource/Title/ScoreNormal.png");
	if (mnScoreNormal == -1)
	{
		printfDx("MP画像ない");
	}

	// ただのまるの画像
	mnScoreLow = LoadGraph("Resource/Title/ScoreLow.png");
	if (mnScoreLow == -1)
	{
		printfDx("MP画像ない");
	}

}

Score::~Score()
{
	DeleteGraph(mnHandle);
	DeleteGraph(mnScoreMax);
	DeleteGraph(mnScoreNormal);
	DeleteGraph(mnScoreLow);
}

void Score::Initialize()
{
	mScoreRank = ScoreRank::RANK_NONOE; // ランクなし　初期化

	mbInitialize = true; // 初期化終了フラグ
}

void Score::Update()
{
	// ゲームの初期化が終わっていたら
	if (mbInitialize)
	{
		//---追加・変更したやつ---//

		DrawScoreTime++; // スコアの描画をする時間を増やす

		//---リザルトに移動したら勝利と敗北の文字を時間関係なしに表示させるために追加---//
		static float velocity = 0.0f; // 今の速度
		float targetY = 520.0f; // 目的地Y

		// 調整パラメータ
		float gravity = 0.1f;      // 落ちる速さ 重力
		float power = 0.25f;	   // バネの強さ
		float damping = 0.70f;     // 減衰

		// 重力で下に引っ張る
		velocity += gravity;

		// バネ
		float force = (targetY - mfTurnBox) * power;
		velocity += force;

		// 減衰
		velocity *= damping;

		// 移動させてる
		mfTurnBox += velocity; // 後ろの画像も一緒に


		// ほぼ目的地についたら止める
		if (fabs(velocity) < 0.6f && fabs(targetY - mfTurnBox) < 0.6f)
		{
			mfTurnBox = targetY;
			mfTurnBox = targetY;
			velocity = 0.0f;
			//mbMove = true; // 文字の動きが終わったフラグON  ・文字の動きが終わったフラグは使用アイテム数の文字が描画し終わったときにONにするように変更した
		}
		//----------------//


		// 文字が指定した時間になったら動かす
		if (DrawScoreTime >= 60)
		{
			// HPの残数の文字を動かす関数 ・Update関数から移動させた
			DrawHpScoreString();
		}
		if (DrawScoreTime >= 120)
		{
			// 経過ターン数の文字を動かす関数
			DrawTurnScoreString();
		}
		if (DrawScoreTime >= 180)
		{
			// 使用カード枚数の文字を動かす関数
			DrawUseCardScoreString();
		}
		if (DrawScoreTime >= 240)
		{
			// 使用アイテム数の文字を動かす関数
			DrawUseItemScoreString();
		}

		//---ここまで追加・変更---//

	}

}

void Score::Draw()
{
	int hp = Master::mpSaveHp; // HPの残数
	int turn = Master::mpTurnCount; // ターンの回数


	unsigned int color = GetColor(60, 60, 60); // 黒色

	int x = 650; // Xの位置
	int scoreX = x + 600; // スコアのXの位置
	int y = (int)mfMoveY; // Yの位置
	int y1 = (int)mfTurnMoveY; // 経過ターン数のYの位置 y + 60
	int y2 = (int)mfUseCardMoveY; // 経過ターン数のYの位置 y + 150
	int y3 = (int)mfUseItemMoveY; // 経過ターン数のYの位置 y + 210


	// スコアの描画
	// 残りHP
	DrawFormatStringToHandle(x, y, color,Master::mpFontManager->GetScoreFontHandle(),"残りHP");
	DrawFormatStringToHandle(scoreX, y, color,Master::mpFontManager->GetScoreFontHandle(),"%d", hp);

	// 経過ターン数
	DrawFormatStringToHandle(x, y1, color,Master::mpFontManager->GetScoreFontHandle(),"経過ターン数");
	DrawFormatStringToHandle(scoreX, y1, color,Master::mpFontManager->GetScoreFontHandle(),"%d", turn);

	// 使用カード枚数
	DrawFormatStringToHandle(x, y2, color,Master::mpFontManager->GetScoreFontHandle(),"使用カード枚数");
	DrawFormatStringToHandle(scoreX, y2, color,Master::mpFontManager->GetScoreFontHandle(),"%d", Master::mpSaveCardCount);

	// 使用アイテム数
	DrawFormatStringToHandle(x, y3, color,Master::mpFontManager->GetScoreFontHandle(),"使用アイテム数");
	DrawFormatStringToHandle(scoreX, y3, color,Master::mpFontManager->GetScoreFontHandle(),"%d", Master::mpSaveItemCount);


	// 三段階で評価するための変数
	int ScoreHp = 0;
	int ScoreTurn = 0;


	// HPの判定
	// 三段階で評価している
	if (hp >= 75)
	{
		ScoreHp = 3;
	}
	else if (hp >= 40)
	{
		ScoreHp = 2;
	}
	else
	{
		ScoreHp = 1;
	}

	// ターン数の評価
	if (turn <= 8)
	{
		ScoreTurn = 3;
	}
	else if (turn <= 16)
	{
		ScoreTurn = 2;
	}
	else
	{
		ScoreTurn = 1;
	}


	// スコアの合計
	if (ScoreHp == 3 && ScoreTurn == 3)
	{
		mScoreRank = ScoreRank::RANK_MAX;
	}
	else if (ScoreHp >= 2 && ScoreTurn >= 2)
	{
		mScoreRank = ScoreRank::RANK_NORMAL;
	}
	else
	{
		// 負けたら丸になる プレイヤーのHPが0になるから
		mScoreRank = ScoreRank::RANK_LOW;
	}


	// ランクによって表示するものを変える
	switch (mScoreRank)
	{
	case ScoreRank::RANK_MAX:
		mnHandle = mnScoreMax; // ハンドルの代入
		break;

	case ScoreRank::RANK_NORMAL:
		mnHandle = mnScoreNormal; // ハンドルの代入
		break;

	case ScoreRank::RANK_LOW:
		mnHandle = mnScoreLow; // ハンドルの代入
		break;

	default:
		break;
	}


	// 文字が位置についたら表示する
	if (mbMove)
	{
		// まるの描画
		DrawRotaGraph(Utility::SCREEN_WIDTH / 2,  400,  0.2f, 0.0f, mnHandle, TRUE);
	}
}


//----追加した関数-----//
// HPの残数の文字を動かす関数 ・Update関数から移動させた
void Score::DrawHpScoreString()
{
	static float velocity = 0.0f; // 今の速度
	float targetY = 520.0f; // 目的地Y

	// 調整パラメータ
	float gravity = 0.1f;      // 落ちる速さ 重力
	float power = 0.25f;	   // バネの強さ
	float damping = 0.70f;     // 減衰

	// 重力で下に引っ張る
	velocity += gravity;

	// バネ
	float force = (targetY - mfMoveY) * power;
	velocity += force;

	// 減衰
	velocity *= damping;

	// 移動させてる
	mfMoveY += velocity;
	//mfTurnBox += velocity; // 後ろの画像も一緒に


	// ほぼ目的地についたら止める
	if (fabs(velocity) < 0.6f && fabs(targetY - mfMoveY) < 0.6f)
	{
		//mfTurnBox = targetY;
		//mfTurnBox = targetY;
		mfMoveY = targetY;
		velocity = 0.0f;
		//mbMove = true; // 文字の動きが終わったフラグON  ・文字の動きが終わったフラグは使用アイテム数の文字が描画し終わったときにONにするように変更した
	}

}


//----追加した関数-----//
// 経過ターン数の文字を動かす関数
void Score::DrawTurnScoreString()
{
	static float velocity = 0.0f; // 今の速度
	float targetY = mfMoveY + 60; // 目的地Y

	// 調整パラメータ
	float gravity = 0.1f;      // 落ちる速さ 重力
	float power = 0.25f;	   // バネの強さ
	float damping = 0.70f;     // 減衰


	// 重力で下に引っ張る
	velocity += gravity;

	// バネ
	float force = (targetY - mfTurnMoveY) * power;
	velocity += force;

	// 減衰
	velocity *= damping;

	// 移動させてる
	mfTurnMoveY += velocity;


	// ほぼ目的地についたら止める
	if (fabs(velocity) < 0.6f && fabs(targetY - mfTurnMoveY) < 0.6f)
	{
		mfTurnMoveY = targetY;
		velocity = 0.0f;
	}

}

//----追加した関数----//
// 使用カード枚数の文字を動かす関数
void Score::DrawUseCardScoreString()
{
	static float velocity = 0.0f; // 今の速度
	float targetY = mfMoveY + 150; // 目的地Y

	// 調整パラメータ
	float gravity = 0.1f;      // 落ちる速さ 重力
	float power = 0.25f;	   // バネの強さ
	float damping = 0.70f;     // 減衰

	// 重力で下に引っ張る
	velocity += gravity;

	// バネ
	float force = (targetY - mfUseCardMoveY) * power;
	velocity += force;

	// 減衰
	velocity *= damping;

	// 移動させてる
	mfUseCardMoveY += velocity;


	// ほぼ目的地についたら止める
	if (fabs(velocity) < 0.6f && fabs(targetY - mfUseCardMoveY) < 0.6f)
	{
		mfUseCardMoveY = targetY;
		velocity = 0.0f;
	}


}

//----追加した関数-----//
// 使用アイテム数の文字を動かす関数
void Score::DrawUseItemScoreString()
{
	// ゲームの初期化が終わっていたら
	if (mbInitialize)
	{
		static float velocity = 0.0f; // 今の速度
		float targetY = mfMoveY + 210; // 目的地Y

		// 調整パラメータ
		float gravity = 0.1f;      // 落ちる速さ 重力
		float power = 0.25f;	   // バネの強さ
		float damping = 0.70f;     // 減衰

		// 重力で下に引っ張る
		velocity += gravity;

		// バネ
		float force = (targetY - mfUseItemMoveY) * power;
		velocity += force;

		// 減衰
		velocity *= damping;

		// 移動させてる
		mfUseItemMoveY += velocity;


		// ほぼ目的地についたら止める
		if (fabs(velocity) < 0.6f && fabs(targetY - mfUseItemMoveY) < 0.6f)
		{
			mfUseItemMoveY = targetY;
			velocity = 0.0f;
			mbMove = true; // 文字の動きが終わったフラグON
		}

	}

}
