#include "Card.h"
#include <iostream>
#include "DxLib.h"
#include "Mouse.h"
#include "Utility.h"
#include "SceneManager.h"

Card::Card(const CardData& cardData)
    : data(cardData) // データコピー
    , x(0.0f)
    , y((float)Utility::SCREEN_HEIGHT - 100.0f)
    , rot(0.0f)
    , scale(0.05f) // 初期サイズ小さく
    , imgHandle(-1)
    , frameHandle(-1)
    , isHover(false)
    , isDragging(false)
    , isKeep(false)
{   
    // 画像の読み込み
    // 表画像
    omoteHandle = LoadGraph(data.cardFilename.c_str());

    // 裏画像 (効果の方)
    uraHandle = LoadGraph("Resource/Card/enemy_card_warning.png");

    // 最初は表を表示
    showFront = true;


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
    frameHandle = LoadGraph(framePath.c_str());
}


Card::Card(const EnemyCardData& enemyData)
    : enemyData(enemyData) // データコピー
    , x((float)Utility::SCREEN_WIDTH / 2)
    , y(-200.0f)
    , rot(0.0f)
    , scale(0.05f) // 初期サイズ小さく
    , imgHandle(-1)
    , frameHandle(-1)
    , isHover(false)
    , isDragging(false)
    ,isKeep(false)
{   
    // dataの中身も空にしておくと、Draw関数で変な文字が出ない
    data.desciption = "";

    frameHandle = LoadGraph("Resource/Card/enemy_card_front.png");

    // 表裏画像読み込み
    // 表 = 行動カード画像
    omoteHandle =
        LoadGraph(
            enemyData.frontImageName.c_str()
        );

    // 裏 = 共通裏面画像
    uraHandle =
        LoadGraph(
            enemyData.backImageName.c_str()
        );

    // 最初は裏向き
    showFront = false;

    scaleNormal = 0.68f;
    targetScale = scaleNormal; // 目標サイズも 0.34 に設定する
    scale = scaleNormal;       // 初期サイズも 0.34 にしておく
}

// 変更　小池　5/15
Card::~Card()
{
    // ハンドル解放
    if (omoteHandle != -1) DeleteGraph(omoteHandle);
    if (uraHandle != -1) DeleteGraph(uraHandle);
    if (frameHandle != -1) DeleteGraph(frameHandle);
}

// ホバーされているとかを外部からとるやつ
void Card::SetFocus(bool hover, bool dragging)
{
    isHover = hover;
    isDragging = dragging;
}


// 目標の位置を設定するため　Playerがカード達の位置を設定するため
void Card::SetCardPosition(float _x, float _y, float _rot)
{
    targetX = _x;
    targetY = _y;
    targetRot = _rot;
}

// 今の位置を変える関数
void Card::SetNowPosition(float nx, float ny, float nrot)
{
    x = nx;
    y = ny;
    rot = nrot;
}


void Card::LateUpdate()
{
    // 座標の補間  　大谷
    if (!isDragging)
    {
        // イージング補間 っぽいやつ
        float ty = isHover ? targetY - 50.0f : targetY;
        float tr = isHover ? 0.0f : targetRot;
        float ts = isHover ? scaleMax : targetScale;

        // ターゲットとどのくらい離れているか
        float diffX = targetX - x;
        float diffY = ty - y;
        float diffRot = tr - rot;
        float diffScale = ts - scale;


        // 差分んがあるなら演出中フラグON
        // 動くようにした   途中でも出したいから広めに判定
        if (fabsf(diffX) > 200.0f || fabsf(diffY) > 200.0f || fabsf(diffRot) > 2.0f)
        {
            mbDirection = true;
        }
        else
        {
            mbDirection = false;
        }


        // 速さをかけてジョジョに動かす
        x += diffX * mfSpeed;
        y += diffY * mfSpeed;
        rot += diffRot * mfSpeed;

        // ホバーの時はすぐ大きくさせたい
        if (isHover || isDragging)
        {
            scale = scaleMax;
        } 
        else
        {
            scale += diffScale * mfSpeed;
        }
    }
    else
    {
        scale = scaleMax; // ドラック中はすぐ大きく
        mbDirection = false; // 演出OFF
    }

    // カード回転演出

    if (isFlipping)
    {
        // 横幅を縮める
        flipScaleX -= 0.08f;

        // 真横になった瞬間
        if (flipScaleX <= 0.0f)
        {
            flipScaleX = 0.0f;

            // 表裏切り替え
            showFront = !showFront;

            // 広げる用
            flipScaleX = -1.0f;
        }

        // 広がる
        if (flipScaleX < 0.0f)
        {
            flipScaleX += 0.08f;

            // 完了
            if (flipScaleX >= 1.0f)
            {
                flipScaleX = 1.0f;
                isFlipping = false;
            }
        }
    }
}

