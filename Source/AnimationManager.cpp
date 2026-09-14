#include "AnimationManager.h"
#include "Player.h"
#include "Enemy.h"
#include <cmath>
#include "DxLib.h"


// 1つのポーズの状態をまとめたデータ構造
struct AnimStep
{
    float x = 0.0f;     // 横方向の移動量
    float y = 0.0f;     // 縦方向の移動量
    float angleDegree = 0.0f; // 回転
};

// 移動だけのアニメーションステップを作る
static AnimStep Move(float x, float y)
{
    return { x, y, 0.0f };
}


// 移動と回転を含むアニメーションステップを作る
static AnimStep MoveAndRotate(
    float x,
    float y,
    float angleDegree)
{
    return { x, y, angleDegree };
}

// AnimStepをAnimationData::AnimFrameに変換して追加する
static void AddComplexDegreeFrames(
    AnimationData& anim,
    AnimationData::AnimOwner owner,
    int partIdx,
    const std::vector<AnimStep>& steps)
{
    const float DEG_TO_RAD = DX_PI_F / 180.0f;

    for (const auto& step : steps)
    {
        AnimationData::AnimFrame frame;

        frame.owner = owner;
        frame.partIndex = partIdx;
        frame.targetX = step.x;
        frame.targetY = step.y;

        // 度数法からラジアンへ変換
        frame.targetAngle = step.angleDegree * DEG_TO_RAD;

        anim.AddFrame(frame);
    }
}


// アニメーションデータの実体
std::unordered_map<AnimationData::AnimType, AnimationData>
AnimationManager::animCatalog;


// アニメーションの初期化
void AnimationManager::Initialize()
{
    CreateIdleAnimation();
    CreateAttackAnimation();
    CreateDamageAnimation();
    CreateSpecialAnimation();
    CreateDeathAnimation();
}




// 待機アニメーション
// ============================================================

void AnimationManager::CreateIdleAnimation()
{
    auto& anim = animCatalog[AnimationData::AnimType::IDLE];

    anim.SetType(AnimationData::AnimType::IDLE);
    anim.SetDuration(60);
    anim.SetLoop(true);


    // -------------------------
    // プレイヤー
    // -------------------------

    // 胴体
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::BODY,
        {
            Move(0.0f,  0.0f),
            Move(0.0f, -2.5f),
            Move(0.0f, -5.0f),
            Move(0.0f,  2.5f),
            Move(0.0f,  0.0f)
        });


    // 頭
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::HEAD,
        {
            Move(0.0f, 0.0f),
            MoveAndRotate(0.0f, -2.5f, 1.0f),
            MoveAndRotate(0.0f, -5.0f, 3.0f),
            MoveAndRotate(0.0f,  2.5f, 1.0f),
            Move(0.0f, 0.0f)
        });


    // 右手
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::ARM_R,
        {
            Move(0.0f,  0.0f),
            Move(0.0f, -2.5f),
            MoveAndRotate(0.0f, -5.0f, 5.0f),
            Move(0.0f,  2.5f),
            Move(0.0f,  0.0f)
        });


    // 武器
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::WEAPON,
        {
            Move(0.0f,  0.0f),
            Move(0.0f, -2.5f),
            MoveAndRotate(0.0f, -5.0f, 5.0f),
            Move(0.0f,  2.5f),
            Move(0.0f,  0.0f)
        });


    // 右足
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::LEG_R,
        {
            Move(0.0f,  0.0f),
            Move(0.0f, -1.5f),
            Move(0.0f, -3.0f),
            Move(0.0f,  1.5f),
            Move(0.0f,  0.0f)
        });


    // 左足
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::LEG_L,
        {
            Move(0.0f,  0.0f),
            Move(0.0f, -1.5f),
            Move(0.0f, -3.0f),
            Move(0.0f,  1.5f),
            Move(0.0f,  0.0f)
        });


    // -------------------------
    // 敵
    // -------------------------

    // 全体 待機
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::ENEMY,
        Enemy::PartID::BODY,
        {
            Move(0.0f,  0.0f),
            Move(0.0f, -2.5f),
            Move(0.0f, -5.0f),
            Move(0.0f,  2.5f),
            Move(0.0f,  0.0f)
        });
}


