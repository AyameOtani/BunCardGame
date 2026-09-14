#include "Card.h"
#include <iostream>
#include "DxLib.h"
#include "Mouse.h"
#include "Utility.h"
#include "SceneManager.h"

Card::Card(const CardData& cardData)
    : data(cardData) // データコピー
    , mfX(0.0f)
    , mfY((float)Utility::SCREEN_HEIGHT - 100.0f)
    , mfRotation(0.0f)
    , mfScale(0.05f) // 初期サイズ小さく
    , mnImageHandle(-1)
    , mnFrameHandle(-1)
    , mbHover(false)
    , mbDragging(false)
    , mbKeep(false)
    , mfTargetX(0.0f)
    , mfTargetY(0.0f)
    , mfTargetRotation(0.0f)
{
    // 画像の読み込み
    // 表画像
    mnOmoteHandle = LoadGraph(data.cardFilename.c_str());

    // 裏画像 (の方)
    mnUraHandle = LoadGraph("Resource/Card/enemy_card_front.png");

    // 最初は表を表示
    mbShowFront = true;


    // コストに応じた枠画像の読み込み
    std::string framePath = "";

    // フラグをチェック
    if (data.isSpecial)
    {
        // 必殺技専用の枠
        framePath = "Resource/Card/player_card_special.png";
    }
    else
    {
        switch (GetCostType())
        {
        case CostType::COST_0: framePath = "Resource/Card/player_card_cost0.png"; break;
        case CostType::COST_1: framePath = "Resource/Card/player_card_cost1.png"; break;
        case CostType::COST_2: framePath = "Resource/Card/player_card_cost2.png"; break;
        default:               framePath = "Resource/Card/player_card_cost3.png"; break;
        }
    }

    // 枠の読み込み
    mnFrameHandle = LoadGraph(framePath.c_str());
}


Card::Card(const EnemyCardData& enemyCardData)
    : enemyData(enemyCardData) // データコピー
    , mfX((float)Utility::SCREEN_WIDTH / 2)
    , mfY(-200.0f)
    , mfRotation(0.0f)
    , mfScale(0.05f) // 初期サイズ小さく
    , mnImageHandle(-1)
    , mnFrameHandle(-1)
    , mbHover(false)
    , mbDragging(false)
    , mbKeep(false)
    , mfTargetX(0.0f)
    , mfTargetY(0.0f)
    , mfTargetRotation(0.0f)
{
    // dataの中身も空にしておくと、Draw関数で変な文字が出ない
    data.desciption = "";

    mnFrameHandle = LoadGraph("Resource/Card/enemy_card_front.png");

    // 表裏画像読み込み
    // 表 = 行動カード画像
    mnOmoteHandle =
        LoadGraph(
            enemyCardData.frontImageName.c_str()
        );

    // 裏 = 共通裏面画像
    mnUraHandle =
        LoadGraph(
            enemyCardData.backImageName.c_str()
        );

    // 最初は裏向き
    mbShowFront = false;

    mfNormalScale = 0.68f;
    mfTargetScale = mfNormalScale; // 目標サイズも 0.34 に設定する
    mfScale = mfNormalScale;       // 初期サイズも 0.34 にしておく
}


// 変更　小池　5/15
Card::~Card()
{
    // ハンドル解放
    if (mnOmoteHandle != -1) DeleteGraph(mnOmoteHandle);
    if (mnUraHandle != -1) DeleteGraph(mnUraHandle);
    if (mnFrameHandle != -1) DeleteGraph(mnFrameHandle);
}


// ホバーされているとかを外部からとるやつ
void Card::SetFocus(bool hover, bool dragging)
{
    mbHover = hover;
    mbDragging = dragging;
}


// 目標の位置を設定するため　Playerがカード達の位置を設定するため
void Card::SetCardPosition(float _x, float _y, float _rot)
{
    mfTargetX = _x;
    mfTargetY = _y;
    mfTargetRotation = _rot;
}


// 今の位置を変える関数
void Card::SetNowPosition(float nx, float ny, float nrot)
{
    mfX = nx;
    mfY = ny;
    mfRotation = nrot;
}


