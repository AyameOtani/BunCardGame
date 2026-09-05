#include "Card.h"
#include "Enemy.h"
#include "Master.h"
#include "Utility.h"

Enemy::Enemy(VECTOR initPos, std::string filename,EnemyType type)
    : Unit(filename, initPos)
    , mType(type)
{
    SetFontSize(25);

    animator.ownerType = AnimationData::AnimOwner::ENEMY;

    //敵のタイプごとにステータスを変える
    switch (mType)
    {
    case EnemyType::TypeA:
        // ステータス一旦ここ
        hp = 80;
        maxHp = 80;

        // 敵の行動値
        mnDamage = 8;
        mnBlock = 5;
        mnApplyBuff = 3;
        mnPoison = 3;
        mnApplyWeaken = 3;
        
        break;

    case EnemyType::TypeB:
        // ステータス一旦ここ
        hp = 160;
        maxHp = 160;

        // 敵の行動値
        mnDamage = 12;
        mnBlock = 7;
        mnApplyBuff = 5;
        mnPoison = 4;
        mnApplyWeaken = 6;

        break;

    case EnemyType::TypeC:
		// ステータス一旦ここ
        hp = 320;
		maxHp = 320;

        // 敵の行動値
        mnDamage = 16;
        mnBlock = 13;
        mnApplyBuff = 12;
        mnPoison = 5;
        mnApplyWeaken = 10;

		break;

    default:
        // ステータス一旦ここ
        hp = 130;
        maxHp = 130;

        // 敵の行動値
        mnDamage = 15;
        mnBlock = 12;
        mnApplyBuff = 8;
        mnPoison = 6;
        mnApplyWeaken = 8;

        break;
    }

	displayDamageHp = maxHp; // 表示上のHPを初期HPに設定

    // 画像の大きさ大きくする
    SetScale(1.5);

    // タグ設定
    SetTag(Object2D::Enemy2D);

    mnPoisonHandle = LoadGraph("Resource/Icon/icon_poison.png"); // 毒のエフェクト画像のロード
    mnShieldHandle = LoadGraph("Resource/Icon/icon_shield.png"); // シールドのエフェクト画像のロード
    mnCurseHandle = LoadGraph("Resource/Icon/icon_curse.png"); // シールドのエフェクト画像のロード
}

void Enemy::PlayMotion(AnimationData::AnimType type)
{
    // 敵用のアニメーションを取得して再生
    const AnimationData* anim = Master::mpAnimationManager->GetAnim(type);
    animator.Play(anim);
}

// まだ追加してないからコメントアウト
void Enemy::Initialize(std::string filename)
{
    parts.clear();
    parts.resize(PartID::PART_MAX); // 敵は敵の PART_MAX

    parts[PartID::BODY].graphHandle = LoadGraph(filename.c_str());

    animator.Play(Master::mpAnimationManager->GetAnim(AnimationData::AnimType::IDLE));
}


Enemy::~Enemy()
{

}

void Enemy::Update()
{
    // HPゲージのやつ呼び出し
    HpGaugeUpdate();

    Object2D::Update();
}

// HPバーの描画 
void Enemy::HpGaugeDraw()
{
    int y = (int)mvPosition.y + 110;
    int drawY = y+4; // 基準位置


    // Hpゲージの作成 //
    int hpX = (int)mvPosition.x - 150;
    // Hpゲージの枠
    DrawRoundRect(hpX, y, hpX + width, y + height, 10, 10, GetColor(80, 80, 80), true);


    // ダメージ分のゲージ（赤）
    DrawRoundRect(
        hpX,
        y,
        hpX + gaugeDamegeWidth,
        y + height,
        10,
        10,
        GetColor(255, 0, 0),
        true
    );

    // 本来のゲージ幅
    int drawGaugeWidth = gaugeWidth;

    // 最低幅
    int minWidth = (int)((float)mnHpLow / maxHp * width);

    // HPが残っているなら最低幅を保証
    if (drawGaugeWidth < minWidth && hp > 0)
    {
        drawGaugeWidth = minWidth;
    }

    // Hpゲージ（赤）
    DrawRoundRect(hpX, y, hpX + drawGaugeWidth, y + height,
        10, 10, GetColor(190, 24, 24), true);

    // Hpゲージの枠線
    DrawRoundRect(hpX, y, hpX + width, y + height, 10, 10, GetColor(255, 255, 255), false);

    DrawFormatStringToHandle(hpX + 70, drawY, GetColor(255, 255, 255),
        Master::mpFontManager->GetHpFontHandle(), "HP  %d / %d", hp, maxHp);
}

void Enemy::EnemyIcon()
{
    // 表示を開始する基準位置
    int x = (int)mvPosition.x - 80;
    int y = (int)mvPosition.y + 110;
    //アイコンの描画
    int iconX = x - 50;
    int iconY = y += 20;

    int yDist = 18;

    //シールドのアイコン描画
    DrawGraph(iconX - 35, iconY, mnShieldHandle, TRUE);
    // 表示する文字幅を取得
    int textWidth_S = GetDrawFormatStringWidth("%d", block);
    // 中央になるようにXを調整
    int ShildText = (x - 43) - textWidth_S / 2;
    // 描画
    DrawFormatStringToHandle(ShildText, y + yDist, GetColor(230, 230, 230),
        Master::mpFontManager->GetStatusFontHandle(), "%d", block);


    //呪いのアイコン描画
    DrawGraph(iconX + 10, iconY - 10, mnCurseHandle, TRUE);
    // 表示する文字幅を取得
    int textWidth_C = GetDrawFormatStringWidth("%d", curse);
    // 中央になるようにXを調整
    int CurseText = (x+2) - (textWidth_C / 2);
    // 描画
    DrawFormatStringToHandle(CurseText, y + yDist, GetColor(230, 230, 230),
        Master::mpFontManager->GetStatusFontHandle(), "%d", curse);

}

