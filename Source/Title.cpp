#include "InputManager.h"
#include "Master.h"
#include "Title.h"
#include "Utility.h"
#include "GameConstants.h"
#include "VolumeBar.h"


Title::Title()
	: Scene()
	// スマートポインタ
	, mpGameStart(nullptr)
	, mpExplainGraph(nullptr)
	, mpOptionButton(nullptr)
	, mpMusicClose(nullptr)
	, mpBgmVolumeBar(nullptr)
	, mpSeVolumeBar(nullptr)
	// 次のシーン
	, mNextScene(NONE_SCENE)
	// 画像ハンドル
	, mnRogoHandle(-1)
	, mnBagHandle(-1)
	, mnCardHandle(-1)
	// 菊池
	, mnKorukuitaHandle(-1)
	, mnBatuHandle(-1)
	// 音量設定
	, mbOption(false)
	// タイトル演出
	, mbMouseButton(false)
	, mbWhite(false)
	// カード演出
	, mnCardX(TitleAnimation::CardInitialX)
	, mnCardY(TitleAnimation::CardInitialY)
	, mnCardAngle(3.0f)
	, mnCardRota(0.01f)
	// カードのターゲット位置
	, targetX(TitlePosition::CardTargetOffsetX)
	, targetY(TitlePosition::CardTargetOffsetY)
	, targetAngle(-0.05f)
	, targetRota(1.0f)
	// 白いBOX
	, mfWhiteBoxAlpha(0.0f)
	// ボタン演出
	, mfStartX(TitlePosition::StartInitialX)
	, mfStartY(static_cast<float>(ScreenSize::Height) + TitlePosition::ButtonInitialYOffset)
	, mfExplainX(static_cast<float>(ScreenSize::Width) + TitlePosition::ExplainInitialX)
	, mfExplainY(static_cast<float>(ScreenSize::Height) + TitlePosition::ButtonInitialYOffset)
	, mfTargetStartX(static_cast<float>(ScreenSize::CenterX) + TitlePosition::StartTargetOffsetX)
	, mfTargetStartY(static_cast<float>(ScreenSize::CenterY) + TitlePosition::StartTargetOffsetY)
	, mfTargetExplainX(static_cast<float>(ScreenSize::CenterX) + TitlePosition::ExplainTargetOffsetX)
	, mfTargetExplainY(static_cast<float>(ScreenSize::CenterY) + TitlePosition::ExplainTargetOffsetY)
	// ロゴ演出
	, mfTurnY(TitleAnimation::LogoInitialY)
	, mbInitialize(false)
	, mfLogoTime(0.0f)
{

}


Title::~Title()
{
	mpGameStart.reset();
	mpExplainGraph.reset();
	mpOptionButton.reset();
	mpMusicClose.reset();
	mpBgmVolumeBar.reset();
	mpSeVolumeBar.reset();
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

	mnRogoHandle = LoadGraph(TitleResourcePath::Logo.c_str());
	if (mnRogoHandle == -1) { printfDx("ロゴ画像ない"); }
	mnBagHandle = LoadGraph(TitleResourcePath::Background.c_str());
	if (mnBagHandle == -1) { printfDx("背景画像ない"); }
	mnCardHandle = LoadGraph(TitleResourcePath::MoveBackGround.c_str());
	if (mnCardHandle == -1) { printfDx("動く黒板の画像がない"); }


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
		ColorOption::BgmBar
	);
	mpSeVolumeBar = std::make_unique<VolumeBar>(
		TitlePosition::VolumeBarX,
		TitlePosition::SeBarY,
		TitlePosition::VolumeBarWidth,
		TitlePosition::VolumeBarHeight,
		ColorOption::SeBar
	);

	mbInitialize = true; // 初期化終わりON
}


void Title::Update()
{
	// ボタンの飛んでくる演出
	UpdateButtonAnimation();

	// ロゴの演出
	UpdateLogoAnimation();

	// ボタン・設定ボタンの入力
	UpdateButtonInput();

	// シーン遷移の演出
	UpdateSceneTransition();

	// 白いBOXの透過
	UpdateWhiteBox();

	// 音量設定
	UpdateVolumeSetting();

	Scene::Update();
}