void Card::LateUpdate()
{
    // 座標の補間  　大谷
    if (!mbDragging)
    {
        // イージング補間 っぽいやつ
        float ty = mbHover ? mfTargetY - 50.0f : mfTargetY;
        float tr = mbHover ? 0.0f : mfTargetRotation;
        float ts = mbHover ? mfMaxScale : mfTargetScale;

        // ターゲットとどのくらい離れているか
        float diffX = mfTargetX - mfX;
        float diffY = ty - mfY;
        float diffRot = tr - mfRotation;
        float diffScale = ts - mfScale;


        // 差分んがあるなら演出中フラグON
        // 動くようにした   途中でも出したいから広めに判定
        if (fabsf(diffX) > 200.0f ||
            fabsf(diffY) > 200.0f ||
            fabsf(diffRot) > 2.0f)
        {
            mbDirection = true;
        }
        else
        {
            mbDirection = false;
        }


        // 速さをかけてジョジョに動かす
        mfX += diffX * mfMoveSpeed;
        mfY += diffY * mfMoveSpeed;
        mfRotation += diffRot * mfMoveSpeed;

        // ホバーの時はすぐ大きくさせたい
        if (mbHover || mbDragging)
        {
            mfScale = mfMaxScale;
        }
        else
        {
            mfScale += diffScale * mfMoveSpeed;
        }
    }
    else
    {
        mfScale = mfMaxScale; // ドラック中はすぐ大きく
        mbDirection = false; // 演出OFF
    }

    // カード回転演出

    if (mbFlipping)
    {
        // 横幅を縮める
        mfFlipScaleX -= 0.08f;

        // 真横になった瞬間
        if (mfFlipScaleX <= 0.0f)
        {
            mfFlipScaleX = 0.0f;

            // 表裏切り替え
            mbShowFront = !mbShowFront;

            // 広げる用
            mfFlipScaleX = -1.0f;
        }

        // 広がる
        if (mfFlipScaleX < 0.0f)
        {
            mfFlipScaleX += 0.08f;

            // 完了
            if (mfFlipScaleX >= 1.0f)
            {
                mfFlipScaleX = 1.0f;
                mbFlipping = false;
            }
        }
    }
}


void Card::StartFlip()
{
    // すでに回転中なら何もしない
    if (mbFlipping) return;

    mbFlipping = true;

    // 裏から始める
    mbShowFront = false;
    mfFlipScaleX = 1.0f;
}


// 追加：外部から表裏フラグを設定する実装（GameScene から呼ぶため）
void Card::SetShowFront(bool front)
{
    mbShowFront = front;
}


void Card::Update()
{
    //// ホバー中(マウス乗ってたら)またはドラッグ中なら大きく
    //if (mbHover || mbDragging)
    //{
    //    mfScale = mfMaxScale;
    //}
    //else
    //{
    //    mfScale = mfNormalScale;
    //}
}


// 追加：表→裏に回す（逆向き）
void Card::StartFlipReverse()
{
    if (mbFlipping) return;

    mbFlipping = true;

    // 今回は表から裏にするため最初は表にしておく
    mbShowFront = true;
    mfFlipScaleX = 1.0f;
}


bool Card::IsFlipping() const
{
    return mbFlipping;
}


bool Card::IsShowFront() const
{
    return mbShowFront;
}


// OBB判定 マウスが乗っているかの判定のやつ
bool Card::CheckHover(float mx, float my)
{
    if (mnFrameHandle == -1) return false;

    int w, h;
    GetGraphSize(mnFrameHandle, &w, &h); // グラフのサイズを取得

    // 元の画像にスケールをかけて画面に表示されている今の大きさを出す
    float sw = (float)w * mfScale;
    float sh = (float)h * mfScale;

    // カードの中心からマウスとの距離を計算s
    // ローカル座標へ変更してる
    float dx = mx - mfX;  // マウスX　- カードX
    float dy = my - mfY;


    // 座標を回転させる公式
    float s = sinf(-mfRotation);  // マイナスの回転角のサイン   縦方向
    float c = cosf(-mfRotation);  // マイナスの回転角のコサイン 横方向
    float rx = dx * c - dy * s; // 逆回転させた時のマウス位置Ｘ
    float ry = dx * s + dy * c;

    // マウスがカードの範囲内にあるかのチェック
    return (fabsf(rx) <= sw / 2.0f && fabsf(ry) <= sh / 2.0f);
}


