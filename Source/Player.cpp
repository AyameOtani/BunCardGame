#include "Player.h"
#include <math.h>
#include "Mouse.h"
#include "EffectManager.h"
#include "Utility.h"
#include <algorithm>
#include <random>
#include "InputManager.h"
#include "Master.h"

Player::Player(VECTOR initPos, std::string filename, SceneManager::GScene gscene)
    : Unit(filename, initPos)
    , mGScene(gscene)
{
    // ステータス一旦ここにしてる
    hp = 100;
    maxHp = 100;
    maxMp = 3;
    mp = maxMp;
    specialCharge = 0;    
    specialChargeMax = 100;
	displayHp = maxHp;
	displayDamageHp = maxHp;


    SetFontSize(25);

    // 画像の大きさ大きくする
    SetScale(1.7f);

    // タグ
    SetTag(Object2D::Player2D);

    animator.ownerType = AnimationData::AnimOwner::PLAYER; // プレイヤーの判別追加


    // アイテム画像のロード
    itemImageHandles[0] = LoadGraph("Resource/Item/img_scissors.png"); // ハサミ
    itemImageHandles[1] = LoadGraph("Resource/Item/img_glue.png"); // のり
    itemImageHandles[2] = LoadGraph("Resource/Item/img_pen.png"); // ペン

    // 使用後
    itemUsedHandles[0] = LoadGraph("Resource/Item/img_use_scissors.png");
    itemUsedHandles[1] = LoadGraph("Resource/Item/img_use_glue.png");
    itemUsedHandles[2] = LoadGraph("Resource/Item/img_use_pen.png");

    // アイテムの位置設定
	float GraphX = 1800.0f; // 画面右端からの距離
    itemPositions[0] = VGet(GraphX, 100, 0); // ハサミ
    itemPositions[1] = VGet(GraphX, 280, 0); // のり
    itemPositions[2] = VGet(GraphX, 460, 0); // ペン

	mnPoisonHandle = LoadGraph("Resource/Icon/icon_poison.png"); // 毒のエフェクト画像のロード
	mnShieldHandle = LoadGraph("Resource/Icon/icon_shield.png"); // シールドのエフェクト画像のロード


    mnCardOperateHandle = LoadGraph("Resource/2D/card_operate.png"); // カードの選択説明の画像
}

Player::~Player()
{
    // 全てのリストを掃除
    for (auto c : hand)        delete c;
    for (auto c : deck)        delete c;
    for (auto c : discardPile) delete c;
    for (auto c : exhaustPile) delete c;
    for (auto c : pickOption) delete c;

    hand.clear();
    deck.clear();
    discardPile.clear();
    exhaustPile.clear();

    DeleteGraph(mnCardOperateHandle);

    // アイテム画像の削除
    for (int i = 0; i < 3; i++)
    {
        DeleteGraph(itemImageHandles[i]);
        DeleteGraph(itemUsedHandles[i]);
    }

    for (auto c : specialOptions) delete c;
    specialOptions.clear();

    for (auto& p : parts)
    {
        DeleteGraph(p.graphHandle);
    }
    parts.clear();

}

void Player::PlayMotion(AnimationData::AnimType type)
{
    const AnimationData* anim = Master::mpAnimationManager->GetAnim(type);
    animator.Play(anim);
}

// 初期化
void Player::Initialize()
{
    parts.clear();
    parts.resize(PartID::PART_MAX); //  サイズをMAXにしている

    // 名前で読み込んでる
    parts[PartID::LEG_L].graphHandle = LoadGraph("Resource/PlayerAnimation/anim_leg_l.png");
    parts[PartID::LEG_R].graphHandle = LoadGraph("Resource/PlayerAnimation/anim_leg_r.png");
    parts[PartID::WEAPON].graphHandle = LoadGraph("Resource/PlayerAnimation/anim_weapon.png");
    parts[PartID::HEAD].graphHandle = LoadGraph("Resource/PlayerAnimation/anim_head.png");
    parts[PartID::BODY].graphHandle = LoadGraph("Resource/PlayerAnimation/anim_body.png");
    parts[PartID::ARM_R].graphHandle = LoadGraph("Resource/PlayerAnimation/anim_arm.png");

    // アニメーターの初期設定　待機にしている
    animator.Play(Master::mpAnimationManager->GetAnim(AnimationData::AnimType::IDLE));
}


// カードが移動中かを返す関数
bool Player::IsCardMove()
{
    for (auto* c : hand)
    {
        if (c->GetDirection())
        return true;  // 一人でも動いていたら trueにしてる
    }

    return false;
}


