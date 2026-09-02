#include "InputManager.h"
#include "Master.h"
#include "Title.h"
#include "Utility.h"

Title::Title()
{
	mpGameStart = nullptr;
	mpExplainGraph = nullptr;
	mpGearButtom = nullptr;
	mpMusicClose = nullptr;

	// 演出用変数の初期値を設定
	mnCardX = -200.0f;
	mnCardY = -200.0f;

	int x = Utility::SCREEN_WIDTH / 2;
	int y = Utility::SCREEN_HEIGHT / 2;

	// Start 左下から
	mfStartX = -300.0f;
	mfStartY = (float)Utility::SCREEN_HEIGHT + 150.0f;

	// Explain 右下から
	mfExplainX = (float)Utility::SCREEN_WIDTH + 300.0f;
	mfExplainY = (float)Utility::SCREEN_HEIGHT + 150.0f;

	// 最終位置
	mfTargetStartX = (float)x - 320.0f;
	mfTargetStartY = (float)y + 200.0f;

	mfTargetExplainX = (float)x + 320.0f;
	mfTargetExplainY = (float)y + 200.0f;

	// フラグや変数の初期化
	mfTurnY = -800.0f;
	mbInitialize = false;
	mbOption = false;
	mbMouseButton = false;
	mbWhite = false;
	mfWhiteBoxAlpha = 0.0f;
	mfLogoTime = 0.0f;
}


Title::~Title()
{

}



void Title::Initialize()
{
	float x = (float)Utility::SCREEN_WIDTH / 2;
	float y = (float)Utility::SCREEN_HEIGHT / 2;

	//mfStartAngle = -2.5f;
	//mfExplainAngle = 2.5f;

	// ボタン生成（初期位置）
	mpGameStart = new MouseGraph(
		mfStartX,
		mfStartY,
		0.0f,
		"Resource/Title/Start.png",
		0.4f,
		0.45f
	);

	mpExplainGraph = new MouseGraph(
		mfExplainX,
		mfExplainY,
		0.0f,
		"Resource/Title/Explain.png",
		0.4f,
		0.45f
	);

	mnRogoHandle = LoadGraph("Resource/Title/Rogo.png");
	if (mnRogoHandle == -1) { printfDx("画像ない1"); }
	mnBagHandle = LoadGraph("Resource/Title/Title.png");
	if (mnBagHandle == -1) { printfDx("画像ない2"); }
	mnCardHandle = LoadGraph("Resource/Title/1.png");
	if (mnCardHandle == -1) { printfDx("画像ない3"); }

	mnMusicBag = LoadGraph("Resource/Title/Titleboard.png");
	if (mnMusicBag == -1) { printfDx("画像ない黒板"); }

	/*Master::mpSoundManager->PlayBGM(
		SoundManager::BGM_TITLE,
		true,
		92
	);*/

	mbInitialize = true; // 初期化終わりON


	// 中心座標XとY　角度　画像　画像の拡大率　変えるときの拡大率
	mpGameStart = new MouseGraph(x - 320, y + 200, 0.0f, "Resource/Title/Start.png", 0.4f, 0.45f); // スタートボタン
	mpExplainGraph = new MouseGraph(x + 320, y + 200, 0.0f, "Resource/Title/Explain.png", 0.4f, 0.45f); // スタートボタン

	// 中心座標XとY　角度　画像　画像の拡大率　変えるときの拡大率
	mpGearButtom = new MouseGraph(100.0f, 100.0f, 0.0f, "Resource/Title/option.png", 0.35f, 0.39f); // 音量調整のやつ
	mpMusicClose = new MouseGraph(1570.0f, 370.0f, 0.0f, "Resource/Title/MusicClose.png", 0.30f, 0.35f); // 音量調節の×ボタン

	mnOnpuHandle = LoadGraph("Resource/Title/MusicNote.png"); // 音符の画像
	Master::mpSoundManager->PlayBGM(SoundManager::BgmTitle);

	mBgmVolume = Master::mpSoundManager->GetBGMVolume();
	mSeVolume = Master::mpSoundManager->GetSEVolume();

	mbInitialize = true; // 初期化終わりON

}