// 描画
void Card::Draw(int workScreen, int fontHandle, bool isFocus, bool isPickOption)
{
    if (mnFrameHandle == -1) return;

    int w, h;
    GetGraphSize(mnFrameHandle, &w, &h);

    // 描画先を作業用画面に切り替えてカードの中身を描く
    SetDrawScreen(workScreen);
    ClearDrawScreen();

    // イラストと枠を描画
    // 追加　小池　5/15
    // 今表示する画像を決める
    // 枠
    DrawGraph(0, 0, mnFrameHandle, TRUE);

    // 敵カードなら表裏を使う
    if (enemyData.mbEnemyFlag)
    {
        int currentHandle = mbShowFront ? mnOmoteHandle : mnUraHandle;

        if (currentHandle != -1)
        {
            DrawExtendGraph(0, 0, w, h, currentHandle, TRUE);
        }
    }
    else
    {
        // 普通のカードは今まで通り
        DrawExtendGraph(0, 0, w, h, mnImageHandle, TRUE);
    }


    DrawFormatStringToHandle(
        (int)(w * 0.09f),
        (int)(h * 0.6f),
        GetColor(100, 100, 100),
        fontHandle,
        "%s",
        data.desciption.c_str()
    );


    // キープ中のボックスを描画（カード本体と同じ画面内に描く）
    if (mbKeep)
    {
        // 作業用画面内での位置
        int bx = w / 2 - 30;
        int by = 30; // 上から30ピクセルくらいの場所

        // オレンジ色のボックス
        DrawBox(
            bx - 60,
            by - 20,
            bx + 60,
            by + 20,
            GetColor(255, 165, 0),
            TRUE
        );

        DrawBox(
            bx - 60,
            by - 20,
            bx + 60,
            by + 20,
            GetColor(255, 255, 255),
            FALSE
        );

        // 文字
        DrawString(
            bx - 22,
            by - 8,
            "KEEP",
            GetColor(255, 255, 255)
        );
    }


    // 描画先を裏画面に戻す
    SetDrawScreen(DX_SCREEN_BACK);

    // 演出用の計算
    float drawRot = (mbHover || mbDragging) ? 0.0f : mfRotation;
    float drawX = mbDragging ? Mouse::x : mfX;
    float drawY = mfY;

    if (mbDragging)
    {
        drawY = Mouse::y;
    }
    else if (mbHover)
    {
        if (isPickOption)
        {
            drawY = mfY;
        }
        else
        {
            drawY = Utility::SCREEN_HEIGHT - 200;
        }
    }

    // キープ中なら沈ませている
    if (mbKeep)
    {
        drawY += 60.0f;
    }

    // 自分以外の誰かが主役なら 自分を暗くする
    if (isFocus && !mbHover && !mbDragging)
    {
        SetDrawBright(200, 200, 200); // 輝度を下げて暗くする
    }
    else
    {
        SetDrawBright(255, 255, 255); // 主役、または誰も選んでない時は明るく
    }

    // 最終的な描画
    //DrawRotaGraphF(drawX, drawY, scale, drawRot, workScreen, TRUE);
    // 回転っぽく見せるために横方向だけ縮める　追加　小池　5/15
    // 横方向だけ縮めて回転っぽく見せる
    DrawRotaGraph3F(
        drawX,
        drawY,
        w / 2.0f,
        h / 2.0f,
        mfScale * fabsf(mfFlipScaleX), // 横スケール
        mfScale,                     // 縦スケール
        drawRot,
        workScreen,
        TRUE
    );

    // 忘れずに輝度を元に戻す
    SetDrawBright(255, 255, 255);
}