// カードの描画
void Player::DrawCard(int num)
{
    for (int i = 0; i < num; i++)
    {
        // 手札上限チェック
        if (hand.size() >= HAND_CARD_MAX) break;

        // 山札が空なら ゴミ箱を混ぜて戻す
        if (deck.empty())
        {
            DiscardIntoDeck();
        }

        // ゴミ箱を戻してもまだ空なら  全カードが手札か除外にあるもう引けない
        if (deck.empty()) break;


        // 山札の最後から1枚取り出して手札に移動
        Card* pCard = deck.back();
        deck.pop_back();
        hand.push_back(pCard);

        // 現在位置を左下にワープ
        pCard->SetNowPosition(200.0f, Utility::SCREEN_HEIGHT - 100.0f, 0.0f);
        // 初期スケール
        pCard->SetScale(0.05f);
        // 手札に来たときの目標スケール
        pCard->SetTargetScale(0.93f);
    }

    // 位置再セット
    RefreshCardPositions();

    // Refreshした後の座標を即座に反映させるために全員Update
    for (auto* c : hand)
    {
        c->SetFocus(false, false); // ちらつき防止
        //c->Update();
    }
}


void Player::Update(Enemy& enemy)
{
    //HpGaugeUpdate(); // ゲームシーンに移動してすぐに更新されるようにした　大谷

    if (!isPicking) //初めの選択フェーズまたは必殺中じゃないなら
    {
        if (!isSpecialPicking)
        {
            int currentHover = -1; // 保存用 ホバーしているかのやつ
            // アイテムアイコンのクリック判定
            for (int i = 0; i < 3; i++)
            {
                int imgW, imgH;
                GetGraphSize(itemImageHandles[i], &imgW, &imgH);
                int scaledW = (int)(imgW * ITEM_SCALE);
                int scaledH = (int)(imgH * ITEM_SCALE);
                int left = (int)itemPositions[i].x - scaledW / 2;
                int right = (int)itemPositions[i].x + scaledW / 2;
                int top = (int)itemPositions[i].y - scaledH / 2;
                int bottom = (int)itemPositions[i].y + scaledH / 2;

            //    if (!usedItems[i] && Mouse::x >= left && Mouse::x <= right &&
            //        Mouse::y >= top && Mouse::y <= bottom)
            //    {
            //        currentHover = i; // 数値を代入して今選んでいるのがわかる
            //        mfItemSizes[i] = 0.3f;
            //        if (Mouse::IsTrigger())
            //        {
            //            Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
            //            // クリックされたアイテムの効果を発動
            //            Effect e;
            //            if (i == 0) // ハサミ
            //            {
            //                e = { EffectType::EXTRA_PLAY, 0, 0 };
				        //}
            //            else if (i == 1) // のり
            //            {
            //                e = { EffectType::KEEP_CARD, 0, 0 };
            //            }
            //            else if (i == 2) // ペン
            //            {
            //                e = { EffectType::PEN_RANDOM, 0, 0 };
            //            }
            //            EffectManager::ApplyEffect(e, *this, enemy);
            //            usedItems[i] = true; // 使用済みに設定

            //            // アイテム使用回数を保存するやつ
            //            Master::mpSaveItemCount += 1;

            //        }
            //    }
            //    else
            //    {
            //        mfItemSizes[i] = 0.25f;
            //    }
            //    mnHoverItem = currentHover; // 代入している


                // マウスが乗っているか
                bool isHover =
                    Mouse::x >= left && Mouse::x <= right &&
                    Mouse::y >= top && Mouse::y <= bottom;

                if (isHover)
                {
                    currentHover = i;
                    mfItemSizes[i] = 0.3f;

                    // 未使用ならクリック可能
                    if (!usedItems[i] && Mouse::IsTrigger())
                    {
                        Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

                        Effect e;

                        if (i == 0) // ハサミ
                        {
                            e = { EffectType::EXTRA_PLAY, 0, 0 };
                        }
                        else if (i == 1) // のり
                        {
                            e = { EffectType::KEEP_CARD, 0, 0 };
                        }
                        else if (i == 2) // ペン
                        {
                            e = { EffectType::PEN_RANDOM, 0, 0 };
                        }

                        EffectManager::ApplyEffect(e, *this, enemy);

                        usedItems[i] = true;

                        // アイテム使用回数を保存
                        Master::mpSaveItemCount += 1;
                    }
                }
                else
                {
                    mfItemSizes[i] = 0.25f;
                }

                mnHoverItem = currentHover;
            }
        }
    }

    Object2D::Update();
}


