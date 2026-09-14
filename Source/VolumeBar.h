#pragma once

// 音量調節バーを管理・描画するクラス
// SEとBGMのバーを管理して音量に応じたバーの描画を行う。
// オブジェクト指向的にするためにクラス化した。　大谷
class VolumeBar
{
public:
    // 音量バーを生成する
    // x, y      : 音量バーの左上座標
    // width     : 音量バーの幅
    // height    : 音量バーの高さ
    // color     : 音量バーの色
    VolumeBar(int inX, int inY, int inWidth, int inHeight, int inColor);

    void Draw(int inVolume);

private:
    int mnX;        // 音量バーのX座標
    int mnY;        // 音量バーのY座標
    int mnWidth;    // 音量バーの幅
    int mnHeight;   // 音量バーの高さ
    int mnColor;    // 音量バーの色
};
