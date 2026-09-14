#include "InputManager.h"
#include "Master.h"
#include "Title.h"
#include "Utility.h"
#include "GameConstants.h"
#include "VolumeBar.h"


Title::Title()
    : targetX(ScreenSize::CenterX + TitlePosition::CardTargetOffsetX)
    , targetY(ScreenSize::CenterY + TitlePosition::CardTargetOffsetY)
{
	mpGameStart = nullptr;
	mpExplainGraph = nullptr;
	mpOptionButton = nullptr;
	mpMusicClose = nullptr;
	mpBgmVolumeBar = nullptr;
	mpSeVolumeBar = nullptr;

	// 演出用変数の初期値を設定
	mnCardX = TitleAnimation::CardInitialX;
	mnCardY = TitleAnimation::CardInitialY;

	int x = ScreenSize::CenterX;
	int y = ScreenSize::CenterY;

	// Start 左下から
	mfStartX = TitlePosition::StartInitialX;
	mfStartY = (float)ScreenSize::Height + TitlePosition::ButtonInitialYOffset;

	// Explain 右下から
	mfExplainX = (float)ScreenSize::Width + TitlePosition::ExplainInitialX;
	mfExplainY = (float)ScreenSize::Height + TitlePosition::ButtonInitialYOffset;

	// 最終位置
	mfTargetStartX = (float)x + TitlePosition::StartTargetOffsetX;
	mfTargetStartY = (float)y + TitlePosition::StartTargetOffsetY;
	mfTargetExplainX = (float)x + TitlePosition::ExplainTargetOffsetX;
	mfTargetExplainY = (float)y + TitlePosition::ExplainTargetOffsetY;

	// フラグや変数の初期化
	mfTurnY = TitleAnimation::LogoInitialY;
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
	// ボタン生成（初期位置）
	mpGameStart = std::make_unique<MouseGraph>(
		mfStartX,
		mfStartY,
		0.0f,
		TitleResourcePath::GameStartButton,
		TitleScale::StartNormal,
		TitleScale::StartHover
	);

	mpExplainGraph = std::make_unique<MouseGraph>(
		mfExplainX,
		mfExplainY,
		0.0f,
		TitleResourcePath::ExplainButton,
		TitleScale::ExplainNormal,
		TitleScale::ExplainHover
	);

	mnRogoHandle = LoadGraph(TitleResourcePath::Rogo.c_str());
	if (mnRogoHandle == -1) { printfDx("ロゴ画像ない"); }
	mnBagHandle = LoadGraph(TitleResourcePath::Bagground.c_str());
	if (mnBagHandle == -1) { printfDx("背景画像ない"); }
	mnCardHandle = LoadGraph(TitleResourcePath::MoveBagGround.c_str());
	if (mnCardHandle == -1) { printfDx("動く黒板の画像がない"); }
	mnVolumeSettingsBg = LoadGraph(TitleResourcePath::VolumeSettingsBg.c_str());
	if (mnVolumeSettingsBg == -1) { printfDx("音量設定の背景画像がない"); }
	mnOnpuHandle = LoadGraph(TitleResourcePath::MusicNote.c_str()); // 音符の画像
	if (mnOnpuHandle == -1) { printfDx("音符の画像がない"); }


	// 中心座標XとY　角度　画像　画像の拡大率　変えるときの拡大率
	mpOptionButton = std::make_unique<MouseGraph>(
		TitlePosition::OptionButtonX,
		TitlePosition::OptionButtonY,
		0.0f,
		TitleResourcePath::OptionButton.c_str(),
		TitleScale::OptionNormal,
		TitleScale::OptionHover
	);
	mpMusicClose = std::make_unique<MouseGraph>(
		TitlePosition::MusicCloseX,
		TitlePosition::MusicCloseY,
		0.0f,
		TitleResourcePath::MusicClose.c_str(),
		TitleScale::MusicCloseNormal,
		TitleScale::MusicCloseHover
	);

	Master::mpGameManager->GetSoundManager()->PlayBGM(SoundManager::BgmTitle);


	// 音量バーの生成
	mpBgmVolumeBar = std::make_unique<VolumeBar>(
		TitlePosition::VolumeBarX,
		TitlePosition::BgmBarY,
		TitlePosition::VolumeBarWidth,
		TitlePosition::VolumeBarHeight,
		TitleColor::BgmBar
	);
	mpSeVolumeBar = std::make_unique<VolumeBar>(
		TitlePosition::VolumeBarX,
		TitlePosition::SeBarY,
		TitlePosition::VolumeBarWidth,
		TitlePosition::VolumeBarHeight,
		TitleColor::SeBar
	);

	mbInitialize = true; // 初期化終わりON
}


