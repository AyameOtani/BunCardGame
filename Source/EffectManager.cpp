#include "EffectManager.h"
#include "Player.h"
#include "Enemy.h"
#include <cstdio>
#include "Master.h"
#include "Utility.h"
#include "GameScene.h"

// バフ要素を相手のリストに入れる
void EffectManager::ApplyBuffEffect(const Buff& buff, Unit& self, Unit& target)
{
    self.buffs.push_back(buff);
    std::printf("Buff Type %d, Value %d, Turn %d", (int)buff.type, buff.value, buff.turns);
}


// デバフ要素を相手のリストに入れる
void EffectManager::ApplyDebuffEffect(const Debuff& debuff, Unit& self, Unit& target)
{
    target.debuffs.push_back(debuff);
    std::printf("Debuff Type %d, Value %d, Turn %d", (int)debuff.type, debuff.value, debuff.turns);
}


void EffectManager::ApplyEffect(const Effect& effect, Unit& self, Unit& target)
{
    // プレイヤーとしての機能を使うとき用  selfをPlayerに変換
    Player* pPlayer = dynamic_cast<Player*>(&self);

    switch (effect.type)
    {
        case EffectType::DAMAGE:
        {
            // 菊池
            // ダメージのSE  プレイヤーと敵で変える
            if (pPlayer != nullptr)
            {
                Master::mpSoundManager->PlaySE(SoundManager::SE_KURAE);

                if (effect.DoubleAttack)
                {
                    // 2回攻撃
                    Master::mpSoundManager->PlaySEDelay(SoundManager::SE_DAMEIGI, 0);
                    Master::mpSoundManager->PlaySEDelay(SoundManager::SE_DAMEIGI, 30);
                }
                else
                {
                    // 通常攻撃
                    Master::mpSoundManager->PlaySEDelay(SoundManager::SE_DAMEIGI, 0);
                }
            }
            else
            {
                Master::mpSoundManager->PlaySE(SoundManager::SE_KUUU);
                // 敵の攻撃
                Master::mpSoundManager->PlaySEDelay(SoundManager::SE_DAMEIGIEN, 10);

            }

            // 多段ヒットのやつ
            int times = effect.count > 0 ? effect.count : 1;

            int base = effect.value;

            // 自分のバフを計算 (攻撃力アップとか)
            int bonus = 0;
            for (const auto& b : self.buffs)
            {
                if (b.type == BuffType::ATTACK)
                {
                    bonus += b.value; // バフを取り出している
                }
            }

            // 相手のデバフを計算   弱体化 受けるダメージ増加
            int weaken = 0;
            for (const auto& d : target.debuffs)
            {
                if (d.type == DebuffType::WEAKEN)
                    weaken += d.value;
            }

            // 自分のデバフを計算
            int attackDown = 0;
            for (const auto& a : self.debuffs)
            {
                if (a.type == DebuffType::ATTACK_DOWN)
                {
                    attackDown += a.value;
                }
            }

            // 最終的に与えるダメージ計算
            // ダメージ　＝　攻撃力 + バフ + 相手のデバフ - 自分のデバフ
            int totalDamage = base + bonus + weaken - attackDown;
            if (totalDamage < 0) totalDamage = 0; // 一応クランプ

            // 多段込みの最終ダメージ
            int finalDamage = totalDamage * times;


            if (finalDamage >= target.block)
            {
                int remainDamage = finalDamage - target.block;

                target.block = 0;
                target.hp -= remainDamage;

                // ダメージがブロックを貫通する場合は攻撃のSE
                if (pPlayer != nullptr)
                {
                    // プレイヤー攻撃
                    //Master::mpSoundManager->PlaySEDelay(SoundManager::SE_MOSUTAADAME,70);
                }
                else
                {
                    // 敵攻撃
                    Master::mpSoundManager->PlaySEDelay(SoundManager::SE_DAMEARI, 50);
                }
            }
            else
            {
                // ガード成功
               // ダメージがブロックを完全に防いでいる場合は防御のSEと効かないぞのSE
                if (pPlayer != nullptr)
                {
                    Master::mpSoundManager->PlaySE(SoundManager::SE_BOUGYO);

                    //Master::mpSoundManager->PlaySEDelay(SoundManager::SE_DAMEZERO, 50);
                }
                else
                {
                    Master::mpSoundManager->PlaySE(SoundManager::SE_BOUGYO);

                    Master::mpSoundManager->PlaySEDelay(SoundManager::SE_DAMEZERO, 50);
                }

                // ブロックがダメージを完全に防ぐ時はブロックの数を削る
                target.block -= finalDamage;
            }
                 
            if (target.hp < 0) target.hp = 0;

            std::printf("Damage : Base(%d) + Buff(%d) + Weaken(%d) = Total %d\n",
                base, bonus, weaken, totalDamage);



          
           

             // 必殺のカードだったらエフェクトを変える
             if (effect.isSpecial)
             {
                 // 必殺技のダメージエフェクトアニメーション表示
                 new Object2D(
                     target.GetPosition(),
                     "Resource/Effect/special_Damege_effect.png",
                     12,
                     6,
                     2,
                     3
                 );

                 //  攻撃アニメーションを再生
                 self.PlayMotion(AnimationData::AnimType::SPECIAL);
                 //ダメージを受けた側
                 target.PlayMotion(AnimationData::AnimType::DAMAGE);
             }
             else
             {
                 // ダメージエフェクトアニメーション表示
                 new Object2D(
                     target.GetPosition(),
                     "Resource/Effect/damege_effect.png",
                     5,
                     5,
                     1,
                     3
                 );

                 //  攻撃アニメーションを再生
                 self.PlayMotion(AnimationData::AnimType::ATTACK);
                 //ダメージを受けた側
                 target.PlayMotion(AnimationData::AnimType::DAMAGE); 
             }

           
            break;
        }


        case EffectType::BLOCK:

            // 菊池
            // ブロックアップのSE
            if (effect.BlockBuff)
            {
                Master::mpSoundManager->PlaySEDelay(SoundManager::SE_BOUGYO_UP, 75);
            }
            else
            {
                Master::mpSoundManager->PlaySE(SoundManager::SE_BOUGYO_UP);
            }

            self.block += effect.value; // 発動者にブロック付与


            // selfをUnitにキャスト
            if (self.pShieldEffect == nullptr)
            {
                self.pShieldEffect=
                    new Object2D(
                    VSub(self.GetPosition(), VGet(0.0f, 0.0f, 0.0f)), // 位置は調整してくれ
                    "Resource/Effect/shield_effect.png",
                    15,
                    5,
                    3,
                    3,
                    2.0f // 少し大きくしてる  調整してほしい
                );

                self.pShieldEffect->SetLoop(true); // さっき追加したループフラグをON
            }
            break;


           // プレイヤーのみ
        case EffectType::DRAW:
        {
            // 菊池
            // ドローのSE
            if (effect.MpHeel)
            {
                Master::mpSoundManager->PlaySEDelay(SoundManager::SE_DOROU, 75);
            }
            else if (effect.mpHeel_1)
            {
                Master::mpSoundManager->PlaySEDelay(SoundManager::SE_DOROU, 40);
            }
            else
            {
                Master::mpSoundManager->PlaySE(SoundManager::SE_DOROU);
            }

            // 存在していたら
            if (pPlayer != nullptr)
            {
                // 今の空き枠を計算
                int space = pPlayer->HAND_CARD_MAX - (int)pPlayer->hand.size();

                // 効果枚数と空き枠の、小さい方の数だけ引く
                int actualDraw = (effect.value < space) ? effect.value : space;

                if (actualDraw > 0)
                {
                    // Player::DrawCard は「枚数」を引く関数なのでまとめて呼ぶ
                    pPlayer->DrawCard(actualDraw);
                }
            }
        }
            break;


        case EffectType::ENERGY_MP:
        {
            if (pPlayer != nullptr) // プレイヤーの時だけMPを増やす
            {
                // 菊池
                // MP増加のSE
                Master::mpSoundManager->PlaySE(SoundManager::SE_MP_UP);
                pPlayer->mp += effect.value;
            }
        }
            break;


        case EffectType::APPLY_POISON:
        {
            // EffectからDebuffへ入れる
            Debuff d;
            d.type = DebuffType::DEBUFF_POISON;
            d.value = effect.value; // 毒がある場合 毎ターンダメージ
            d.turns = effect.count; // 持続するターンを入れる
            ApplyDebuffEffect(d, self, target);

            if (target.pPoisonEffect == nullptr)
            {
                // 毒エフェクトアニメーション表示
                target.pPoisonEffect =
                    new Object2D(
                        target.GetPosition(),
                        "Resource/Effect/poison_effect.png",
                        8,
                        8,
                        1,
                        8,
                        1.6f  // 画像大きく
                    );
                target.pPoisonEffect->SetLoop(true); // 毒状態の間ずっと出す
            }

            // 菊池
           // ダメージ付き毒とそうでない毒でSEを変える
            if (effect.parentHasDamage)
            {
                // ダメージ付き毒 → 少し遅らせる（攻撃の後に鳴る）
                Master::mpSoundManager->PlaySEDelay(SoundManager::SE_DOKU, 75);
            }
            else
            {
                // 毒だけ → 即再生
                Master::mpSoundManager->PlaySE(SoundManager::SE_DOKU);
            }

            //毒を受けた側
            target.PlayMotion(AnimationData::AnimType::DAMAGE);

            break;
        }

        case EffectType::APPLY_WEAKEN:
        {
            Debuff d;
            d.type = DebuffType::WEAKEN;
            d.value = effect.value;
            d.turns = effect.count;
            ApplyDebuffEffect(d, self, target);

            // 弱体化エフェクトアニメーション表示
            if (target.pDebuffEffect == nullptr)
            {
                target.pDebuffEffect =
                    new Object2D(
                        VSub(target.GetPosition(), VGet(60.0f, 0.0f, 0.0f)),
                        "Resource/Effect/powerDown_effect.png",
                        10,
                        10,
                        1,
                        5,
                        0.6f
                    );

                target.pDebuffEffect->SetLoop(true); // デバフ中はずっとキラキラ
            }

            // 菊池
            // デバフのSE
            Master::mpSoundManager->PlaySE(SoundManager::SE_DOWN);

            break;
        }

        case EffectType::APPLY_BUFF: // 自分にバフ
        {
            // バフは 自分に付与する
            Buff b;
            b.type = BuffType::ATTACK;
            b.value = effect.value;
            b.turns = effect.count;
            ApplyBuffEffect(b, self, target);

          
            // 攻撃力アップエフェクトアニメーション表示
            if (self.pBuffEffect == nullptr)
            {
                self.pBuffEffect = 
                    new Object2D(
                        VSub(self.GetPosition(), VGet(-60.0f, 0.0f, 0.0f)),
                        "Resource/Effect/powerUp_effect.png",
                        10,
                        10,
                        1,
                        5,
                        0.6f
                    );


                self.pBuffEffect->SetLoop(true); // バフ中はずっとキラキラ
            }

            // 菊池
            // バフのSE
            Master::mpSoundManager->PlaySE(SoundManager::SE_UP);

            break;
        }

        case EffectType::APPLY_CURSE:  //呪い
            {
            int add = (effect.value > 0) ? effect.value : 1;

            target.curse += add; // 呪い足す

                // まだ死んでいないなら ５個貯まったら死亡や
                if (target.hp > 0 && target.curse >= 5)
                {
                    target.hp = 0;
                }

                Master::mpSoundManager->PlaySE(SoundManager::SE_NOROI,180);

                new Object2D(
                    target.GetPosition(),
                    "Resource/Effect/curse_effect.png",
                    16,
                    8,
                    2,
                    8,
                    1.0f
                );


            }
            break;

        // アイテム関係
        // はさみさん
        case EffectType::EXTRA_PLAY:
            if (pPlayer != nullptr)
            {
                pPlayer->doubleNextCard = true;
                std::printf("ハサミ発動：次のカードを2回使います\n");

                // 通知表示
                if (Master::mpSceneManager->GetCurrentScene())
                {
                    GameScene* pGameScene =
                        dynamic_cast<GameScene*>(Master::mpSceneManager->GetCurrentScene());

                    if (pGameScene)
                    {
                        pGameScene->ShowItemMessage(
                            "ハサミ発動!\n\nカードを2回使用する\n",
                            GetColor(205,5,5)
                        );
                    }
                }

                // はさみエフェクト
                new Object2D(
                    pPlayer->GetPosition(),
                    "Resource/Effect/scissors_effect.png",
                    10,
                    2,
                    5,
                    6
                );

                Master::mpSoundManager->PlaySE(
                    SoundManager::SE_KIRU
                );
            }
            break;

            // のりさん
        case EffectType::KEEP_CARD:
            if (pPlayer != nullptr)
            {
                pPlayer->keepNextCardCount += 1;
                std::printf("のり発動：カードを1枚持ち越します\n");

                // 通知表示
                if (Master::mpSceneManager->GetCurrentScene())
                {
                    GameScene* pGameScene =
                        dynamic_cast<GameScene*>(Master::mpSceneManager->GetCurrentScene());

                    if (pGameScene)
                    {
                        pGameScene->ShowItemMessage(
                            "のり発動！\n\nカードを1枚持ち越す\n",
                            GetColor(0, 95, 148)
                        );
                    }
                }

                // のりエフェクト
                new Object2D(
                    VGet(
                        (float)Utility::SCREEN_WIDTH / 2 + 30.0f,
                        (float)Utility::SCREEN_HEIGHT / 2,
                        0.0f
                    ),
                    "Resource/Effect/glue_effect.png",
                    6,
                    2,
                    3,
                    8,
                    1.8f
                );
            }
            break;


            // ペンさん
        case EffectType::PEN_RANDOM:
            if (pPlayer != nullptr)
            {
                // ランダムで取得
                int r = GetRand(2);

                if (r == 0)
                {
                    // 回復
                    int heal = pPlayer->mp * 7;
                    pPlayer->hp += heal;
                    pPlayer->mp = 0;
                    if (pPlayer->hp > pPlayer->maxHp) pPlayer->hp = pPlayer->maxHp;

                    pPlayer->itemEffectText = "ペン：回復 " + std::to_string(heal);

                    if (Master::mpSceneManager->GetCurrentScene())
                    {
                        GameScene* pGameScene =
                            dynamic_cast<GameScene*>(Master::mpSceneManager->GetCurrentScene());

                        if (pGameScene)
                        {
                            pGameScene->ShowItemMessage(
                                "ペン発動！\n\nHPを " + std::to_string(heal) + " 回復",
                                GetColor(40, 40, 40)
                            );
                        }
                    }

                    std::printf("回復(%d)\n", heal);


					// 回復のエフェクトアニメーション表示
                    new Object2D(
                        VSub(pPlayer->GetPosition(), VGet(30.0f, 0.0f, 0.0f)),
                        "Resource/Effect/heal_effect.png",
                        8,
                        8,
                        1,
                        7,
                        2.0f
                    );

                    Master::mpSoundManager->PlaySE(SoundManager::SE_HEAL,180);

                }
                else if (r == 1)
                {
                    // スタン
                    Enemy* pEnemy = dynamic_cast<Enemy*>(&target);
                    if (pEnemy != nullptr)
                    {
                        pEnemy->stunTurns = 2;
                    }

                    pPlayer->itemEffectText = "敵は2ターン行動不能！";

                    if (Master::mpSceneManager->GetCurrentScene())
                    {
                        GameScene* pGameScene =
                            dynamic_cast<GameScene*>(Master::mpSceneManager->GetCurrentScene());

                        if (pGameScene)
                        {
                            pGameScene->ShowItemMessage(
                                "ペン発動！\n\n敵を2ターンスタン！",
                                GetColor(40, 40, 40)
                            );
                        }
                    }

					// スタンのエフェクトアニメーション表示
                    if (target.pStunEffect == nullptr)
                    {
                        target.pStunEffect = 
                            new Object2D(
                                VSub(target.GetPosition(), VGet(0.0f, 150.0f, 0.0f)),
                                "Resource/Effect/stun_effect.png",
                                5,
                                5,
                                1,
                                5
                            );

                        target.pStunEffect->SetLoop(true); // スタンの間ずっと出す
                    }

                    Master::mpSoundManager->PlaySE(SoundManager::SE_BIRIBIRI);

                }
                else
                {
                    // コストランダム
                    pPlayer->randomizeCostThisTurn = true;
                    for (auto* pCard : pPlayer->hand)
                    {
                        if (!pCard) continue;

                        int newCost = GetRand(3);
                        pCard->SetCost(newCost);
                        pCard->ResetFrameByCost();
                    }

                    pPlayer->itemEffectText = "手札コスト変動！";

                    if (Master::mpSceneManager->GetCurrentScene())
                    {
                        GameScene* pGameScene =
                            dynamic_cast<GameScene*>(Master::mpSceneManager->GetCurrentScene());

                        if (pGameScene)
                        {
                            pGameScene->ShowItemMessage(
                                "ペン発動！\n\n手札コスト変動！",
                                GetColor(40, 40, 40)
                            );
                        }
                    }


                    Master::mpSoundManager->PlaySE(SoundManager::SE_HENNDOU,180);
                }


                // ペンのエフェクトアニメーション表示
                new Object2D(
                    VGet((float)Utility::SCREEN_WIDTH / 2 + 60.0f, (float)Utility::SCREEN_HEIGHT / 2 + 130.0f, 0.0f),
                    "Resource/Effect/pen_effect.png",
                    6,
                    2,
                    3,
                    7,
                    1.8f
                );

            }
            break;
    }
}