void Title::UpdateButtonAnimation()
{
	// スタートボタンを目標位置へ移動
	mfStartX +=
		(mfTargetStartX - mfStartX) *
		TitleAnimation::ButtonMoveSpeed;

	mfStartY +=
		(mfTargetStartY - mfStartY) *
		TitleAnimation::ButtonMoveSpeed;

	// 説明ボタンを目標位置へ移動
	mfExplainX +=
		(mfTargetExplainX - mfExplainX) *
		TitleAnimation::ButtonMoveSpeed;

	mfExplainY +=
		(mfTargetExplainY - mfExplainY) *
		TitleAnimation::ButtonMoveSpeed;


	// スタートボタン
	if (mpGameStart)
	{
		mpGameStart->SetPosition(mfStartX, mfStartY);

		if (fabs(mfStartX - mfTargetStartX) <
			TitleAnimation::ButtonStopDistance &&
			fabs(mfStartY - mfTargetStartY) <
			TitleAnimation::ButtonStopDistance)
		{
			mfStartX = mfTargetStartX;
			mfStartY = mfTargetStartY;
		}
	}

	// 説明ボタン
	if (mpExplainGraph)
	{
		if (fabs(mfExplainX - mfTargetExplainX) <
			TitleAnimation::ButtonStopDistance &&
			fabs(mfExplainY - mfTargetExplainY) <
			TitleAnimation::ButtonStopDistance)
		{
			mfExplainX = mfTargetExplainX;
			mfExplainY = mfTargetExplainY;
		}

		mpExplainGraph->SetPosition(mfExplainX, mfExplainY);
	}
}

void Title::UpdateLogoAnimation()
{
	if (!mbInitialize)
	{
		return;
	}

	static float velocity = 0.0f;
	mfLogoTime += TitleAnimation::LogoMoveSpeed;

	float baseY =
		(Utility::SCREEN_HEIGHT / 2) +
		TitlePosition::LogoBaseYOffset;

	float targetY =
		baseY +
		sinf(mfLogoTime) *
		TitleAnimation::LogoSwingSize;

	float gravity = TitleAnimation::Gravity;
	float power = TitleAnimation::SpringPower;
	float damping = TitleAnimation::Damping;

	// 重力
	velocity += gravity;
	// バネ
	float force = (targetY - mfTurnY) * power;
	velocity += force;
	// 減衰
	velocity *= damping;
	// 移動
	mfTurnY += velocity;

	// ほぼ目的地についたら停止
	if (fabs(velocity) < TitleAnimation::VelocityStop &&
		fabs(targetY - mfTurnY) <
		TitleAnimation::TargetStopDistance)
	{
		mfTurnY = targetY;
		velocity = 0.0f;
	}
}

void Title::UpdateButtonInput()
{
	// マウスボタンを押している最中は操作できない
	if (!mpGameStart ||
		!mpExplainGraph ||
		mbMouseButton)
	{
		return;
	}

	mpGameStart->Update();
	mpExplainGraph->Update();

	// 設定ボタン	  x
	if (mpOptionButton)
	{
		mpOptionButton->Update();

		// 音量設定中は設定ボタンを押せない
		if (mbOption)
		{
			mpOptionButton->SetActive(false);
		}
		else
		{
			mpOptionButton->SetActive(true);
		}


		// 設定ボタンが押された
		if (mpOptionButton->IsClicked() &&
			!mbOption)
		{
			Master::mpGameManager
				->GetSoundManager()
				->PlaySE(SoundManager::SE_DECIDE);

			mbOption = true;
		}
	}

	// ゲーム開始ボタン
	if (mpGameStart->IsClicked() &&
		!mbOption)
	{
		Master::mpGameManager
			->GetSoundManager()
			->PlaySE(SoundManager::SE_DECIDE);

		mbMouseButton = true;

		mNextScene = SELECT_SCENE;
	}

	// 説明ボタン
	else if (mpExplainGraph->IsClicked() &&
		!mbOption)
	{
		Master::mpGameManager
			->GetSoundManager()
			->PlaySE(SoundManager::SE_DECIDE);

		mbMouseButton = true;

		mNextScene = EXPLAIN_SCENE;
	}

	// 音量設定の×ボタン
	if (mpMusicClose)
	{
		mpMusicClose->Update();

		if (mpMusicClose->IsClicked())
		{
			Master::mpGameManager
				->GetSoundManager()
				->PlaySE(SoundManager::SE_DECIDE);

			mbOption = false;
		}
	}
}