// カード使うやつ
void Player::CardUpdate(Enemy& enemy)
{

    // まずマウスが乗っているカード(index)を先に計算しておく 重なりを考えて逆順にした
    int hoverIndex = -1;
    for (int i = (int)hand.size() - 1; i >= 0; i--)
    {
        if (hand[i]->CheckHover(Mouse::x, Mouse::y))
        {
            hoverIndex = i;
            break;
        }
    }

    //必殺のカード選択中がったら
    if (isSpecialPicking)
    {
        for (int i = 0; i < specialOptions.size(); i++)
        {
            //ホバーさせるかのやつ
            specialOptions[i]->SetFocus(specialOptions[i]->CheckHover(Mouse::x, Mouse::y), false);
            specialOptions[i]->LateUpdate();

            // ホバーしているかつクリックされたら発動させる
            if (specialOptions[i]->CheckHover(Mouse::x, Mouse::y) && Mouse::IsTrigger())
            {
                // 必殺カードごとのSE
                int cardId = specialOptions[i]->GetData().id;

                if (cardId == 1)
                {
                    Master::mpSoundManager->PlaySE(SoundManager::SE_OUGI);
                }
                else if (cardId == 2)
                {
                    Master::mpSoundManager->PlaySE(SoundManager::SE_SEIREI);
                }
                else if (cardId == 3)
                {
                    Master::mpSoundManager->PlaySE(SoundManager::SE_SAAIKUZO);
                }

                // 効果発動
                const auto& effects = specialOptions[i]->GetData().effects;
                for (const auto& e : effects)
                {
                    EffectManager::ApplyEffect(e, *this, enemy);
                }


                // ゲージをリセット
                specialCharge = 0;
                specialGaugeHeight = 0;

                // 選択終了
                isSpecialPicking = false;
                for (auto c : specialOptions) delete c;
                specialOptions.clear();
            }
        }
    }
    else
    {
        // のり選択の判定（クリックではなくリリースで判定する）
        if (keepNextCardCount > 0)
        {
            // マウスのリリースを検出 最後にクリックされて、今はクリックされていないなら
            bool isRelease = (Mouse::lastInput & MOUSE_INPUT_LEFT) && !(Mouse::input & MOUSE_INPUT_LEFT);

            if (isRelease && hoverIndex != -1)
            {
                Master::mpSoundManager->PlaySE(SoundManager::SE_PETA);
                int i = hoverIndex;
                // 既に自分がキープ中だった場合は解除するだけ
                if (hand[i]->isKeep)
                {
                    hand[i]->isKeep = false;
                }
                else
                {
                    // 他のカードのキープをすべて一旦解除
                    for (auto* p : hand)
                    {
                        p->isKeep = false;
                    }
                    // このカードだけをキープ
                    hand[i]->isKeep = true;
                }
            }
        }

        // 選択モードの時 (ゲーム開始時に選ぶやつね)
        if (isPicking)
        {
            for (int i = 0; i < pickOption.size(); i++)
            {
                // マウスが乗っているか判定
                bool isHover = pickOption[i]->CheckHover(Mouse::x, Mouse::y);
                // カードにホバー状態を伝える (ドラッグはしていないので false)
                pickOption[i]->SetFocus(isHover, false);
                pickOption[i]->Update();

                // クリック判定 
                if (pickOption[i]->CheckHover(Mouse::x, Mouse::y) && Mouse::IsTrigger())
                {
                    Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);

                    // 選んだカードをデッキへ   (4枚の固定カード + 3枚)
                    deck.push_back(new Card(pickOption[i]->GetData()));

                    // 合計7枚まで繰り返す
                    if (deck.size() < HAND_CARD_MAX)
                    {
                        GeneratePickOntions(); // 次のペアを出す
                    }
                    else
                    {
                        // 残り13枚を自動補充する処理
                        while (deck.size() < DECK_MAX)
                        {
                            int randId = GetRand(Card::CARD_COUNT - 1) + 1;
                            Card::CardData data = Card::GetCardDataById(randId);

                            // 枚数制限ルールチェック
                            int count = 0;
                            // 今のデッキに選ぼうとしているカードが何枚あるか数える
                            for (auto* c : deck) if (c->GetData().id == randId) count++;

                            if (data.isExhaust && count >= 1) continue; // 一回きりは1枚まで
                            if (!data.isExhaust && count >= 2) continue; // その他は2枚まで

                            deck.push_back(new Card(data));
                        }

                        // 全て終わったので選択終了
                        isPicking = false;
                        for (auto c : pickOption) delete c;
                        pickOption.clear();

                        // 20枚をシャッフルして5枚引く
                        DiscardIntoDeck();
                        DrawCard(FIRST_HAND_CARD); // 20枚から7枚引くので、山札は「13枚」残る

                        RefreshCardPositions(); // 位置決める

                        // 引いたばかりのカードすべてに Update をかけて
                        // 座標 (x, y) やスケール (scale) を 1フレーム目から確定させる
                        for (auto* c : hand)
                        {
                            c->SetFocus(false, false); // 注目させているかをOFF
                        }

                    }
                    //return; // 1クリックで1枚選ぶのでここで抜ける  いらんかも
                }
            }
        }


        else // 選択モードじゃないとき
        {
            // hoverIndex は先に計算済み

            // ドラッグ判定
            if (Mouse::IsPress())
            {
                if (draggingIndex == -1 && hoverIndex != -1)
                {
                    // ここでMP判定
                    // ドラック中のコストとMPを比べる
                    if (mp >= hand[hoverIndex]->GetCost())
                    {
                        draggingIndex = hoverIndex;
                        IsMpError = false;
                    }
                    else
                    {
                        IsMpError = true;
                    }
                }
            }
            else // 指を離したとき
            {
                IsMpError = false;
                if (draggingIndex != -1)
                //if (draggingIndex >= 0 && draggingIndex < (int)hand.size())
                {
                    Card* usedCard = hand[draggingIndex];

                    // 使う条件を満たしているか (位置OK ＋ MPもOK)
                    if (Mouse::y < DraggY && mp >= usedCard->GetCost())
                    {
                        // 消去と効果発動の処理
                        mp -= usedCard->GetCost();
                        hand.erase(hand.begin() + draggingIndex);

                        // 効果発動
                        // ハサミ対応Ver フラグ立っていたら２回効果を与える
                        const auto& effects = usedCard->GetData().effects;

                        // ハサミフラグが立っていたら2回、そうでなければ1回
                        int loopCount = doubleNextCard ? 2 : 1;

                        for (int i = 0; i < loopCount; i++)
                        {
                            for (const auto& e : effects)
                            {
                                EffectManager::ApplyEffect(e, *this, enemy);
                            }
                        }




                        // 一回きりかで振り分ける
                        // 使用後は KEEP フラグをリセットしてから移動する
                        usedCard->isKeep = false;

                        // コストを元に戻す
                        int originalCost = Card::GetCardDataById(usedCard->GetData().id).cost;
                        usedCard->SetCost(originalCost);
                        usedCard->ResetFrameByCost();

                        if (usedCard->GetData().isExhaust)
                        {
                            exhaustPile.push_back(usedCard); // trueなら場外のリストへ
                        }
                        else
                        {
                            discardPile.push_back(usedCard); // ゴミ箱へ行く
                        }

                        // ★ discardPile に入るカードは必ず初期位置へ戻す
                        usedCard->SetCardPosition(200.0f, Utility::SCREEN_HEIGHT - 100.0f, 0.0f);
                        usedCard->SetTargetScale(0.93f);
                        usedCard->Update();

                        // 無効にしとく
                        hoverIndex = -1;
                    }
                    else
                    {
                        // もしMP不足、または位置が下の方なら「何もしない」
                        // ここで nothing! カードは hand に残ったまま。
                    }

                    // 最後に必ずドラッグを解除
                    draggingIndex = -1;
                    IsMpError = false;
					Master::mpSaveCardCount += 1; // カード使用回数を保存するやつ
                }
            }


            // 各カードの状態更新
            for (int i = 0; i < hand.size(); i++)
            //for (int i = 0; i < (int)hand.size(); i++)
            {
                // 今ループしているカードはさっきのカードと同じかのチェック
                hand[i]->SetFocus((i == hoverIndex), (i == draggingIndex));
                hand[i]->Update();
            }

            // 再配置　カードの枚数変わった時のため
            RefreshCardPositions();
        }
    }

  

}


