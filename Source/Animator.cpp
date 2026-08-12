#include "Animator.h"
#include "DxLib.h"

// アニメの再生開始
void Animator::Play(const AnimationData* anim)
{
	timer = 0;	 // 再生時間リセット
	currentAnim = anim;	 // 再生するアニメをリセット
}

//　アニメーションのタイプを探す
AnimationData::AnimType Animator::GetCurrentAnimType()  const
{
    if (currentAnim == nullptr)
    {
        // 何も再生されていなければIDLEにしている
        return AnimationData::AnimType::IDLE;
    }
    return currentAnim->GetType();  //今再生中のやつのタイプを返す
}


// アニメーションが終わったか
bool Animator::IsFinished() const
{
    // timer が再生時間以上になったら終了
    if (currentAnim != nullptr && !currentAnim->IsLoop())
    {
        return timer >= currentAnim->GetDuration();
    }
    return false;
}

void Animator::Update(std::vector<Part>& parts)
{
    if (!currentAnim) return;

    timer++;

    const auto& allFrames = currentAnim->GetFrames();
    if (allFrames.empty()) return;

    // 全パーツをチェック
    for (int i = 0; i < (int)parts.size(); i++)
    {
        // このパーツ用のデータがあるか探す
        std::vector<AnimationData::AnimFrame> targetFrames;
        for (const auto& f : allFrames)
        {
            if (f.owner == ownerType && f.partIndex == i) targetFrames.push_back(f);
        }

        // データが見つからないパーツは、座標を上書きせずにスルー
        if (targetFrames.empty()) continue;


        // 全体の再生時間現在の経過時間が何番目のコマにあるかを計算
        // 現在の時間 × そのパーツの総コマ数) ÷ アニメ全体の長さ
        int frameIndex = (timer * (int)targetFrames.size()) / currentAnim->GetDuration();

        // アクセスを防ぐためのガード
        if (frameIndex >= (int)targetFrames.size())
        {
            frameIndex = (int)targetFrames.size() - 1;
        }

        // 決定したコマframeIndexから　具体的なポーズデータを読みこみ
        const auto& f = targetFrames[frameIndex];

        // パーツごとの移動
        parts[i].targetAngle = f.targetAngle; // 次の目標角度
        parts[i].targetXOffset = f.targetX;     // 次の目標X座標
        parts[i].targetYOffset = f.targetY;     // 次の目標Y座標
    }

    // 再生終了判定
    if (timer >= currentAnim->GetDuration())
    {
        if (currentAnim->IsLoop()) timer = 0;
        else timer = currentAnim->GetDuration();
    }
}