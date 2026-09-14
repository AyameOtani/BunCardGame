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
	// 音量に応じたバーの幅を計算 100を上限にするため
    int volumeWidth = inVolume * mnWidth / 100;

    // 音量に応じたバーを描画
    DrawBox(
        mnX,
        mnY,
        mnX + volumeWidth,
        mnY + mnHeight,
        mnColor,
        TRUE
    );

    // バーの枠を描画
    DrawBox(
        mnX,
        mnY,
        mnX + mnWidth,
        mnY + mnHeight,
        GetColor(255, 255, 255),
        FALSE
    );
}