// 枚数変わったら詰めるように
void Player::RefreshCardPositions()
{
    float centerX = Utility::SCREEN_WIDTH / 2.0f;  // 円の中心 画面の真ん中
    float centerY = Utility::SCREEN_HEIGHT + 80.0f; // 円の中心 画面の下の方
    float radiusX = 450.0f;  // 横の半径
    float radiusY = 200.0f;  // 縦の半径

    // 枚数入れている
    int currentNum = (int)hand.size();
    // カードとカードの間の角度　14にしている
    float stepAngle = (14.0f * (DX_PI_F / 180.0f));
    // カード全体の幅の半分の角度
    float totalHalfAngle = (stepAngle * (currentNum - 1)) / 2.0f;
    // 一番左のカードの開始角度
    float startAngle = (-90.0f * (DX_PI_F / 180.0f)) - totalHalfAngle;

    for (int i = 0; i < currentNum; i++)
    {
        float angle = startAngle + (stepAngle * i);
        float tx = centerX + cosf(angle) * radiusX; // 角度から横の位置を出す
        float ty = centerY + sinf(angle) * radiusY; // 縦の位置を出す

        // 円の方向を向くようにして扇にしている
        float trot = angle + (DX_PI_F / 2.0f);

        // 位置と回転を設定
        hand[i]->SetCardPosition(tx, ty, trot);
    }
}