void Title::Update()
{
	// タイトル演出（飛んでくる演出のもの）
	mfStartX += (mfTargetStartX - mfStartX) * TitleAnimation::ButtonMoveSpeed;
	mfStartY += (mfTargetStartY - mfStartY) * TitleAnimation::ButtonMoveSpeed;
	mfExplainX += (mfTargetExplainX - mfExplainX) * TitleAnimation::ButtonMoveSpeed;
	mfExplainY += (mfTargetExplainY - mfExplainY) * TitleAnimation::ButtonMoveSpeed;

	// ボタン座標更新
	if (mpGameStart)
	{
		mpGameStart->SetPosition(mfStartX, mfStartY);


		if (fabs(mfStartX - mfTargetStartX) < TitleAnimation::ButtonStopDistance &&
			fabs(mfStartY - mfTargetStartY) < TitleAnimation::ButtonStopDistance)
		{
			mfStartX = mfTargetStartX;
			mfStartY = mfTargetStartY;
		}
	}

	if (mpExplainGraph)
	{

		if (fabs(mfExplainX - mfTargetExplainX) < TitleAnimation::ButtonStopDistance &&
			fabs(mfExplainY - mfTargetExplainY) < TitleAnimation::ButtonStopDistance)
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


		mfLogoTime += TitleAnimation::LogoMoveSpeed;
		float baseY =
			(Utility::SCREEN_HEIGHT / 2) + TitlePosition::LogoBaseYOffset;
		float targetY =
			baseY + sinf(mfLogoTime) * TitleAnimation::LogoSwingSize;
		float gravity = TitleAnimation::Gravity;
		float power = TitleAnimation::SpringPower;
		float damping = TitleAnimation::Damping;

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
		if (fabs(velocity) < TitleAnimation::VelocityStop &&
			fabs(targetY - mfTurnY) < TitleAnimation::TargetStopDistance)
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
		if (mpOptionButton)
		{
			mpOptionButton->Update();

			// もし音量画面が開かれていたら押せないように
			if (mbOption)
			{
				mpOptionButton->SetActive(false);
			}
			else
			{
				mpOptionButton->SetActive(true); // 開かれていなかったら押せる
			}

			if (mpOptionButton->IsClicked() && !mbOption)
			{
				Master::mpGameManager->GetSoundManager()->PlaySE(SoundManager::SE_DECIDE);

				mbOption = true;
			}
		}

		if (mpGameStart->IsClicked() && !mbOption) // 押されたらゲーム画面にいく
		{
			Master::mpGameManager->GetSoundManager()->PlaySE(SoundManager::SE_DECIDE);
			//Master::mpGameManager->GetSoundManager()->StartFadeOut();
			mbMouseButton = true; // フラグをTRUE
			mNextScene = SELECT_SCENE; // ゲーム画面に飛ぶのを設定
		}
		else if (mpExplainGraph->IsClicked() && !mbOption)
		{
			Master::mpGameManager->GetSoundManager()->PlaySE(SoundManager::SE_DECIDE);
			//Master::mpGameManager->GetSoundManager()->StartFadeOut();
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
				Master::mpGameManager->GetSoundManager()->PlaySE(SoundManager::SE_DECIDE);

				mbOption = false;
			}
		}
	}


	// マウスが押されたら
	if (mbMouseButton)
	{
		float diffX = (float)targetX - mnCardX;
		float diffY = (float)targetY - mnCardY;
		float diffAngle = (float)targetAngle - mnCardAngle;
		float diffRota = (float)targetRota - mnCardRota;

		// 移動処理
		mnCardX += diffX * TitleAnimation::CardMoveSpeed;
		mnCardY += diffY * TitleAnimation::CardMoveSpeed;
		mnCardAngle += diffAngle * TitleAnimation::CardMoveSpeed;
		mnCardRota += diffRota * TitleAnimation::CardMoveSpeed;

		if (mnCardAngle <= 0.0f) mnCardAngle = 0.0f;

		if (fabsf(diffX) < TitleAnimation::CardStopDistanceX &&
			fabsf(diffY) < TitleAnimation::CardStopDistanceY &&
			fabsf(diffRota) < TitleAnimation::CardStopDistanceRota)
		{
			targetRota += TitleAnimation::CardRotaIncrease;

			if (targetRota >= TitleAnimation::CardRotaMax)
			{
				targetRota = TitleAnimation::CardRotaMax;
				mbWhite = true;// 白いBOXの描画をON
			}
		}

		if (mbWhite &&
			mfWhiteBoxAlpha > TitleAnimation::WhiteBoxSceneChangeAlpha)
		{
			// タイプによって分ける
			if (mNextScene == SELECT_SCENE)
			{
				Master::Master::mpGameManager->GetSceneManager()->SetNextScene(SceneManager::SELECT_SCENE);
			}
			if (mNextScene == EXPLAIN_SCENE) 
			{
				Master::Master::mpGameManager->GetSceneManager()->SetNextScene(SceneManager::EXPAIN_SCENE);
			}
			return;
		}
	}

	// 透過度を増やすやつ
	if (mbWhite)
	{
		mfWhiteBoxAlpha += TitleAnimation::WhiteBoxAlphaIncrease;
		if (mfWhiteBoxAlpha > TitleAnimation::WhiteBoxMaxAlpha)
		{
			mfWhiteBoxAlpha = TitleAnimation::WhiteBoxMaxAlpha;
		}
	}

	if (!mbMouseButton && mbOption)
	{
		int mouseX, mouseY;

		GetMousePoint(&mouseX, &mouseY);

		if (GetMouseInput() & MOUSE_INPUT_LEFT)
		{
			if (mouseX >= TitlePosition::VolumeBarX &&
				mouseX <= TitlePosition::VolumeBarX
				+ TitlePosition::VolumeBarWidth
				+ TitlePosition::VolumeBarDist &&
				mouseY >= TitlePosition::BgmBarY
				- TitlePosition::VolumeBarUp &&
				mouseY <= TitlePosition::BgmBarY
				+ TitlePosition::VolumeBarDown)
			{
				int bgmVolume =
					(mouseX - TitlePosition::VolumeBarX)
					* SoundSetting::VolumeMax
					/ TitlePosition::VolumeBarWidth;

				Master::mpGameManager->GetSoundManager()->SetBGMVolume(bgmVolume);
				Master::mpGameManager->GetSoundManager()->SaveVolume();
			}

			// =========================
			// SEバー
			// =========================
			if (mouseX >= TitlePosition::VolumeBarX &&
				mouseX <= TitlePosition::VolumeBarX
				+ TitlePosition::VolumeBarWidth
				+ TitlePosition::VolumeBarDist &&
				mouseY >= TitlePosition::SeBarY
				- TitlePosition::VolumeBarUp &&
				mouseY <= TitlePosition::SeBarY
				+ TitlePosition::VolumeBarDown)
			{
				int seVolume =
					(mouseX - TitlePosition::VolumeBarX)
					* SoundSetting::VolumeMax
					/ TitlePosition::VolumeBarWidth;

				Master::mpGameManager->GetSoundManager()->SetSEVolume(seVolume);
				Master::mpGameManager->GetSoundManager()->SaveVolume();
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
	int color = TitleColor::White;


	// 2D用に設定
	SetUseZBufferFlag(FALSE);
	SetWriteZBufferFlag(FALSE);
	
	// 背景描画
	DrawRotaGraph(
		x + TitleSetting::BackgroundOffsetX,
		y,
		TitleScale::Background,
		0.0f,
		mnBagHandle,
		TRUE
	);

	// ロゴ描画
	DrawRotaGraph(
		x + TitleSetting::LogoOffsetX,
		(int)mfTurnY,
		TitleScale::Logo,
		0.0f,
		mnRogoHandle,
		TRUE
	);

	if (mpGameStart && mpExplainGraph)
	{
		mpGameStart->Draw();
		mpExplainGraph->Draw();
	}

	if (mpOptionButton)
	{
		mpOptionButton->Draw();
	}

	// ゲーム画面に行く時の演出の画像
	if (mbMouseButton)
	{
		// 黒板イラストの描画
		DrawRotaGraph(
			ScreenSize::CenterX,
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
			DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, TitleColor::White, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	// 音量調整のやつがONなら描画
	if (mbOption)
	{
		// 背景暗くする
		SetDrawBlendMode(
			DX_BLENDMODE_ALPHA,
			TitleSetting::VolumeBackgroundAlpha
		);
		//黒いBOX
		DrawBox(
			0,
			0,
			Utility::SCREEN_WIDTH,
			Utility::SCREEN_HEIGHT,
			TitleColor::Black,
			TRUE
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 戻す


		// 音量設定の背景の黒板
		DrawRotaGraph(
			Utility::SCREEN_WIDTH / 2,
			Utility::SCREEN_HEIGHT / 2,
			TitleScale::VolumeSettingsBackground,
			0.0f,
			mnVolumeSettingsBg,
			TRUE
		);
		DrawStringToHandle(
			TitlePosition::VolumeTitleX,
			TitlePosition::VolumeTitleY,
			"音量設定",
			TitleColor::White,  Master::mpGameManager->GetFontManager()->GetSelectFontHandle());


		// BGM関係
		{
			// BGM文字
			DrawStringToHandle(
				TitlePosition::BgmTextX,
				TitlePosition::BgmTextY,
				"BGM",
				TitleColor::White,
				Master::mpGameManager->GetFontManager()->GetTextFontHandle()
			);

			int bgmVolume = Master::mpGameManager->GetSoundManager()->GetBGMVolume();

			// BGMバー
			mpBgmVolumeBar->Draw(bgmVolume);

			// BGM音量の数字
			DrawFormatStringToHandle(
				TitlePosition::VolumeBarX
				+ (bgmVolume * TitlePosition::VolumeBarWidth
					/ TitleSetting::VolumeMax)
				+ TitlePosition::VolumeNumberXOffset,

				TitlePosition::BgmBarY
				- TitlePosition::VolumeNumberYOffset,

				TitleColor::White,

				Master::mpGameManager->GetFontManager()->GetMusicFontHandle(),
				"%d",
				bgmVolume
			);

			// BGMつまみ
			DrawRotaGraph(
				TitlePosition::VolumeBarX
				+ (bgmVolume * TitlePosition::VolumeBarWidth
					/ TitleSetting::VolumeMax),

				TitlePosition::BgmBarY
				+ TitlePosition::MusicNoteOffset,

				TitleScale::MusicNote,
				0.0f,
				mnOnpuHandle,
				TRUE
			);
		}


		// SE関係
		{
			// SE文字
			DrawStringToHandle(
				TitlePosition::SeTextX,
				TitlePosition::SeTextY,
				"SE",
				TitleColor::White,
				Master::mpGameManager->GetFontManager()->GetTextFontHandle()
			);

			int seVolume = Master::mpGameManager->GetSoundManager()->GetSEVolume();

			// SEバー
			mpSeVolumeBar->Draw(seVolume);

			// SE音量の数字
			DrawFormatStringToHandle(
				TitlePosition::VolumeBarX
				+ (seVolume * TitlePosition::VolumeBarWidth
					/ TitleSetting::VolumeMax)
				+ TitlePosition::VolumeNumberXOffset,

				TitlePosition::SeBarY
				- TitlePosition::VolumeNumberYOffset,

				TitleColor::White,

				Master::mpGameManager->GetFontManager()->GetMusicFontHandle(),
				"%d",
				seVolume
			);

			// SEつまみ
			DrawRotaGraph(
				TitlePosition::VolumeBarX
				+ (seVolume * TitlePosition::VolumeBarWidth
					/ TitleSetting::VolumeMax),

				TitlePosition::SeBarY
				+ TitlePosition::MusicNoteOffset,

				TitleScale::MusicNote,
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
	
	// 画像ハンドルの削除
	if (mnRogoHandle != -1) { DeleteGraph(mnRogoHandle); mnRogoHandle = -1; }
	if (mnBagHandle != -1) { DeleteGraph(mnBagHandle);  mnBagHandle = -1; }
	if (mnCardHandle != -1) { DeleteGraph(mnCardHandle); mnCardHandle = -1; }
	if (mnOnpuHandle != -1) { DeleteGraph(mnOnpuHandle); mnOnpuHandle = -1; }
}