#include "InputManager.h"
#include "Master.h"
#include "Title.h"
#include "Utility.h"
#include "GameConstants.h"

Title::Title()
	: Scene()
	// スマートポインタ
	, mpGameStart(nullptr)
	, mpExplainGraph(nullptr)
	, mpOptionButton(nullptr)
	, mpTitleEffectAnimation(nullptr)
	, mpSoundOption(nullptr)
	// 次のシーン
	, mNextScene(NONE_SCENE)
	// 画像ハンドル
	, mnRogoHandle(-1)
	, mnBagHandle(-1)
	, mnCardHandle(-1)
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
	mpTitleEffectAnimation.reset();
	mpSoundOption.reset();
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

	// カード・シーン遷移演出の生成と初期化を行うため
	mpTitleEffectAnimation = std::make_unique<TitleEffectAnimation>();
	mpTitleEffectAnimation->Initialize();
	mpTitleEffectAnimation->SetCardHandle(mnCardHandle);

	// 音量設定機能の生成と初期化を行うため
	mpSoundOption = std::make_unique<SoundOption>();
	mpSoundOption->Initialize();

	// 設定ボタンの生成
	mpOptionButton = std::make_unique<MouseGraph>(
		TitlePosition::OptionButtonX,
		TitlePosition::OptionButtonY,
		0.0f,
		TitleResourcePath::OptionButton.c_str(),
		TitleScale::OptionNormal,
		TitleScale::OptionHover
	);

	Master::mpGameManager->GetSoundManager()->PlayBGM(SoundManager::BgmTitle);
	mbInitialize = true; // 初期化終わりON
}

void Title::Update()
{
	// 音量設定が開いている場合はタイトルの入力を制限するため
	bool isOptionOpen = mpSoundOption && mpSoundOption->GetIsOpen();

	if (!isOptionOpen)
	{
		// ボタンの飛んでくる演出
		UpdateButtonAnimation();

		// ロゴの演出
		UpdateLogoAnimation();

		// ボタン・設定ボタンの入力
		UpdateButtonInput();
	}

	// 音量設定の更新処理を行うため
	if (mpSoundOption)
	{
		mpSoundOption->Update();
	}

	// シーン遷移の演出を更新するため
	if (mpTitleEffectAnimation)
	{
		mpTitleEffectAnimation->Update();

		// シーン遷移条件を満たしたかを判定するため
		if (mpTitleEffectAnimation->GetIsWhite() &&
			mpTitleEffectAnimation->GetWhiteBoxAlpha() >
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

	Scene::Update();
}

// ボタンの移動アニメーションを更新
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

// ロゴの上下アニメーションを更新
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

// ボタンのクリック・入力処理を更新
void Title::UpdateButtonInput()
{
	// マウスボタンを押している最中は操作できない
	if (!mpGameStart || !mpExplainGraph ||
		(mpTitleEffectAnimation && mpTitleEffectAnimation->GetIsMouseButton()))
	{
		return;
	}

	mpGameStart->Update();
	mpExplainGraph->Update();

	// 設定ボタン
	if (mpOptionButton)
	{
		mpOptionButton->Update();

		// 音量設定が開いていないときのみ設定ボタンの有効状態を切り替えるため
		bool isOptionOpen = mpSoundOption && mpSoundOption->GetIsOpen();
		mpOptionButton->SetActive(!isOptionOpen);

		// 設定ボタンが押されたらオプション画面を開くため
		if (mpOptionButton->IsClicked() && !isOptionOpen)
		{
			Master::mpGameManager
				->GetSoundManager()
				->PlaySE(SoundManager::SE_DECIDE);

			if (mpSoundOption)
			{
				mpSoundOption->SetIsOpen(true);
			}
		}
	}

	// 音量設定が開いていないときのみゲーム開始・説明ボタンの入力を受け付けるため
	bool isOptionOpen = mpSoundOption && mpSoundOption->GetIsOpen();
	if (isOptionOpen)
	{
		return;
	}

	// ゲーム開始ボタン
	if (mpGameStart->IsClicked())
	{
		Master::mpGameManager
			->GetSoundManager()
			->PlaySE(SoundManager::SE_DECIDE);

		if (mpTitleEffectAnimation)
		{
			mpTitleEffectAnimation->StartTransition();
		}

		mNextScene = SELECT_SCENE;
	}

	// 説明ボタン
	else if (mpExplainGraph->IsClicked())
	{
		Master::mpGameManager
			->GetSoundManager()
			->PlaySE(SoundManager::SE_DECIDE);

		if (mpTitleEffectAnimation)
		{
			mpTitleEffectAnimation->StartTransition();
		}
		mNextScene = EXPLAIN_SCENE;
	}
}

void Title::Draw()
{
	if (!mbInitialize) return;

	// ロゴの位置
	int x = Utility::SCREEN_WIDTH / 2;
	int y = Utility::SCREEN_HEIGHT / 2;

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

	// 画面遷移演出を描画するため
	if (mpTitleEffectAnimation)
	{
		mpTitleEffectAnimation->Draw();
	}

	// 音量設定画面を描画するため
	if (mpSoundOption)
	{
		mpSoundOption->Draw();
	}

	Scene::Draw();
}

void Title::Finalize()
{
	// 画像ハンドルの削除
	if (mnRogoHandle != -1) { DeleteGraph(mnRogoHandle); mnRogoHandle = -1; }
	if (mnBagHandle != -1) { DeleteGraph(mnBagHandle); mnBagHandle = -1; }
	if (mnCardHandle != -1) { DeleteGraph(mnCardHandle); mnCardHandle = -1; }
}