void Title::Update()
{
	// タイトル演出（飛んでくる）
	// Start
	mfStartX += (mfTargetStartX - mfStartX) * 0.08f;
	mfStartY += (mfTargetStartY - mfStartY) * 0.08f;

	// Explain
	mfExplainX += (mfTargetExplainX - mfExplainX) * 0.08f;
	mfExplainY += (mfTargetExplainY - mfExplainY) * 0.08f;

	// ---------------------------
	// 回転演出
	// ---------------------------

	// ロゴ
	//mfLogoY += (mfTargetLogoY - mfLogoY) * 0.08f;

	// ボタン座標更新
	if (mpGameStart)
	{
		mpGameStart->SetPosition(mfStartX, mfStartY);


		if (fabs(mfStartX - mfTargetStartX) < 1.0f && fabs(mfStartY - mfTargetStartY) < 1.0f)
		{
			mfStartX = mfTargetStartX;
			mfStartY = mfTargetStartY;
		}

	}

	if (mpExplainGraph)
	{

		if (fabs(mfExplainX - mfTargetExplainX) < 1.0f && fabs(mfExplainY - mfTargetExplainY) < 1.0f)
		{
			mfExplainX = mfTargetExplainX;
			mfExplainY = mfTargetExplainY;
		}
		
		mpExplainGraph->SetPosition(mfExplainX,mfExplainY);
	}

	// 初期化が終わっていたら
	if (mbInitialize)
	{
		static float velocity = 0.0f; // 今の速度


		mfLogoTime += 0.05f;	// 揺れる速さ
		float baseY = (Utility::SCREEN_HEIGHT / 2) - 150; // 中心の位置
		float targetY = baseY + sinf(mfLogoTime) * 17.0f; // 揺れる大きさ   17から-17をふらふらゆらゆら

		//float targetY = (Utility::SCREEN_HEIGHT / 2) - 150; // 目的地Y

		// 調整パラメータ
		float gravity = 0.02f;     // 落ちる速さ 重力
		float power = 0.04f; // バネの強さ
		float damping = 0.87f;     // 減衰

		// 重力で下に引っ張る
		velocity += gravity;

		// バネ
		float force = (targetY - mfTurnY) * power;
		velocity += force;

		// 減衰
		velocity *= damping;

		// 移動させてる
		mfTurnY += velocity;

		// ほぼ目的地についたら止める
		if (fabs(velocity) < 0.3f && fabs(targetY - mfTurnY) < 0.3f)
		{
			mfTurnY = targetY;
			velocity = 0.0f;
		}
	}


	// ゲームボタンのUpdateと判定  マウスがまだ押されていなかったら判定
	// 音量調節中は押せないように追加  大谷
	if (mpGameStart && mpExplainGraph && !mbMouseButton)
	{
		mpGameStart->Update();
		mpExplainGraph->Update();

		// 設定歯車ボタンが押されたらフラグをON
		if (mpGearButtom)
		{
			mpGearButtom->Update();

			// もし音量画面が開かれていたら押せないように
			if (mbOption)
			{
				mpGearButtom->SetActive(false);
			}
			else
			{
				mpGearButtom->SetActive(true); // 開かれていなかったら押せる
			}

			if (mpGearButtom->IsClicked() && !mbOption)
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

				mbOption = true;
			}
		}

		if (mpGameStart->IsClicked() && !mbOption) // 押されたらゲーム画面にいく
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			//Master::mpSoundManager->StartFadeOut();
			mbMouseButton = true; // フラグをTRUE
			mNextScene = SELECT_SCENE; // ゲーム画面に飛ぶのを設定
		}
		else if (mpExplainGraph->IsClicked() && !mbOption)
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			//Master::mpSoundManager->StartFadeOut();
			mbMouseButton = true;
			mNextScene = EXPLAIN_SCENE; // 説明画面に飛ぶのを設定
		}

		// 音量×ボタンのUPD
		if (mpMusicClose)
		{
			mpMusicClose->Update();
			// 押されたらSEとフラグOFF
			if (mpMusicClose->IsClicked())
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

				mbOption = false;
			}
		}
	}


	//if (fabs(mfStartX - mfTargetStartX) < 1.0f)
	//{
	//	mfStartX = mfTargetStartX;
	//}
	


	// マウスが押されたら
	if (mbMouseButton)
	{
		float diffX = (float)targetX - mnCardX;
		float diffY = (float)targetY - mnCardY;
		float diffAngle = (float)targetAngle - mnCardAngle;
		float diffRota = (float)targetRota - mnCardRota;

		// 移動処理
		mnCardX += diffX * mfCardSpeed;
		mnCardY += diffY * mfCardSpeed;
		mnCardAngle += diffAngle * mfCardSpeed;
		mnCardRota += diffRota * mfCardSpeed;

		if (mnCardAngle <= 0.0f) mnCardAngle = 0.0f;

		if (fabsf(diffX) < 14.0f && fabsf(diffY) < 14.0f && fabsf(diffRota) < 6.0f)
		{
			// 少しずつ増やしていく
			targetRota += 0.1f;
			if (targetRota >= 2.0f)
			{
				targetRota = 2.0f;
				mbWhite = true;// 白いBOXの描画をON
			}
		}

		if (mbWhite && mfWhiteBoxAlpha > 254)
		{
			// タイプによって分ける
			if (mNextScene == SELECT_SCENE)
			{
				Master::mpSceneManager->SetNextScene(SceneManager::SELECT_SCENE);
			}
			if (mNextScene == EXPLAIN_SCENE) 
			{
				Master::mpSceneManager->SetNextScene(SceneManager::EXPAIN_SCENE);
			}
			return;
		}
	}

	// 透過度を増やすやつ
	if (mbWhite)
	{
		mfWhiteBoxAlpha += 4.0f;
		if (mfWhiteBoxAlpha > 255.0f)
		{
			mfWhiteBoxAlpha = 255.0f;
		}
	}

	if (!mbMouseButton && mbOption)
	{
		int mouseX, mouseY;

		GetMousePoint(&mouseX, &mouseY);

		if (GetMouseInput() & MOUSE_INPUT_LEFT)
		{
			// =========================
			// BGMバー
			// =========================
			if (mouseX >= mBarX &&
				mouseX <= mBarX + mBarWidth + mnBarDist &&
				mouseY >= mBgmBarY - mnBarUp &&
				mouseY <= mBgmBarY + mnBarDown)
			{
				mBgmVolume =
					(mouseX - mBarX) * 100 / mBarWidth;

				if (mBgmVolume < 0)
				{
					mBgmVolume = 0;
				}

				if (mBgmVolume > 100)
				{
					mBgmVolume = 100;
				}

				Master::mpSoundManager->SetBGMVolume(mBgmVolume);
				Master::mpSoundManager->SaveVolume();
			}

			// =========================
			// SEバー
			// =========================
			if (mouseX >= mBarX &&
				mouseX <= mBarX + mBarWidth + mnBarDist &&
				mouseY >= mSeBarY - mnBarUp &&
				mouseY <= mSeBarY + mnBarDown)
			{
				mSeVolume =
					(mouseX - mBarX) * 100 / mBarWidth;

				if (mSeVolume < 0)
				{
					mSeVolume = 0;
				}

				if (mSeVolume > 100)
				{
					mSeVolume = 100;
				}

				Master::mpSoundManager->SetSEVolume(mSeVolume);
				Master::mpSoundManager->SaveVolume();
			}

		
		}
	}

	Scene::Update();
}

