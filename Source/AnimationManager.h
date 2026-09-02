#pragma once
#include "AnimationData.h"
#include <unordered_map>

/// <summary>
/// アニメーションのデータを管理するクラス
/// cppでパーツ一つ一つにどのように動いてほしいかを命令するためのクラス
/// </summary>
class AnimationManager
{
public:
	static void Initialize(); // メインでよんでる

	// 登録されたアニメーションデータを取得する
	static const AnimationData* GetAnim(AnimationData::AnimType type);

private:
	// アニメーションの種類と実体を紐づけて保存する
	static std::unordered_map<AnimationData::AnimType, AnimationData> animCatalog;
};



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

//1    // 名前で読み込んでる 重ねていく番で読み込む
//1    parts[PartID::LEG_L].graphHandle = LoadGraph("Resource/Player/Leg_L.png");
//1    parts[PartID::LEG_R].graphHandle = LoadGraph("Resource/Player/Leg_R.png");
//1    parts[PartID::WEAPON].graphHandle = LoadGraph("Resource/Player/Weapon.png");
//1    parts[PartID::HEAD].graphHandle = LoadGraph("Resource/Player/Head.png");
//1    parts[PartID::BODY].graphHandle = LoadGraph("Resource/Player/Body.png");
//1    parts[PartID::ARM_R].graphHandle = LoadGraph("Resource/Player/Arm.png");