// コストによって分けるやつ
Card::CostType Card::GetCostType() const
{
    // コストを取得してタイプ分けてる
    switch (data.cost)
    {
    case 0:
        return Card::CostType::COST_0;

    case 1:
        return Card::CostType::COST_1;

    case 2:
        return Card::CostType::COST_2;

    case 3:
        return Card::CostType::COST_3;

    default: // 3以上は全部COST_3にしとく　 全然変更OK 記述大谷
        return Card::CostType::COST_3;
    }
}


// カードそれぞれの設定
Card::CardData Card::GetCardDataById(int id)
{
    Card::CardData d;
    d.id = id;
    d.isExhaust = false; // 一回きりじゃないのをデフォにしておく

    // これでダメージ効果があるかどうかを判定するフラグ 菊池
    bool hasDamage = false;

    // これカードの種類分かくでー
    // 名前　コスト　イラスト画像　効果説明　カード効果とかある  詳しくはヘッダー見てくれ  大谷
    switch (id)
    {
    case 1:
        d.name = "カード1";
        d.cost = 1;
        d.cardFilename = "Resource/CardEffect/attack.png";
        d.desciption = "5ダメージを与える";
        d.effects = { {EffectType::DAMAGE, 5, 0} };
        break;

    case 2:
        d.name = "カード2";
        d.cost = 1;
        d.cardFilename = "Resource/CardEffect/block.png";
        d.desciption = "ブロックを5獲得";
        d.effects = { {EffectType::BLOCK, 5, 0} };
        break;

    case 3:
        d.name = "カード3";
        d.cost = 1;
        d.cardFilename = "Resource/CardEffect/attack_and_poison.png";
        d.desciption = "5ダメージを与える\n毒を3与える";
        d.effects = {
            {EffectType::DAMAGE, 5, 0},
            {EffectType::APPLY_POISON, 3, 0}
        };

        // 菊池
        // ダメージ効果があるかどうかを判定するフラグを立てる
        for (const auto& e : d.effects)
        {
            if (e.type == EffectType::DAMAGE)
            {
                hasDamage = true;
                break;
            }
        }

        // ダメージ効果があるなら、他の効果にも親にダメージ効果があるフラグを立てる
        for (auto& e : d.effects)
        {
            e.parentHasDamage = hasDamage;
        }

        break;

    case 4:
        d.name = "カード4";
        d.cost = 0;
        d.cardFilename = "Resource/CardEffect/draw_card.png";
        d.desciption = "カードを2枚引く";
        d.effects = { {EffectType::DRAW, 2, 0} };
        break;

    case 5:
        d.name = "カード5";
        d.cost = 0;
        d.cardFilename = "Resource/CardEffect/mp.png";
        d.desciption = "MPを1回復";
        d.effects = { {EffectType::ENERGY_MP, 1, 0} };
        break;

    case 6:
        d.name = "カード6";
        d.cost = 2;
        d.cardFilename = "Resource/CardEffect/block.png";
        d.desciption = "ブロックを12獲得";
        d.effects = { {EffectType::BLOCK, 12, 0} };
        break;

    case 7:
        d.name = "カード7";
        d.cost = 2;
        d.cardFilename = "Resource/CardEffect/poison.png";
        d.desciption = "毒を5与える\n3ターン継続";
        d.effects = { {EffectType::APPLY_POISON, 5, 3} };
        break;

    case 8:
        d.name = "カード8";
        d.cost = 1;
        d.cardFilename = "Resource/CardEffect/attack_and_draw.png";
        d.desciption = "6ダメージを与える\nカードを1枚引く";
        d.effects = {
            {EffectType::DAMAGE, 6, 0},
            {EffectType::DRAW, 1, 0}
        };

        // 菊池
        // ダメージ効果があるかどうかを判定するフラグを立てる
        for (const auto& e : d.effects)
        {
            if (e.type == EffectType::DAMAGE)
            {
                hasDamage = true;
                break;
            }
        }

        // ダメージ効果があるなら、他の効果にも親にダメージ効果があるフラグを立てる
        for (auto& e : d.effects)
        {
            e.MpHeel = hasDamage;
        }

        break;

    case 9:
        d.name = "カード9";
        d.cost = 3;
        d.isExhaust = true;
        d.cardFilename = "Resource/CardEffect/attack.png";
        d.desciption = "20ダメージを与える\n使い切り";
        d.effects = { {EffectType::DAMAGE, 20, 0} };
        break;

    case 10:
        d.name = "カード10";
        d.cost = 0;
        d.isExhaust = true;
        d.cardFilename = "Resource/CardEffect/mp_and_draw.png";
        d.desciption = "MPを1獲得\nカードを2枚引く\n使い切り";
        d.effects = {
            {EffectType::ENERGY_MP, 1, 0},
            {EffectType::DRAW, 2, 0}
        };

        // 菊池
        // ダメージ効果があるかどうかを判定するフラグを立てる
        for (const auto& e : d.effects)
        {
            if (e.type == EffectType::ENERGY_MP)
            {
                hasDamage = true;
                break;
            }
        }

        // ダメージ効果があるなら、他の効果にも親にダメージ効果があるフラグを立てる
        for (auto& e : d.effects)
        {
            e.mpHeel_1 = hasDamage;
        }

        break;


    case 11:
        d.name = "カード11";
        d.cost = 1;
        d.cardFilename = "Resource/CardEffect/attack.png";
        d.desciption = "4ダメージを\n2回与える";
        d.effects = { {EffectType::DAMAGE, 4, 2} };

        // 菊池
        // ダメージ効果があるかどうかを判定するフラグを立てる
        for (const auto& e : d.effects)
        {
            if (e.type == EffectType::DAMAGE)
            {
                hasDamage = true;
                break;
            }
        }

        // ダメージ効果があるなら、他の効果にも親にダメージ効果があるフラグを立てる
        for (auto& e : d.effects)
        {
            e.DoubleAttack = hasDamage;
        }

        break;


    case 12:
        d.name = "カード12";
        d.cost = 0;
        d.cardFilename = "Resource/CardEffect/attack.png";
        d.desciption = "4ダメージを与える";
        d.effects = { {EffectType::DAMAGE, 4, 0} };
        break;


    case 13:
        d.name = "カード13";
        d.cost = 1;
        d.cardFilename = "Resource/CardEffect/buff.png";
        d.desciption = "このターン\n与えるダメージが\n6増える";
        d.effects = { {EffectType::APPLY_BUFF, 6, 1} };
        break;


    case 14:
        d.name = "カード14";
        d.cost = 0;
        d.cardFilename = "Resource/CardEffect/block.png";
        d.desciption = "ブロックを4獲得";
        d.effects = { {EffectType::BLOCK, 4, 0} };
        break;


    case 15:
        d.name = "カード15";
        d.cost = 1;
        d.cardFilename = "Resource/CardEffect/attack_and_block.png";
        d.desciption = "5ダメージを与える\n4ブロックを獲得";
        d.effects = {
            {EffectType::DAMAGE, 5, 0},
            {EffectType::BLOCK, 4, 0}
        };

        // 菊池
        // ダメージ効果があるかどうかを判定するフラグを立てる
        for (const auto& e : d.effects)
        {
            if (e.type == EffectType::DAMAGE)
            {
                hasDamage = true;
                break;
            }
        }

        // ダメージ効果があるなら、他の効果にも親にダメージ効果があるフラグを立てる
        for (auto& e : d.effects)
        {
            e.BlockBuff = hasDamage;
        }

        break;


    case 16:
        d.name = "カード16";
        d.cost = 1;
        d.cardFilename = "Resource/CardEffect/debuf_easy.png";
        d.desciption = "敵が与える\nダメージが\n2減少する";
        d.effects = { {EffectType::APPLY_WEAKEN, 2, 2} };
        break;


    case 17:
        d.name = "カード17";
        d.cost = 3;
        d.cardFilename = "Resource/CardEffect/curse.png";
        d.desciption = "呪いを1付与する\n5回で即死";
        d.effects = { {EffectType::APPLY_CURSE, 1, 0} };
        break;


    default:
        d.name = "カード17";
        d.cost = 3;
        d.cardFilename = "Resource/CardEffect/curse.png";
        d.desciption = "呪いを1付与する\n5回で即死";
        d.effects = { {EffectType::APPLY_CURSE, 1, 0} };
        break;
    }

    return d;
}


