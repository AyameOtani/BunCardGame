#include "VolumeBar.h"
#include "DxLib.h"

VolumeBar::VolumeBar(int inX, int inY, int inWidth, int inHeight, int inColor)
    : mnX(inX)
    , mnY(inY)
    , mnWidth(inWidth)
    , mnHeight(inHeight)
    , mnColor(inColor)
{
}

void VolumeBar::Draw(int inVolume)
{
    // 現在の音量に応じたバーの描画幅を算出するため
    int volumeWidth = inVolume * mnWidth / MaxVolume;

    // 音量に応じた現在のバーの塗りつぶしを描画するため
    DrawBox(
        mnX,
        mnY,
        mnX + volumeWidth,
        mnY + mnHeight,
        mnColor,
        TRUE
    );

    // 音量バー全体の最大範囲を示す枠線を表現するため
    DrawBox(
        mnX,
        mnY,
        mnX + mnWidth,
        mnY + mnHeight,
        GetColor(FrameColorMax, FrameColorMax, FrameColorMax),
        FALSE
    );
}