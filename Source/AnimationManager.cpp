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
    float angle = 0.0f; // 回転
};

static void AddComplexDegreeFrames(AnimationData& anim, AnimationData::AnimOwner owner, int partIdx, const std::vector<AnimStep>& steps)
{
    //ラジアンを角度に変更
    const float DEG_TO_RAD = DX_PI_F / 180.0f;

    for (const auto& s : steps)
    {
        AnimationData::AnimFrame f;
		f.owner = owner;
        f.partIndex = partIdx;
        f.targetX = s.x;
        f.targetY = s.y;
        // 度をラジアンに変換
        f.targetAngle = s.angle * DEG_TO_RAD;

        anim.AddFrame(f);
    }
}

// 実体の定義
std::unordered_map<AnimationData::AnimType, AnimationData> AnimationManager::animCatalog;


//     大谷の引数ガイド
//     AddComplexDegreeFrames(
//        anim,                // [第1引数] 登録先のアニメーションデータ (anim)
//        Player::PartID::BODY,// [第2引数] 動かしたいパーツのID (BODY, WEAPON, EYE など)
//        {                    // [第3引数] ポーズ(AnimStep)のリスト。{} で囲って並べる
//            // { X移動, Y移動, 回転角度(度) }
// 
//            {  0.0f,   0.0f,   0.0f }, // 1つめのポーズ（開始）
//            {  0.0f, -10.0f,  15.0f }, // 2つめのポーズ（途中経過）
//            {  0.0f,   0.0f,   0.0f }  // 3つめのポーズ（終了）
//        }
//     );



 // アニメーションの初期化