// 手札を描画する
void Player::DrawHand(int workScreen, int fontHandle)
{
    if (isSpecialPicking) //必殺の時は必殺のカードを描画する
    {
        for (auto* c : specialOptions)
        {
            c->Draw(workScreen, fontHandle, true, true);
        }
    }
    else
    {
        if (isPicking)
        {
            // 選択肢のカードを描画
            for (auto* c : pickOption)
            {
                // 第二引数 true でホバー時に大きくなるように
                c->Draw(workScreen, fontHandle, true, true);
            }

            // カードを選択しての説明画像
            DrawRotaGraph(Utility::SCREEN_WIDTH / 2, 250, 0.1f, 0.0f, mnCardOperateHandle, TRUE);
        }
        else
        {

            // 誰かがフォーカスされているかチェック
            int focus = draggingIndex;

            if (focus == -1)
            {
                // 重なりの上にあるやつを後ろから判定する
                for (int i = (int)hand.size() - 1; i >= 0; i--)
                {
                    if (hand[i]->CheckHover(Mouse::x, Mouse::y))
                    {
                        focus = i;
                        break;
                    }
                }
            }

            bool isFocus = (focus != -1);

            // 0コストカードを持っているか
            bool hasZeroCost = false;
            for (auto* c : hand)
            {
                if (c->GetCost() == 0)
                {
                    hasZeroCost = true;
                    break;
                }
            }

            // MP0 かつ 0コスト無しなら暗くする
            bool allDark = (mp <= 0 && !hasZeroCost);

            // 主役じゃないカードを先に描画
     //       for (int i = 0; i < hand.size(); i++)
     //       {
     //           if (i != focus)
     //           {
     //               if (allDark)
     //               {
					//	SetDrawBright(150, 150, 150);
     //               }
					//hand[i]->Draw(workScreen, fontHandle, isFocus);
					//SetDrawBright(255, 255, 255); // 色戻す
     //           }

     //       }
            for (int i = 0; i < hand.size(); i++)
            //for (int i = 0; i < (int)hand.size(); i++)
            {
                if (i != focus)
                {
                    // このカードが使えないなら暗くする
                    if (mp < hand[i]->GetCost())
                    {
                        SetDrawBright(150, 150, 150);
                    }

                    hand[i]->Draw(workScreen, fontHandle, isFocus);

                    SetDrawBright(255, 255, 255);
                }
            }

            // 注目カード 最後に描画
            if (isFocus)
            {
                bool isTransparent = (focus == draggingIndex && Mouse::y < DraggY);

                if (isTransparent)
                {
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
                }

                // 追加した
                /*if (allDark)
                {
                    SetDrawBright(150, 150, 150);
                }*/

                if (mp < hand[focus]->GetCost())
                {
                    SetDrawBright(150, 150, 150);
                }

                hand[focus]->Draw(workScreen, fontHandle, isFocus);

                SetDrawBright(255, 255, 255);

                if (isTransparent)
                {
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                }
            }
        }
    }
  

}

// HPバーの描画
void Player::HpGaugeDraw()
{
    // 表示を開始する基準位置6
    int x = (int)mvPosition.x - 50;
    int y = (int)mvPosition.y + 110;
    int drawY = y+4; // 基準位置


    // Hpゲージの作成 //
    int hpX = (int)mvPosition.x - 160;
    int di = 5;
    // Hpゲージの黒背景
    DrawRoundRect(hpX, y, hpX + width, y + height, 10, 10, GetColor(80, 80, 80), TRUE);

    // 描画用幅
    int drawGaugeWidth = gaugeWidth;

    // 最低幅を保証
    int minWidth = (int)((float)mnHpLow / maxHp * width);

    if (drawGaugeWidth < minWidth && hp > 0)
    {
        drawGaugeWidth = minWidth;
    }

    // ダメージバー
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

    // 緑ゲージ
    DrawRoundRect(
        hpX,
        y,
        hpX + drawGaugeWidth,
        y + height,
        10,
        10,
        GetColor(103, 203, 131),
        true
    );

    // Hpゲージの枠線
    DrawRoundRect(hpX, y, hpX + width, y + height, 10, 10, GetColor(255, 255, 255), FALSE);
    // ------------ //

    DrawFormatStringToHandle(hpX + 70, drawY, GetColor(255, 255, 255),
        Master::mpFontManager->GetHpFontHandle(), "HP  %d / %d", hp, maxHp);

}