void Card::StartFlip()
{
    // すでに回転中なら何もしない
    if (isFlipping) return;

    isFlipping = true;

    // 裏から始める
    showFront = false;
    flipScaleX = 1.0f;
}

// 追加：外部から表裏フラグを設定する実装（GameScene から呼ぶため）
void Card::SetShowFront(bool front)
{
    showFront = front;
}

void Card::Update()
{
    //// ホバー中(マウス乗ってたら)またはドラッグ中なら大きく
    //if (isHover || isDragging)
    //{
    //    scale = scaleMax;
    //}
    //else
    //{
    //    scale = scaleNormal;
    //}
}


// 追加：表→裏に回す（逆向き）
void Card::StartFlipReverse()
{
    if (isFlipping) return;
    isFlipping = true;
    // 今回は表から裏にするため最初は表にしておく
    showFront = true;
    flipScaleX = 1.0f;
}

bool Card::IsFlipping() const
{
    return isFlipping;
}

bool Card::IsShowFront() const
{
    return showFront;
}

// OBB判定 マウスが乗っているかの判定のやつ
bool Card::CheckHover(float mx, float my)
{
    if (frameHandle == -1) return false;

    int w, h;
    GetGraphSize(frameHandle, &w, &h); // グラフのサイズを取得

    // 元の画像にスケールをかけて画面に表示されている今の大きさを出す
    float sw = (float)w * scale;
    float sh = (float)h * scale;

    // カードの中心からマウスとの距離を計算s
    // ローカル座標へ変更してる
    float dx = mx - x;  // マウスX　- カードX
    float dy = my - y;


    // 座標を回転させる公式
    float s = sinf(-rot);  // マイナスの回転角のサイン   縦方向
    float c = cosf(-rot);  // マイナスの回転角のコサイン 横方向
    float rx = dx * c - dy * s; // 逆回転させた時のマウス位置Ｘ
    float ry = dx * s + dy * c;

    // マウスがカードの範囲内にあるかのチェック
    return (fabsf(rx) <= sw / 2.0f && fabsf(ry) <= sh / 2.0f);
}


