#include "TitleOption.h"
#include "Master.h"
#include "GameConstants.h"
#include "DxLib.h"

TitleOption::TitleOption()
    : musicCloseButton(nullptr)
    , mBgmVolumeBar(
        TitlePosition::VolumeBarX,
        TitlePosition::BgmBarY,
        TitlePosition::VolumeBarWidth,
        TitlePosition::VolumeBarHeight,
        ColorOption::BgmBar
    )
    , mSeVolumeBar(
        TitlePosition::VolumeBarX,
        TitlePosition::SeBarY,
        TitlePosition::VolumeBarWidth,
        TitlePosition::VolumeBarHeight,
        ColorOption::SeBar
    )
    , mbIsOpen(false)
{

}

TitleOption::~TitleOption()
{
    musicCloseButton.reset();
}

void TitleOption::Initialize()
{
    // オプション画面内の閉じるボタンを生成するため
    musicCloseButton = std::make_unique<MouseGraph>(
        TitlePosition::MusicCloseX,
        TitlePosition::MusicCloseY,
        0.0f,
        TitleResourcePath::MusicClose.c_str(),
        TitleScale::MusicCloseNormal,
        TitleScale::MusicCloseHover
    );

    mbIsOpen = false;
}

void TitleOption::Update()
{
    if (!mbIsOpen)
    {
        return;
    }

    // 閉じるボタンの入力更新を行うため
    if (musicCloseButton)
    {
        musicCloseButton->Update();

        // 閉じるボタンが押されたらオプション画面を閉じるため
        if (musicCloseButton->IsClicked())
        {
            Master::mpGameManager
                ->GetSoundManager()
                ->PlaySE(SoundManager::SE_DECIDE);

            mbIsOpen = false;
        }
    }

    // 音量バーのドラッグ・クリック入力を処理するため
    UpdateVolumeInput();
}

void TitleOption::Draw() const
{
    if (!mbIsOpen)
    {
        return;
    }

    // 音量設定の背景パネルと黒い半透明背景を描画するため
    const_cast<VolumeTextSet&>(mVolumeTextSet).DrawVolumeSettingPanel();

    // BGMとSEのテキストおよびバーを描画するため
    mVolumeTextSet.DrawBgmText(const_cast<VolumeBar&>(mBgmVolumeBar));
    mVolumeTextSet.DrawSeText(const_cast<VolumeBar&>(mSeVolumeBar));

    // 閉じるボタンを描画するため
    if (musicCloseButton)
    {
        musicCloseButton->Draw();
    }
}


void TitleOption::UpdateVolumeInput()
{
    int mouseX;
    int mouseY;
    GetMousePoint(&mouseX, &mouseY);

    // 左クリックが押されていない場合は処理しないため
    if (!(GetMouseInput() & MOUSE_INPUT_LEFT))
    {
        return;
    }

    // BGMバーの操作を判定するため
    if (mBgmVolumeBar.IsMouseOver(mouseX, mouseY))
    {
        int bgmVolume = mBgmVolumeBar.GetVolumeFromMouse(mouseX);

        Master::mpGameManager
            ->GetSoundManager()
            ->SetBGMVolume(bgmVolume);

        Master::mpGameManager
            ->GetSoundManager()
            ->SaveVolume();
    }

    // SEバーの操作を判定するため
    if (mSeVolumeBar.IsMouseOver(mouseX, mouseY))
    {
        int seVolume = mSeVolumeBar.GetVolumeFromMouse(mouseX);

        Master::mpGameManager
            ->GetSoundManager()
            ->SetSEVolume(seVolume);

        Master::mpGameManager
            ->GetSoundManager()
            ->SaveVolume();
    }
}