// 必殺技ゲージの描画
void Player::SpecialGaugeDraw()
{
    // 必殺技ゲージの作成 //
    // 表示を開始する基準位置
    int spX = 90;
    int y = (int)mvPosition.y + 20;

    // 必殺技ゲージの枠
    DrawBox(spX, y, spX + specialWidth, y + specialHeight, GetColor(80, 80, 80), true);


    // 必殺技ゲージ（黄）
    DrawBox(spX, y, spX + specialWidth, y + specialGaugeHeight, GetColor(255, 207, 22), true);

    // 必殺技ゲージの枠線
    DrawBox(spX, y, spX + specialWidth, y + specialHeight, GetColor(255, 255, 255), false);
    // ------------ //

    // =========================
    // SP文字
    // =========================

    int centerX = spX + 35;

    int spWidth = GetDrawStringWidth("SP", 2);
    int spDrawX = centerX - spWidth / 2;

    DrawStringToHandle(
        spDrawX,
        y + specialHeight + 30,
        "SP",
        GetColor(255, 255, 255),
        Master::mpFontManager->GetItemFontHandle()
    );


    // =========================
    // 数字
    // =========================

    char str[32];
    sprintf_s(str, "%d", specialCharge);

    int numWidth = GetDrawStringWidthToHandle(str, (int)strlen(str), Master::mpFontManager->GetItemFontHandle());
    int numDrawX = centerX - numWidth / 2;

    DrawStringToHandle(
        numDrawX,
        y + specialHeight + 80,
        str,
        GetColor(255, 255, 255),
        Master::mpFontManager->GetItemFontHandle()
    );
}

void Player::IconDraw()
{
    // 表示を開始する基準位置
    int x = (int)mvPosition.x - 80;
    int y = (int)mvPosition.y + 110;

    int drawY = y; // 基準位置

    //アイコンの描画
    int iconX = x - 50;
    int iconY = 720;
    int yDist = 18;

    //シールドのアイコン描画
    DrawGraph(iconX - 45, iconY, mnShieldHandle, TRUE);

    // 表示する文字幅を取得
    int textWidth = GetDrawFormatStringWidth("%d", block);
    // 中央になるようにXを調整
    int textX = (iconX - 3) - textWidth / 2;
    // 描画
    DrawFormatStringToHandle(textX, drawY + 38, GetColor(230, 230, 230),
        Master::mpFontManager->GetStatusFontHandle(), "%d", block);


    // バフ・デバフ（ここから下に伸びる）
    int effectX = x;
    // バフ
    for (const auto& b : buffs)
    {
        // DrawFormatString(effectX, drawY, GetColor(255, 200, 0), "[昇:%d|%dT]", b.value, b.turns);
        drawY += 20; //  縦に並べるならY増やす
    }
    // デバフ
    for (const auto& d : debuffs)
    {
        if (d.type == DebuffType::DEBUFF_POISON)
        {
            //状態異常のアイコン描画
            DrawGraph(iconX + 5, iconY, mnPoisonHandle, TRUE);
        }
        else if (d.type == DebuffType::WEAKEN)
        {
            //DrawFormatString(effectX, drawY, GetColor(255, 100, 100), "[弱:%d|%dT]", d.value, d.turns);
        }
        else if (d.type == DebuffType::ATTACK_DOWN)
        {
            //DrawFormatString(effectX, drawY, GetColor(200, 0, 255), "[減:%d|%dT]", d.value, d.turns);
        }

        drawY += 20;
    }




}