// 描画
void Card::Draw(int workScreen, int fontHandle, bool isFocus, bool isPickOption)
{
    if (frameHandle == -1) return;
    int w, h;
    GetGraphSize(frameHandle, &w, &h);

    // 描画先を作業用画面に切り替えてカードの中身を描く
    SetDrawScreen(workScreen);
    ClearDrawScreen();

    // イラストと枠を描画
	// 追加　小池　5/15
    // 今表示する画像を決める
    // 枠
    DrawGraph(0, 0, frameHandle, TRUE);

    // 敵カードなら表裏を使う
    if (enemyData.mbEnemyFlag)
    {
        int currentHandle = showFront ? omoteHandle : uraHandle;

        if (currentHandle != -1)
        {
            DrawExtendGraph(0, 0, w, h, currentHandle, TRUE);
        }
    }
    else
    {
        // 普通のカードは今まで通り
        DrawExtendGraph(0, 0, w, h, imgHandle, TRUE);
    }


    DrawFormatStringToHandle((int)(w * 0.09f), (int)(h * 0.6f), GetColor(100, 100, 100), fontHandle, "%s", data.desciption.c_str());

    
    // キープ中のボックスを描画（カード本体と同じ画面内に描く）
    if (isKeep)
    {
        // 作業用画面内での位置
        int bx = w / 2 - 30;
        int by = 30; // 上から30ピクセルくらいの場所

        // オレンジ色のボックス
        DrawBox(bx - 60, by - 20, bx + 60, by + 20, GetColor(255, 165, 0), TRUE);
        DrawBox(bx - 60, by - 20, bx + 60, by + 20, GetColor(255, 255, 255), FALSE);

        // 文字
        DrawString(bx - 22, by - 8, "KEEP", GetColor(255, 255, 255));
    }


    // 描画先を裏画面に戻す
    SetDrawScreen(DX_SCREEN_BACK);

    // 演出用の計算
    float drawRot = (isHover || isDragging) ? 0.0f : rot; // 選択されてたら真っすぐ　違うならrot
    float drawX = isDragging ? Mouse::x : x; // マウスの位置にするかOBBの位置にするか
    float drawY = y; // デフォルトは SetCardPosition で決めた位置

    if (isDragging)
    {
        drawY = Mouse::y; // ドラッグ中はマウス位置
    }
    else if (isHover)
    {
        if (isPickOption)
        {
            drawY = y; // 選択中なら y座標は変えない
        }
        else
        {
            drawY = Utility::SCREEN_HEIGHT - 200; // 通常の手札なら上に浮かせる
        }
    }

    // キープ中なら沈ませている
    if (isKeep)
    {
        drawY += 60.0f;
    }

    // 自分以外の誰かが主役なら 自分を暗くする
    if (isFocus && !isHover && !isDragging)
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
        scale * fabsf(flipScaleX), // 横スケール
        scale,                     // 縦スケール
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
        d.effects = { {EffectType::DAMAGE, 5, 0}, {EffectType::APPLY_POISON, 3, 0} };

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
        d.effects = { {EffectType::DAMAGE, 6, 0}, {EffectType::DRAW, 1, 0} };

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
        d.effects = { {EffectType::ENERGY_MP, 1, 0}, {EffectType::DRAW, 2, 0} };

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
        d.effects = { {EffectType::DAMAGE, 5, 0}, {EffectType::BLOCK, 4, 0} };

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
        d.effects = { {EffectType::APPLY_CURSE, 1,0} };
        break;


    default:
        d.name = "カード17";
        d.cost = 3;
        d.cardFilename = "Resource/CardEffect/curse.png";
        d.desciption = "呪いを1付与する\n5回で即死";
        d.effects = { {EffectType::APPLY_CURSE, 1,0} };
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
        d.effects = { {EffectType::DAMAGE, 30, 0, true},{EffectType::DRAW,2,0,true} };
        break;

    case 2:

        d.name = "必殺技2";
        d.cardFilename = "Resource/CardEffect/special_2.png";
        d.desciption = "MPを2回復する\n2枚カードを引く";
        d.effects = { {EffectType::ENERGY_MP, 2,0,true},{EffectType::DRAW,2,0,true } };
        break;

    case 3:
        d.name = "必殺技3";
        d.cardFilename = "Resource/CardEffect/special_3.png";
        d.desciption = "与えるダメージが\n10増える\n敵の攻撃力を10減少\n2ターン継続";
        d.effects = { {EffectType::APPLY_WEAKEN, 10,0, true},{EffectType::APPLY_ATTACK_DOWN,10,2,true} };
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
    d.backImageName = "Resource/Card/enemy_card_warning.png";

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
    default:
        d.enemyName = "敵のカード";
        d.frontImageName = "Resource/Card/enemy_card_warning.png";
        d.desciption = "";
        break;
    }
    return d;
}


// 枠のサイズを返すやつ
void Card::GetFrameSize(int* w, int* h)
{
    if (frameHandle != -1)
    {
        GetGraphSize(frameHandle, w, h);
    }
    else
    {
        // ないなら0にしてる
        *w = 0; *h = 0;
    }
}

void Card::ResetFrameByCost()
{
    // 古い枠を一旦消す
    if (frameHandle != -1)
    {
        DeleteGraph(frameHandle);
        frameHandle = -1;
    }

    // 今のコストを見てパスを決定
    std::string framePath = "";
    switch (GetCostType()) // GetCostTypeは今のdata.costを判定してくれる
    {
    case CostType::COST_0: framePath = "Resource/Card/player_card_cost0.png"; break;
    case CostType::COST_1: framePath = "Resource/Card/player_card_cost1.png"; break;
    case CostType::COST_2: framePath = "Resource/Card/player_card_cost2.png"; break;
    default:               framePath = "Resource/Card/player_card_cost3.png"; break;
    }

    // 新しい画像をロード
    frameHandle = LoadGraph(framePath.c_str());
}