void AnimationManager::Initialize()
{
    for (int i = 0; i < (int)AnimationData::AnimType::ANIMATION_MAX; i++)
    {                    
        auto type = static_cast<AnimationData::AnimType>(i); // 数字を名前に変更
        auto& anim = animCatalog[type];   //forで回したやつを引っ張ってる
        anim.SetType(type);

        // デフォルトでこの時間とループなしを設定
        anim.SetDuration(60);
        anim.SetLoop(false); // ループnasi

        //タイプごとに分けている
        switch (type)
        {
        case AnimationData::AnimType::IDLE:
            anim.SetLoop(true); // ループあり
                                    
            // プレイヤー用
            {
                // 胴体
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::BODY,
                    {
                        { 0.0f,   0.0f, 0.0f },
                        { 0.0f,  -2.5f, 0.0f },
                        { 0.0f,  -5.0f, 0.0f },
                        { 0.0f,   2.5f, 0.0f },
                        { 0.0f,   0.0f, 0.0f }
                    });

                // 頭
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::HEAD,
                    {
                        { 0.0f,   0.0f, 0.0f },
                        { 0.0f,  -2.5f, 1.0f },
                        { 0.0f,  -5.0f, 3.0f },
                        { 0.0f,   2.5f, 1.0f },
                        { 0.0f,   0.0f, 0.0f }
                    });

                // 右手
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::ARM_R,
                    {
                        { 0.0f,   0.0f, 0.0f }, 
                        { 0.0f,  -2.5f, 0.0f }, 
                        { 0.0f,  -5.0f, 5.0f },
                        { 0.0f,   2.5f, 0.0f },
                        { 0.0f,   0.0f, 0.0f }
                    });

                // 武器
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::WEAPON,
                    {
                        { 0.0f,   0.0f, 0.0f }, 
                        { 0.0f,  -2.5f, 0.0f }, 
                        { 0.0f,  -5.0f, 5.0f },
                        { 0.0f,   2.5f, 0.0f },
                        { 0.0f,   0.0f, 0.0f }
                    });

                // 右足
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::LEG_R,
                    {
                        { 0.0f,   0.0f, 0.0f }, 
                        { 0.0f,  -1.5f, 0.0f }, 
                        { 0.0f,  -3.0f, 0.0f },
                        { 0.0f,   1.5f, 0.0f },
                        { 0.0f,   0.0f, 0.0f }
                    });

                // 左足
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::LEG_L,
                    {
                        { 0.0f,   0.0f, 0.0f }, 
                        { 0.0f,  -1.5f, 0.0f }, 
                        { 0.0f,  -3.0f, 0.0f },
                        { 0.0f,   1.5f, 0.0f },
                        { 0.0f,   0.0f, 0.0f }
                    });

            }
          
            // 敵用のアニメ関係
            {
				// 全体　待機
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::ENEMY, Enemy::PartID::BODY,
                    {
                        { 0.0f,   0.0f, 0.0f },
                        { 0.0f,  -2.5f, 0.0f },
                        { 0.0f,  -5.0f, 0.0f },
                        { 0.0f,   2.5f, 0.0f },
                        { 0.0f,   0.0f, 0.0f }
                    });
            }

            break;

        case AnimationData::AnimType::ATTACK:
            anim.SetLoop(false);
            anim.SetDuration(40);

            // プレイヤー用
            {
                // 胴体 
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::BODY,
                    {
                        {   0.0f,  10.0f,  0.0f },
                        {   0.0f,  0.0f,  0.0f },
                        {   0.0f,  0.0f,  0.0f } 
                    });

                // 頭 
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::HEAD,
                    {
                        {   0.0f,  0.0f,-10.0f },
                        {   0.0f,  0.0f,  0.0f },
                        {   0.0f,  0.0f,  0.0f } 
                    });

                // 武器
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::WEAPON,
                    {
                       
                        {  -5.0f, -10.0f,  -40.0f }, // 上げる
                        {   0.0f,  14.0f,   30.0f }, // 下げる
                        {   0.0f,   0.0f,    0.0f }
                    }
                );

                // 腕
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::ARM_R,
                    {
                        {  -5.0f, -10.0f,  -40.0f }, // 上げる
                        {   0.0f,  14.0f,   30.0f }, // 下げる
                        {   0.0f,   0.0f,    0.0f }
                    }
                );
            }

           
            // 敵用のアニメ関係
            {
                // 全部　攻撃
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::ENEMY, Enemy::PartID::BODY,
                    {
                        { -80.0f,   0.0f,  -30.0f },
                        { 0.0f,   0.0f,  0.0f }
                    });
            }

            break;

        case AnimationData::AnimType::DAMAGE:
            anim.SetDuration(80);
            anim.SetLoop(false);
            anim.SetDuration(40);

            // プレイヤー用
            {
                // 胴体
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::BODY,
                    {
                        { -30.0f, 0.0f,-30.0f },
                        { 0.0f,   0.0f,  0.0f }
                    });

                // 頭
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::HEAD,
                    {
                      { -30.0f, 0.0f,-30.0f },
                      { 0.0f,   0.0f,  0.0f }
                    });

                // 右手
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::ARM_R,
                    {
                       { -30.0f, 0.0f,-30.0f },
                       { 0.0f,   0.0f,  0.0f }
                    });

                // 武器
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::WEAPON,
                    {
                     { -30.0f, 0.0f,-40.0f },
                     { 0.0f,   0.0f,  0.0f }
                    });

                // 右足
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::LEG_R,
                    {
                       { 0.0f,   0.0f,-3.0f },
                       { 0.0f,   0.0f, 0.0f }
                    });

                // 左足
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::LEG_L,
                    {
                      { -30.0f, 0.0f,-20.0f },
                      { 0.0f,   0.0f,  0.0f }
                    });

            }

            // 敵用のアニメ関係
            {
                // 全部　ダメージ
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::ENEMY, Enemy::PartID::BODY,
                    {
                        { 80.0f,   0.0f, 30.0f },
                        { 0.0f,   0.0f,  0.0f }
                    });
            }


            break;



        case AnimationData::AnimType::SPECIAL: // 必殺技
            anim.SetDuration(50);
            anim.SetLoop(false);

            // プレイヤー用
            {
                // 胴体 
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::BODY,
                    {
                        {   400.0f,  10.0f, 0.0f },
                        {   400.0f,  0.0f,  0.0f },
                        {   0.0f,  0.0f,  0.0f }
                    });

                // 頭 
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::HEAD,
                    {
                        {   400.0f,  0.0f,-10.0f },
                        {   400.0f,  0.0f,  0.0f },
                        {   0.0f,  0.0f,  0.0f }
                    });

                // 武器
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::WEAPON,
                    {

                        {   400.0f, -10.0f,  -40.0f }, // 上げる
                        {   400.0f,  20.0f,   30.0f }, // 下げる
                        {   0.0f,   0.0f,    0.0f }
                    }
                );

                // 腕
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::ARM_R,
                    {
                        {   400.0f, -10.0f,  -40.0f }, // 上げる
                        {   400.0f,  20.0f,   30.0f }, // 下げる
                        {   0.0f,   0.0f,    0.0f }
                    }
                );

                // 左足
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::LEG_L,
                    {
                        {   400.0f, 0.0f,    0.0f }, // 上げる
                        {   400.0f, 0.0f,  0.0f }, // 下げる
                        {   0.0f,   0.0f,    0.0f }
                    }
                );

                // 右足
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::LEG_R,
                    {
                        {   400.0f, 0.0f,    0.0f }, // 上げる
                        {   400.0f, 0.0f,  0.0f }, // 下げる
                        {   0.0f,   0.0f,    0.0f }
                    }
                );
            }


            // 敵用のアニメ関係
            {
                // 全部　必殺
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::ENEMY, Enemy::PartID::BODY,
                    {
                        { -400.0f,   0.0f, 60.0f },
                        { -400.0f,   0.0f, 60.0f },
                        { -400.0f,   0.0f, 0.0f },
                        { 0.0f,   0.0f,  0.0f }
                    });
            }

            break;


        case AnimationData::AnimType::DEATH: // 死亡
            anim.SetDuration(80);
            anim.SetLoop(false);

            // プレイヤー用
            {
                // 胴体 
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::BODY,
                    {
                        {   0.0f,  0.0f,  0.0f },
                        {   0.0f,  120.0f,  90.0f },
                        {   0.0f,  120.0f,  90.0f },
                        {   0.0f,  120.0f,  90.0f }
                    });

                // 頭 
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::HEAD,
                    {
                        {   0.0f,  0.0f,  0.0f },
                        {   0.0f,  120.0f,  90.0f },
                        {   0.0f,  120.0f,  90.0f },
                        {   0.0f,  120.0f,  90.0f }
                    });

                // 武器
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::WEAPON,
                    {
                        {  -10.0f,  20.0f,  40.0f },
                        {   0.0f,  100.0f,  0.0f },
                        {   0.0f,  100.0f,  0.0f },
                        {   0.0f,  100.0f,  0.0f }
                    }
                );

                // 腕
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::ARM_R,
                    {
                        {  -10.0f,  30.0f,  40.0f },
                        {   34.0f,  100.0f,  0.0f },
                        {   34.0f,  100.0f,  0.0f },
                        {   34.0f,  100.0f,  0.0f }
                    }
                );

                // 左足
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::LEG_L,
                    {
                        {   0.0f,  0.0f,  0.0f },
                        {   0.0f,  110.0f,  100.0f },
                        {   0.0f,  110.0f,  100.0f },
                        {   0.0f,  110.0f,  100.0f }
                    }
                );

                // 右足
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::PLAYER, Player::PartID::LEG_R,
                    {
                        {   0.0f,  0.0f,  0.0f },
                        {   30.0f,  110.0f,  60.0f },
                        {   30.0f,  110.0f,  60.0f },
                        {   30.0f,  110.0f,  60.0f }
                    }
                );
            }



            // 敵用のアニメ関係
            {
                // 全部　死亡
                AddComplexDegreeFrames(anim, AnimationData::AnimOwner::ENEMY, Enemy::PartID::BODY,
                    {
                        { 0.0f,   0.0f,  0.0f },
                        { 0.0f,   0.0f,180.0f },
                        { 0.0f,   0.0f,180.0f }
                    });
            }

            break;
        }
    }
}

//アニメーションデータの作成
const AnimationData* AnimationManager::GetAnim(AnimationData::AnimType type)
{
    if (animCatalog.count(type) > 0)
    {
        return &animCatalog[type];
    }
    return nullptr;
}