void Player::Draw()
{
    Object2D::Draw();


    // アニメーションの描画
    for (auto& p : parts)
    {
        DrawPart(p, mvPosition.x, mvPosition.y, 0.4f);
    }

  
 
    //// デバッグ表示
    //if (IsMpError)
    //{
    //    DrawFormatString(x, drawY, GetColor(255, 0, 0), "MP不足！");
    //}

    // デバック用線の位置
    //DrawLine(0, (int)DraggY, Utility::SCREEN_WIDTH, (int)DraggY, GetColor(255, 0, 0), TRUE);

    // アイテムアイコンの描画
    for (int i = 0; i < 3; i++)
    {
        int imgW, imgH;
        //GetGraphSize(itemImageHandles[i], &imgW, &imgH);

        int handle = usedItems[i] ? itemUsedHandles[i] : itemImageHandles[i];
        GetGraphSize(handle, &imgW, &imgH);

      /*  DrawExtendGraph(
            (int)itemPositions[i].x - (int)(imgW * ITEM_SCALE / 2),
            (int)itemPositions[i].y - (int)(imgH * ITEM_SCALE / 2),
            (int)itemPositions[i].x + (int)(imgW * ITEM_SCALE / 2),
            (int)itemPositions[i].y + (int)(imgH * ITEM_SCALE / 2),
            itemImageHandles[i], TRUE);*/

        //int handle = usedItems[i] ? itemUsedHandles[i] : itemImageHandles[i];

        DrawRotaGraph(
            (int)itemPositions[i].x,
            (int)itemPositions[i].y,
            mfItemSizes[i],
            0.0f,
            handle,
            TRUE
        );
    }

    // アイテム説明の表示
    if (mnHoverItem != -1)
    {
        const char* itemMemo[] =
        {
            "ハサミ\n\n"
            "このターン  カード2回を発動する",


            "のり\n\n"
            "指定したカードを手札に残す",


            "ペン    効果ランダム3種類\n\n"
          /*"1. 1ターン コストをランダムにする\n"
            "2. MPを全消費してHPを回復する\n"
            "3. 2ターン敵をスタンさせる"*/
        };

        // 表示位置
        int drawX = Utility::SCREEN_WIDTH - 650;
        int drawY = 80 + (mnHoverItem * 110);
        int FrameDist = 16; // 枠の差分
        int BoardDist = 10; // ボードの差分

        // 背景の枠
        DrawBox(drawX - FrameDist, drawY - FrameDist, 1670+ FrameDist,
            (drawY + FrameDist) + 150, GetColor(207, 107, 22), TRUE);

        // 中のBOX
        DrawBox(drawX - BoardDist, drawY - BoardDist, 1670+BoardDist,
            (drawY + BoardDist) + 150,GetColor(23,92, 11), TRUE);

        // 文字の描画
        //DrawString(drawX, drawY, itemMemo[mnHoverItem], GetColor(225, 225, 225));

        DrawFormatStringToHandle(drawX, drawY, GetColor(255, 255, 255),
            Master::mpFontManager->GetItemFontHandle(), itemMemo[mnHoverItem]);

        if (mnHoverItem == 2) // はさみだったら
        {
            // 説明
            int textY = drawY + 45;

            DrawFormatStringToHandle(
                drawX,
                textY,
                GetColor(255, 255, 255),
                Master::mpFontManager->GetItemFontHandle(),
                "1. 1ターン コストをランダムにする"
            );

            DrawFormatStringToHandle(
                drawX,
                textY + 32,
                GetColor(255, 255, 255),
                Master::mpFontManager->GetItemFontHandle(),
                "2. MPを全消費してHPを回復する"
            );

            DrawFormatStringToHandle(
                drawX,
                textY + 64,
                GetColor(255, 255, 255),
                Master::mpFontManager->GetItemFontHandle(),
                "3. 2ターン敵をスタンさせる"
            );

        }

    }
}


// WinMainで呼ぶ バトル開始 の手札引くやつ
void Player::StartBattle()
{
    // 一旦デッキを空にする
    for (auto c : deck) delete c;
    deck.clear();


    //絶対に入れたいカードを2枚ずつ入れる
    //  ID１　5ダメ
    //  ID２　5ブロ
    int mustIds[] = { 1, 2 };
    for (int id : mustIds)
    {
        for (int i = 0; i < 2; i++) // 2枚ずつ
        {
            //deck.push_back(new Card(Card::GetCardDataById(id)));

            Card* c = new Card(Card::GetCardDataById(id));

            // ★ 現在位置を左下にワープ
            c->SetNowPosition(200.0f, Utility::SCREEN_HEIGHT - 100.0f, 0.0f);
            // ★ 初期スケール
            c->SetScale(0.05f);
            deck.push_back(c);

        }
    }

    // 選択モード開始
    isPicking = true;
    GeneratePickOntions();
}


// 手札をすてて山札に戻す
void Player::ReshuffleTurnEnd()
{
    std::vector<Card*> nextHand;

    int keepUsed = 0;

    for (auto* pCard : hand)
    {
        // KEEP対象
        if (pCard->isKeep &&
            keepUsed < keepNextCardCount)
        {
            nextHand.push_back(pCard);

            //  KEEPはここで消費
            keepUsed++;

            // 次ターンにはKEEP表示を消す
            pCard->isKeep = false;

            continue;
        }

        // --- 通常処理 ---
        pCard->isKeep = false;

        int originalCost = Card::GetCardDataById(pCard->GetData().id).cost;
        pCard->SetCost(originalCost);
        pCard->ResetFrameByCost();

        if (pCard->GetData().isExhaust)
        {
            exhaustPile.push_back(pCard);
        }
        else
        {
            discardPile.push_back(pCard);
        }

        pCard->SetNowPosition(200.0f, Utility::SCREEN_HEIGHT - 100.0f, 0.0f);
        pCard->SetScale(0.05f);
    }

    // 手札更新
    hand = nextHand;


    // リセット
    keepNextCardCount = 0;
    doubleNextCard = false;
    randomizeCostThisTurn = false;
}



