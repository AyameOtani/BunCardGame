#include "TitleEffectAnimation.h"
#include "DxLib.h"
#include "GameConstants.h"
#include "Utility.h"
#include <cmath>

TitleEffectAnimation::TitleEffectAnimation()
    : mnBlackboardHandle(-1)
    , mbIsMouseButton(false)
    , mbIsWhite(false)
    , mfBlackboardPositionX(0.0f)
    , mfBlackboardPositionY(0.0f)
    , mfBlackboardAngle(0.0f)
    , mfBlackboardRotation(0.0f)
    , mfTargetPositionX(0.0f)
    , mfTargetPositionY(0.0f)
    , mfTargetAngle(0.0f)
    , mfTargetRotation(0.0f)
    , mfWhiteBoxAlpha(0.0f)
{
    // 初期化処理を一箇所にまとめため
    Initialize();
}

TitleEffectAnimation::~TitleEffectAnimation()
{

}

void TitleEffectAnimation::Initialize()
{
    // 状態を初期値にリセットするため
    mfBlackboardPositionX = TitleAnimation::BlackboardInitialX;
    mfBlackboardPositionY = TitleAnimation::BlackboardInitialY;
    mfBlackboardAngle = TitleAnimation::BlackboardInitialAngle;
    mfBlackboardRotation = TitleAnimation::BlackboardInitialRotation;

    mfTargetPositionX = TitlePosition::CardTargetOffsetX;
    mfTargetPositionY = TitlePosition::CardTargetOffsetY;
    mfTargetAngle = TitleAnimation::BlackboardTargetAngle;
    mfTargetRotation = TitleAnimation::BlackboardTargetRotation;

    mfWhiteBoxAlpha = 0.0f;
    mbIsMouseButton = false;
    mbIsWhite = false;
}

void TitleEffectAnimation::Update()
{
    UpdateSceneTransition();
    UpdateWhiteBox();
}

void TitleEffectAnimation::Draw() const
{
    if (!mbIsMouseButton)
    {
        return;
    }

    // シーン遷移時の演出用として黒板イラストを描画するため
    DrawRotaGraph(
        static_cast<int>(mfBlackboardPositionX),
        static_cast<int>(mfBlackboardPositionY),
        mfBlackboardRotation,
        mfBlackboardAngle,
        mnBlackboardHandle,
        TRUE
    );

    // シーン遷移へ向けて画面を覆う白いボックスを描画するため
    if (mbIsWhite)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(mfWhiteBoxAlpha));
        DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, ColorOption::White, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}


void TitleEffectAnimation::UpdateSceneTransition()
{
    if (!mbIsMouseButton)
    {
        return;
    }

    float differenceX = mfTargetPositionX - mfBlackboardPositionX;
    float differenceY = mfTargetPositionY - mfBlackboardPositionY;
    float differenceAngle = mfTargetAngle - mfBlackboardAngle;
    float differenceRotation = mfTargetRotation - mfBlackboardRotation;

    // 目標位置へ滑らかに近づけるため
    mfBlackboardPositionX += differenceX * TitleAnimation::BlackboardMoveSpeed;
    mfBlackboardPositionY += differenceY * TitleAnimation::BlackboardMoveSpeed;
    mfBlackboardAngle += differenceAngle * TitleAnimation::BlackboardMoveSpeed;
    mfBlackboardRotation += differenceRotation * TitleAnimation::BlackboardMoveSpeed;

    // 角度がマイナスになることを防ぐため
    if (mfBlackboardAngle <= 0.0f)
    {
        mfBlackboardAngle = 0.0f;
    }

    // ほぼ目的地に到着したかを判定するため
    if (fabsf(differenceX) < TitleAnimation::BlackboardStopDistanceX &&
        fabsf(differenceY) < TitleAnimation::BlackboardStopDistanceY &&
        fabsf(differenceRotation) < TitleAnimation::BlackboardStopDistanceRota)
    {
        mfTargetRotation += TitleAnimation::BlackboardRotaIncrease;

        if (mfTargetRotation >= TitleAnimation::BlackboardRotaMax)
        {
            mfTargetRotation = TitleAnimation::BlackboardRotaMax;
            mbIsWhite = true;
        }
    }
}

void TitleEffectAnimation::UpdateWhiteBox()
{
    if (!mbIsWhite)
    {
        return;
    }

    mfWhiteBoxAlpha += TitleAnimation::WhiteBoxAlphaIncrease;
    if (mfWhiteBoxAlpha > TitleAnimation::WhiteBoxMaxAlpha)
    {
        mfWhiteBoxAlpha = TitleAnimation::WhiteBoxMaxAlpha;
    }
}