// ============================================================
// 攻撃アニメーション
// ============================================================

void AnimationManager::CreateAttackAnimation()
{
    auto& anim = animCatalog[AnimationData::AnimType::ATTACK];

    anim.SetType(AnimationData::AnimType::ATTACK);
    anim.SetDuration(40);
    anim.SetLoop(false);


    // -------------------------
    // プレイヤー
    // -------------------------

    // 胴体
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::BODY,
        {
            Move(0.0f, 10.0f),
            Move(0.0f,  0.0f),
            Move(0.0f,  0.0f)
        });


    // 頭
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::HEAD,
        {
            MoveAndRotate(0.0f, 0.0f, -10.0f),
            Move(0.0f, 0.0f),
            Move(0.0f, 0.0f)
        });


    // 武器
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::WEAPON,
        {
            MoveAndRotate(-5.0f, -10.0f, -40.0f),
            MoveAndRotate(0.0f,  14.0f,  30.0f),
            Move(0.0f, 0.0f)
        });


    // 腕
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::ARM_R,
        {
            MoveAndRotate(-5.0f, -10.0f, -40.0f),
            MoveAndRotate(0.0f,  14.0f,  30.0f),
            Move(0.0f, 0.0f)
        });


    // -------------------------
    // 敵
    // -------------------------

    // 全体 攻撃
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::ENEMY,
        Enemy::PartID::BODY,
        {
            MoveAndRotate(-80.0f, 0.0f, -30.0f),
            Move(0.0f, 0.0f)
        });
}


// ============================================================
// ダメージアニメーション
// ============================================================

void AnimationManager::CreateDamageAnimation()
{
    auto& anim = animCatalog[AnimationData::AnimType::DAMAGE];

    anim.SetType(AnimationData::AnimType::DAMAGE);
    anim.SetDuration(40);
    anim.SetLoop(false);


    // -------------------------
    // プレイヤー
    // -------------------------

    // 胴体
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::BODY,
        {
            MoveAndRotate(-30.0f, 0.0f, -30.0f),
            Move(0.0f, 0.0f)
        });


    // 頭
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::HEAD,
        {
            MoveAndRotate(-30.0f, 0.0f, -30.0f),
            Move(0.0f, 0.0f)
        });


    // 右手
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::ARM_R,
        {
            MoveAndRotate(-30.0f, 0.0f, -30.0f),
            Move(0.0f, 0.0f)
        });


    // 武器
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::WEAPON,
        {
            MoveAndRotate(-30.0f, 0.0f, -40.0f),
            Move(0.0f, 0.0f)
        });


    // 右足
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::LEG_R,
        {
            MoveAndRotate(0.0f, 0.0f, -3.0f),
            Move(0.0f, 0.0f)
        });


    // 左足
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::LEG_L,
        {
            MoveAndRotate(-30.0f, 0.0f, -20.0f),
            Move(0.0f, 0.0f)
        });


    // -------------------------
    // 敵
    // -------------------------

    // 全体 ダメージ
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::ENEMY,
        Enemy::PartID::BODY,
        {
            MoveAndRotate(80.0f, 0.0f, 30.0f),
            Move(0.0f, 0.0f)
        });
}


// ============================================================
// 必殺技アニメーション
// ============================================================