// ゴミ箱に移動
void Player::DiscardIntoDeck()
{
    // ゴミ箱のカードを山札に移動
    for (auto* pCard : discardPile)
    {
        // ★位置もスケールも初期化
        pCard->SetNowPosition(200.0f, Utility::SCREEN_HEIGHT - 100.0f, 0.0f);
        deck.push_back(pCard); // 捨て札のカードを1枚ずつ山札の末尾に入れる
    }
    discardPile.clear(); // 捨て札リストを空にする

    // シャッフル
    std::random_device rd;   // 種を作るための装置
    std::mt19937 g(rd());    // メルセンヌ・ツイスタという高性能な乱数生成器らしい
    std::shuffle(deck.begin(), deck.end(), g); // 山札をバラバラに混ぜる
}


void Player::GeneratePickOntions()
{
    // 新しいカード用のリストを一時的に作る
    std::vector<Card*> nextOptions;

    while (nextOptions.size() < 2)
    {
        int randId = GetRand(Card::CARD_COUNT - 1) + 1;
        Card::CardData data = Card::GetCardDataById(randId);

        // カードがダブったりしないように                                         
        // 1回きりのカードは1枚にしたい
        int count = 0;
        for (auto* c : deck) if (c->GetData().id == randId) count++;
        for (auto* c : nextOptions) if (c->GetData().id == randId) count++;

        if (data.isExhaust && count >= 1) continue;
        if (!data.isExhaust && count >= 2) continue;

        // 選択の時のカードの位置
        Card* pCard = new Card(data);
        float offsetX = (nextOptions.size() == 0) ? -250.0f : 250.0f;
        pCard->SetCardPosition(Utility::SCREEN_WIDTH / 2.0f + offsetX, Utility::SCREEN_HEIGHT / 2.0f, 0.0f);

        // 座標を確定させておく
        pCard->Update();
        nextOptions.push_back(pCard);
    }

    // 1枚も作れなかった場合
    if (nextOptions.empty())
    {
        Card* fallback = new Card(Card::GetCardDataById(1));
        fallback->SetCardPosition(
            Utility::SCREEN_WIDTH / 2.0f,
            Utility::SCREEN_HEIGHT / 2.0f,
            0.0f
        );
        fallback->Update();
        nextOptions.push_back(fallback);
    }

    // 新しいカードが揃ってから古いカードを消す
    for (auto* c : pickOption)
    {
        delete c;
    }
    pickOption.clear();

    // 新しいリストを pickOption にコピーする
    pickOption = nextOptions;

    // 菊池
    // ひとつ選んでくださいのSE
    // これも一回だけ鳴らす
    if (!isPickVoicePlayed)
    {
        Master::mpSoundManager->PlaySE(SoundManager::SE_HITOTU);
        Master::mpSoundManager->PlaySEDelay(SoundManager::SE_SELECT2,50);

        isPickVoicePlayed = true;
    }
}

void Player::HpGaugeUpdate()
{
	// ダメージを受けてからHPバーが減るまでのタイマーを進める
    BarTimer++;

	// タイマーが一定以上になったら、表示上のHPを実際のHPに近づける
    if (BarTimer >= BAR_FRAME)
    {
		// 表示上のHPが実際のHPより多い場合、少しずつ減らす
        if (displayDamageHp > hp)
        {
            displayDamageHp -= (int)BarDamageSpeed;

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
        if (displayHp > hp)
        {
            displayHp = hp;
        }
        if (displayHp < hp)
        {
			displayHp++;
        }

		BarTimer = 0; // タイマーをリセットして次の減少に備える
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

void Player::SpecialGaugeUpdate()
{
    if (specialCharge <= specialChargeMax)
    {
        specialCharge += 15; // 必殺技ゲージの増加量
        if (specialCharge > specialChargeMax)
        {
            specialCharge = specialChargeMax; // 上限を超えないようにする
        }
    }

    // 必殺技ゲージの幅を更新
    specialGaugeHeight = (int)((double)specialCharge / specialChargeMax * specialHeight);
}

void Player::StartSpecialPick()
{
    isSpecialPicking = true;

    for (auto c : specialOptions) delete c;
    specialOptions.clear();

    // 必殺技ID 1, 2, 3 を生成して横に並べる
    for (int i = 1; i <= 3; i++)
    {
        Card* pCard = new Card(Card::SpecialCardDataById(i));

        // 画面中央に横並びで配置
        float offsetX = (i - 2) * 350.0f; // -350, 0, 350 の位置
        pCard->SetCardPosition(Utility::SCREEN_WIDTH / 2.0f + offsetX, Utility::SCREEN_HEIGHT / 2.0f, 0.0f);
        pCard->SetTargetScale(1.0f); // 選択肢なので少し大きめ
        pCard->Update();

        specialOptions.push_back(pCard);
    }
}