void Title::Draw()
{
	if (!mbInitialize) return;

	// ロゴの位置
	int x = Utility::SCREEN_WIDTH / 2;
	int y = Utility::SCREEN_HEIGHT / 2;
	int dist = 4;
	int color = GetColor(255, 255, 255);


	// 2D用に設定
	SetUseZBufferFlag(FALSE);
	SetWriteZBufferFlag(FALSE);
	
	DrawRotaGraph(x - 50, y, 1.12f, 0.0f, mnBagHandle, TRUE);	   // 背景描画
	DrawRotaGraph(x, (int)mfTurnY, 0.45f, 0.0f, mnRogoHandle, TRUE); // ロゴ描画

	if (mpGameStart && mpExplainGraph)
	{
		mpGameStart->Draw();
		mpExplainGraph->Draw();
	}

	if (mpGearButtom)
	{
		mpGearButtom->Draw();
	}

	// ゲーム画面に行く時の演出の画像
	if (mbMouseButton)
	{
		// 黒板イラストの描画
		DrawRotaGraph(
			(int)mnCardX,
			(int)mnCardY,
			mnCardRota,
			mnCardAngle,
			mnCardHandle,
			TRUE
		);

		// 白いBOXがONなら描画
		if (mbWhite)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mfWhiteBoxAlpha); // 半透明にするため
			DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, GetColor(255, 255, 255), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	// 音量調整のやつがONなら描画
	if (mbOption)
	{
		int color = GetColor(255, 255, 255); // 白

		// 背景暗くする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
		//黒いBOX
		DrawBox(
			0,
			0,
			Utility::SCREEN_WIDTH,
			Utility::SCREEN_HEIGHT,
			GetColor(0, 0, 0),
			TRUE
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 戻す


		// 音量設定の背景の黒板
		DrawRotaGraph(Utility::SCREEN_WIDTH / 2, Utility::SCREEN_HEIGHT / 2, 1.0f, 0.0f, mnMusicBag, TRUE);

		// 文字
		DrawStringToHandle(800, 300, "音量設定", color, Master::mpFontManager->GetSelectFontHandle());

		// BGM関係
		{
			// BGM文字
			DrawStringToHandle(370, 450, "BGM", color, Master::mpFontManager->GetTextFontHandle());

			// BGMバー本体
			DrawBox(
				mBarX,
				mBgmBarY,
				mBarX + (mBgmVolume * mBarWidth / 100),
				mBgmBarY + mBarHeight,
				GetColor(238,136,214),
				TRUE
			);

			// BGMバー枠
			DrawBox(
				mBarX,
				mBgmBarY,
				mBarX + mBarWidth,
				mBgmBarY + mBarHeight,
				color,
				FALSE
			);

			// BGM音量の数字
			DrawFormatStringToHandle(
				mBarX + (mBgmVolume * mBarWidth / 100) - 20,
				mBgmBarY - mBarString,
				color,
				Master::mpFontManager->GetMusicFontHandle(),
				"%d",
				mBgmVolume
			);

			// BGMつまみ
			DrawRotaGraph(
				mBarX + (mBgmVolume * mBarWidth / 100),
				mBgmBarY + mMusicNote,
				0.3f,
				0.0f,
				mnOnpuHandle,
				TRUE
			);
		}
		

		// SE関係
		{
			// SE文字
			DrawStringToHandle(370, 640, "SE", color, Master::mpFontManager->GetTextFontHandle());

			// SEバー本体
			DrawBox(
				mBarX,
				mSeBarY,
				mBarX + (mSeVolume * mBarWidth / 100),
				mSeBarY + mBarHeight,
				GetColor(252, 235, 98),
				TRUE
			);

			// SEバー枠
			DrawBox(
				mBarX,
				mSeBarY,
				mBarX + mBarWidth,
				mSeBarY + mBarHeight,
				color,
				FALSE
			);

			// SE音量の数字
			DrawFormatStringToHandle(
				mBarX + (mSeVolume * mBarWidth / 100) - 20,
				mSeBarY - mBarString,
				color,
				Master::mpFontManager->GetMusicFontHandle(),
				"%d",
				mSeVolume
			);


			// SEつまみ
			DrawRotaGraph(
				mBarX + (mSeVolume * mBarWidth / 100),
				mSeBarY + mMusicNote,
				0.3f,
				0.0f,
				mnOnpuHandle,
				TRUE
			);
		}

		// 黒板の上の×ボタンの描画
		if (mpMusicClose)
		{
			mpMusicClose->Draw();
		}
	}

	Scene::Draw();
}

void Title::Finalize()
{
	if (mpGameStart)
	{
		delete mpGameStart;
		mpGameStart = nullptr;
	}

	if (mpExplainGraph)
	{
		delete mpExplainGraph;
		mpExplainGraph = nullptr;
	}

	if (mpGearButtom)
	{
		delete mpGearButtom;
		mpGearButtom = nullptr;
	}

	if (mpMusicClose)
	{
		delete mpMusicClose;
		mpMusicClose = nullptr;
	}

	// 画像ハンドルの削除
	if (mnRogoHandle != -1) { DeleteGraph(mnRogoHandle); mnRogoHandle = -1; }
	if (mnBagHandle != -1) { DeleteGraph(mnBagHandle);  mnBagHandle = -1; }
	if (mnCardHandle != -1) { DeleteGraph(mnCardHandle); mnCardHandle = -1; }
	if (mnOnpuHandle != -1) { DeleteGraph(mnOnpuHandle); mnOnpuHandle = -1; }


	//Master::mpSoundManager->StopBGM();
}