void AnimationManager::CreateSpecialAnimation()
{
    auto& anim = animCatalog[AnimationData::AnimType::SPECIAL];

    anim.SetType(AnimationData::AnimType::SPECIAL);
    anim.SetDuration(50);
    anim.SetLoop(false);


    // -------------------------
    // プレイヤー
    // -------------------------

    // 胴体
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::BODY,
        {
            Move(400.0f, 10.0f),
            Move(400.0f,  0.0f),
            Move(0.0f,  0.0f)
        });


    // 頭
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::HEAD,
        {
            MoveAndRotate(400.0f, 0.0f, -10.0f),
            Move(400.0f, 0.0f),
            Move(0.0f, 0.0f)
        });


    // 武器
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::WEAPON,
        {
            MoveAndRotate(400.0f, -10.0f, -40.0f),
            MoveAndRotate(400.0f,  20.0f,  30.0f),
            Move(0.0f, 0.0f)
        });


    // 腕
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::ARM_R,
        {
            MoveAndRotate(400.0f, -10.0f, -40.0f),
            MoveAndRotate(400.0f,  20.0f,  30.0f),
            Move(0.0f, 0.0f)
        });


    // 左足
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::LEG_L,
        {
            Move(400.0f, 0.0f),
            Move(400.0f, 0.0f),
            Move(0.0f, 0.0f)
        });


    // 右足
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::LEG_R,
        {
            Move(400.0f, 0.0f),
            Move(400.0f, 0.0f),
            Move(0.0f, 0.0f)
        });


    // -------------------------
    // 敵
    // -------------------------

    // 全体 必殺
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::ENEMY,
        Enemy::PartID::BODY,
        {
            MoveAndRotate(-400.0f, 0.0f, 60.0f),
            MoveAndRotate(-400.0f, 0.0f, 60.0f),
            Move(-400.0f, 0.0f),
            Move(0.0f, 0.0f)
        });
}


// ============================================================
// 死亡アニメーション
// ============================================================

void AnimationManager::CreateDeathAnimation()
{
    auto& anim = animCatalog[AnimationData::AnimType::DEATH];

    anim.SetType(AnimationData::AnimType::DEATH);
    anim.SetDuration(80);
    anim.SetLoop(false);


    // -------------------------
    // プレイヤー
    // -------------------------

    // 胴体
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::BODY,
        {
            Move(0.0f, 0.0f),
            MoveAndRotate(0.0f, 120.0f, 90.0f),
            MoveAndRotate(0.0f, 120.0f, 90.0f),
            MoveAndRotate(0.0f, 120.0f, 90.0f)
        });


    // 頭
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::HEAD,
        {
            Move(0.0f, 0.0f),
            MoveAndRotate(0.0f, 120.0f, 90.0f),
            MoveAndRotate(0.0f, 120.0f, 90.0f),
            MoveAndRotate(0.0f, 120.0f, 90.0f)
        });


    // 武器
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::WEAPON,
        {
            MoveAndRotate(-10.0f, 20.0f, 40.0f),
            Move(0.0f, 100.0f),
            Move(0.0f, 100.0f),
            Move(0.0f, 100.0f)
        });


    // 腕
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::ARM_R,
        {
            MoveAndRotate(-10.0f, 30.0f, 40.0f),
            Move(34.0f, 100.0f),
            Move(34.0f, 100.0f),
            Move(34.0f, 100.0f)
        });


    // 左足
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::LEG_L,
        {
            Move(0.0f, 0.0f),
            MoveAndRotate(0.0f, 110.0f, 100.0f),
            MoveAndRotate(0.0f, 110.0f, 100.0f),
            MoveAndRotate(0.0f, 110.0f, 100.0f)
        });


    // 右足
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::PLAYER,
        Player::PartID::LEG_R,
        {
            Move(0.0f, 0.0f),
            MoveAndRotate(30.0f, 110.0f, 60.0f),
            MoveAndRotate(30.0f, 110.0f, 60.0f),
            MoveAndRotate(30.0f, 110.0f, 60.0f)
        });


    // -------------------------
    // 敵
    // -------------------------

    // 全体 死亡
    AddComplexDegreeFrames(
        anim,
        AnimationData::AnimOwner::ENEMY,
        Enemy::PartID::BODY,
        {
            Move(0.0f, 0.0f),
            MoveAndRotate(0.0f, 0.0f, 180.0f),
            MoveAndRotate(0.0f, 0.0f, 180.0f)
        });
}


// ============================================================
// アニメーションデータの取得
// ============================================================

const AnimationData& AnimationManager::GetAnim(
    AnimationData::AnimType type) const
{
    return animCatalog.at(type);
}