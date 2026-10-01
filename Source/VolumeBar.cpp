#include "VolumeBar.h"
#include "DxLib.h"

VolumeBar::VolumeBar(
	int inX,
	int inY,
	int inWidth,
	int inHeight,
	int inColor
)
	: mnX(inX)
	, mnY(inY)
	, mnWidth(inWidth)
	, mnHeight(inHeight)
	, mnColor(inColor)
{
}

void VolumeBar::Draw(int inVolume)
{
	// 現在の音量に応じたバーの描画幅を算出する
	int volumeWidth = inVolume * mnWidth / MaxVolume;

	// 音量に応じた現在のバーを描画する
	DrawBox(
		mnX,
		mnY,
		mnX + volumeWidth,
		mnY + mnHeight,
		mnColor,
		TRUE
	);

	// 音量バー全体の最大範囲を示す枠線を描画する
	DrawBox(
		mnX,
		mnY,
		mnX + mnWidth,
		mnY + mnHeight,
		GetColor(
			FrameColorMax,
			FrameColorMax,
			FrameColorMax
		),
		FALSE
	);
}

bool VolumeBar::IsMouseOver(int inMouseX, int inMouseY) const
{
	return
		inMouseX >= mnX &&
		inMouseX <= mnX + mnWidth &&
		inMouseY >= mnY &&
		inMouseY <= mnY + mnHeight;
}

int VolumeBar::GetVolumeFromMouse(int inMouseX) const
{
	int volume =
		(inMouseX - mnX) * MaxVolume / mnWidth;

	// 音量が0未満にならないようにする
	if (volume < 0)
	{
		volume = 0;
	}

	// 音量が100を超えないようにする
	if (volume > MaxVolume)
	{
		volume = MaxVolume;
	}

	return volume;
}