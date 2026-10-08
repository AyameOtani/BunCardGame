#pragma once

#include <memory>
#include "MouseGraph.h"
#include "VolumeBar.h"
#include "VolumeTextSet.h"

// 音量設定画面全体を管理するクラス
// タイトルシーンから音量設定に関する処理を完全に分離して独立させるため
class TitleOption
{
public:
    TitleOption();
    ~TitleOption();

    void Initialize();
    void Update();
    void Draw() const;

    // オプション画面のアクセサ
    bool GetIsOpen() const { return mbIsOpen; }
    void SetIsOpen(bool open) { mbIsOpen = open; }

private:
    // 音量バーの入力とサウンドマネージャーへの反映を更新するため
    void UpdateVolumeInput();

private:
    // 閉じるボタン
    std::unique_ptr<MouseGraph> musicCloseButton;

    // 音量バー
    VolumeBar mBgmVolumeBar;
    VolumeBar mSeVolumeBar;

    // 音量テキストとパネル描画
    VolumeTextSet mVolumeTextSet;

    // オプション画面が開いているかどうか
    bool mbIsOpen;
};