//カードの必殺技の設定
Card::CardData Card::SpecialCardDataById(int id)
{
    Card::CardData d;
    d.id = id;
    d.isExhaust = false; // 一回きりじゃないのをデフォにしておく
    d.isSpecial = true; // 必殺をON

    switch (id)
    {
    case 1:
        d.name = "必殺技1";
        d.cardFilename = "Resource/CardEffect/special_1.png";
        d.desciption = "30ダメージを与える\n2枚カードを引く";
        d.effects = {
            {EffectType::DAMAGE, 30, 0, true},
            {EffectType::DRAW, 2, 0, true}
        };
        break;

    case 2:
        d.name = "必殺技2";
        d.cardFilename = "Resource/CardEffect/special_2.png";
        d.desciption = "MPを2回復する\n2枚カードを引く";
        d.effects = {
            {EffectType::ENERGY_MP, 2, 0, true},
            {EffectType::DRAW, 2, 0, true}
        };
        break;

    case 3:
        d.name = "必殺技3";
        d.cardFilename = "Resource/CardEffect/special_3.png";
        d.desciption = "与えるダメージが\n10増える\n敵の攻撃力を10減少\n2ターン継続";
        d.effects = {
            {EffectType::APPLY_WEAKEN, 10, 0, true},
            {EffectType::APPLY_ATTACK_DOWN, 10, 2, true}
        };
        break;
    }

    return d;
}