void Enemy::Draw()
{
    Object2D::Draw();

    // アニメーションの描画
    for (auto& p : parts)
    {
        DrawPart(p, mvPosition.x, mvPosition.y, 0.4f);
    }


    // 表示を開始する基準位置
    int x = (int)mvPosition.x - 80;
    int y = (int)mvPosition.y + 110;
    // バフ・デバフ（ここから下に伸びる）
    int buffX = x;
	int drawY = 20; // 基準位置
    //アイコンの描画
    int iconX = x - 50;
    int iconY = y += 20;

    // バフ
    for (const auto& b : buffs)
    {
        //DrawFormatString(buffX +130 , y +20, GetColor(255, 200, 0), "[昇:%d|%dT]", b.value, b.turns);
        //DrawFormatString(iconX, drawY, GetColor(255, 200, 0), "[昇:%d|%dT]", b.value, b.turns);
        drawY += 20; //  縦に並べるならY増やす
    }
    // デバフ
    for (const auto& d : debuffs)
    {
        //毒のアイコン描画
        DrawGraph(buffX +20, iconY, mnPoisonHandle, TRUE);
        //DrawFormatString(buffX +60, iconY +=20, GetColor(100, 255, 100), "s:%d|%dT", d.value, d.turns);
    }
}

void Enemy::HpGaugeUpdate()
{
    // ダメージを受けてからHPバーが減るまでのタイマーを進める
    DamageBarTimer++;

    // タイマーが一定以上になったら、表示上のHPを実際のHPに近づける
    if (DamageBarTimer >= DAMAGE_BAR_FRAME)
    {
        // 表示上のHPが実際のHPより多い場合、少しずつ減らす
        if (displayDamageHp > hp)
        {
            displayDamageHp -= (int)DamageBarSpeed;

            // 下回ったら止める
            if (displayDamageHp < hp)
            {
                displayDamageHp = hp;
            }
        }
        if (displayDamageHp < hp)
        {
            displayDamageHp = hp; // 目標値に達したら正確なHPに合わせる
        }

		DamageBarTimer = 0;
    }

    // HPバー計算用
    int hpForGauge = hp;

    if (hpForGauge > 0 && hpForGauge < mnHpLow)
    {
        hpForGauge = mnHpLow;
    }

    // ダメージバー計算用
    int damageHpForGauge = displayDamageHp;

    if (damageHpForGauge > 0 && damageHpForGauge < mnHpLow)
    {
        damageHpForGauge = mnHpLow;
    }

    // 幅計算
    gaugeWidth = (int)((float)hpForGauge / maxHp * width);

    gaugeDamegeWidth = (int)((float)damageHpForGauge / maxHp * width);
}

void Enemy::CardSet()
{
    if (enemyhand.empty()) return; // 手札が空なら何もしない（安全策）

    float centerX = Utility::SCREEN_WIDTH / 2.0f;  // 円の中心 画面の真ん中
    float centerY = -200.0f; // 円の中心 画面の上の方
    float radiusX = 450.0f;  // 横の半径
    float radiusY = 200.0f;  // 縦の半径

    // 枚数入れている
    int currentNum = (int)enemyhand.size();
    // カードとカードの間の角度　14にしている
    float stepAngle = (14.0f * (DX_PI_F / 180.0f));
    // カード全体の幅の半分の角度
    float totalHalfAngle = (stepAngle * (currentNum - 1)) / 2.0f;
    // 一番左のカードの開始角度
    float startAngle = (90.0f * (DX_PI_F / 180.0f)) - totalHalfAngle;


    int i = 0;
    for (auto* card : enemyhand)
    {
        if (card != nullptr) // 中身が空でないか
        {
            float angle = startAngle + (stepAngle * i);
            float tx = centerX + cosf(angle) * radiusX;  // 角度から横の位置を出す
            float ty = centerY + sinf(angle) * radiusY;  // 縦の位置を出す

            // 円の方向を向くようにして扇にしている
            float trot = angle - (DX_PI_F / 2.0f) + DX_PI_F;

            // 位置と回転を設定
            card->SetCardPosition(tx, ty, trot);
            card->SetScale(0.2f);
        }
        i++; // 次のカード用のカウントアップ
    }
}


// 変更　小池
void Enemy::EnemyStartBattle()
{
    enemyhand.clear();

    enemyhand.push_back(new Card(Card::EnemyCardDataById(1))); // 攻撃
    enemyhand.push_back(new Card(Card::EnemyCardDataById(2))); // 防御
    enemyhand.push_back(new Card(Card::EnemyCardDataById(3))); // バフ
    enemyhand.push_back(new Card(Card::EnemyCardDataById(4))); // 毒
    enemyhand.push_back(new Card(Card::EnemyCardDataById(5))); // 弱体
}

void Enemy::EnemyCardDraw(int workScreen, int fontHandle, Card* topCard)
{
    // 手札を描く（topCard は除外）
    for (auto* c : enemyhand)
    {
        if (c == nullptr) continue;
        if (c == topCard) continue;
        c->Draw(workScreen, fontHandle, true);
    }
}