void Title::UpdateSceneTransition()
{
	if (!mbMouseButton)
	{
		return;
	}


	float diffX =
		(float)targetX - mnCardX;

	float diffY =
		(float)targetY - mnCardY;

	float diffAngle =
		(float)targetAngle - mnCardAngle;

	float diffRota =
		(float)targetRota - mnCardRota;


	// 移動
	mnCardX +=
		diffX * TitleAnimation::CardMoveSpeed;

	mnCardY +=
		diffY * TitleAnimation::CardMoveSpeed;

	mnCardAngle +=
		diffAngle * TitleAnimation::CardMoveSpeed;

	mnCardRota +=
		diffRota * TitleAnimation::CardMoveSpeed;


	// 角度がマイナスにならないようにする
	if (mnCardAngle <= 0.0f)
	{
		mnCardAngle = 0.0f;
	}


	// ほぼ目的地に到着
	if (fabsf(diffX) <
		TitleAnimation::CardStopDistanceX &&
		fabsf(diffY) <
		TitleAnimation::CardStopDistanceY &&
		fabsf(diffRota) <
		TitleAnimation::CardStopDistanceRota)
	{
		targetRota +=
			TitleAnimation::CardRotaIncrease;


		if (targetRota >=
			TitleAnimation::CardRotaMax)
		{
			targetRota =
				TitleAnimation::CardRotaMax;

			mbWhite = true;
		}
	}


	// シーン変更
	if (mbWhite &&
		mfWhiteBoxAlpha >
		TitleAnimation::WhiteBoxSceneChangeAlpha)
	{
		if (mNextScene == SELECT_SCENE)
		{
			Master::Master::mpGameManager
				->GetSceneManager()
				->SetNextScene(
					SceneManager::SELECT_SCENE);
		}

		if (mNextScene == EXPLAIN_SCENE)
		{
			Master::Master::mpGameManager
				->GetSceneManager()
				->SetNextScene(
					SceneManager::EXPAIN_SCENE);
		}

		return;
	}
}


void Title::UpdateVolumeSetting()
{
	if (mbMouseButton || !mbOption)
	{
		return;
	}


	int mouseX;
	int mouseY;

	GetMousePoint(&mouseX, &mouseY);


	if (!(GetMouseInput() & MOUSE_INPUT_LEFT))
	{
		return;
	}


	// BGMバー
	if (mpBgmVolumeBar &&
		mpBgmVolumeBar->IsMouseOver(mouseX, mouseY))
	{
		int bgmVolume =
			mpBgmVolumeBar->GetVolumeFromMouse(mouseX);

		Master::mpGameManager
			->GetSoundManager()
			->SetBGMVolume(bgmVolume);

		Master::mpGameManager
			->GetSoundManager()
			->SaveVolume();
	}


	// SEバー
	if (mpSeVolumeBar &&
		mpSeVolumeBar->IsMouseOver(mouseX, mouseY))
	{
		int seVolume =
			mpSeVolumeBar->GetVolumeFromMouse(mouseX);

		Master::mpGameManager
			->GetSoundManager()
			->SetSEVolume(seVolume);

		Master::mpGameManager
			->GetSoundManager()
			->SaveVolume();
	}
}


void Title::UpdateWhiteBox()
{
	if (!mbWhite)
	{
		return;
	}

	mfWhiteBoxAlpha +=
		TitleAnimation::WhiteBoxAlphaIncrease;

	if (mfWhiteBoxAlpha >
		TitleAnimation::WhiteBoxMaxAlpha)
	{
		mfWhiteBoxAlpha =
			TitleAnimation::WhiteBoxMaxAlpha;
	}
}


void Title::Draw()
{
	if (!mbInitialize) return;

	// ロゴの位置
	int x = Utility::SCREEN_WIDTH / 2;
	int y = Utility::SCREEN_HEIGHT / 2;
	int color = ColorOption::White;


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
			DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, ColorOption::White, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	DrawVolumeSettings();
	DrawVolumeTexts();
	Scene::Draw();
}

// 音量設定中フラグがONなら描画される
void Title::DrawVolumeSettings()
{
	if (mbOption)
	{
		mVolumeSet.DrawVolumeSettingPanel();

		// 黒板の上の×ボタンの描画
		if (mpMusicClose)
		{
			mpMusicClose->Draw();
		}
	}
}

void Title::DrawVolumeTexts()
{
	if (mbOption)
	{
		mVolumeSet.DrawBgmText(*mpBgmVolumeBar);
		mVolumeSet.DrawSeText(*mpSeVolumeBar);
	}
}

void Title::Finalize()
{

	// 画像ハンドルの削除
	if (mnRogoHandle != -1) { DeleteGraph(mnRogoHandle); mnRogoHandle = -1; }
	if (mnBagHandle != -1) { DeleteGraph(mnBagHandle);  mnBagHandle = -1; }
	if (mnCardHandle != -1) { DeleteGraph(mnCardHandle); mnCardHandle = -1; }
}