// 敵のカードの設定
Card::EnemyCardData Card::EnemyCardDataById(int id)
{
    Card::EnemyCardData d;
    d.id = id;
    d.mbEnemyFlag = true; // 敵のカードをON

    // 共通の裏面（共通画像）
    d.backImageName = "Resource/Card/enemy_card_front.png";

    switch (id)
    {
    case 1:
        d.enemyName = "攻撃カード";
        d.frontImageName = "Resource/Card/enemy_card_attack.png";
        d.desciption = "攻撃";
        break;

    case 2:
        d.enemyName = "防御カード";
        d.frontImageName = "Resource/Card/enemy_card_guard.png";
        d.desciption = "防御";
        break;

    case 3:
        d.enemyName = "バフカード";
        d.frontImageName = "Resource/Card/enemy_card_buff.png";
        d.desciption = "バフ";
        break;

    case 4:
        d.enemyName = "毒カード";
        d.frontImageName = "Resource/Card/enemy_card_poison.png";
        d.desciption = "毒";
        break;

    case 5:
        d.enemyName = "弱体カード";
        d.frontImageName = "Resource/Card/enemy_card_debuff.png";
        d.desciption = "弱体";
        break;

    case 6:
        d.enemyName = "敵の必殺カード";
        d.frontImageName = "Resource/Card/enemy_card_warning.png";
        d.desciption = "敵の必殺";
        break;

    default:
        d.enemyName = "敵のカード";
        d.frontImageName = "Resource/Card/enemy_card_attack.png";
        d.desciption = "";
        break;
    }

    return d;
}


// 枠のサイズを返すやつ
void Card::GetFrameSize(int* w, int* h)
{
    if (mnFrameHandle != -1)
    {
        GetGraphSize(mnFrameHandle, w, h);
    }
    else
    {
        // ないなら0にしてる
        *w = 0;
        *h = 0;
    }
}


void Card::ResetFrameByCost()
{
    // 古い枠を一旦消す
    if (mnFrameHandle != -1)
    {
        DeleteGraph(mnFrameHandle);
        mnFrameHandle = -1;
    }

    // 今のコストを見てパスを決定
    std::string framePath = "";

    switch (GetCostType()) // GetCostTypeは今のdata.costを判定してくれる
    {
    case CostType::COST_0:
        framePath = "Resource/Card/player_card_cost0.png";
        break;

    case CostType::COST_1:
        framePath = "Resource/Card/player_card_cost1.png";
        break;

    case CostType::COST_2:
        framePath = "Resource/Card/player_card_cost2.png";
        break;

    default:
        framePath = "Resource/Card/player_card_cost3.png";
        break;
    }

    // 新しい画像をロード
    mnFrameHandle = LoadGraph(framePath.c_str());
}