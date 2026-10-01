#pragma once
/// 画像パスや色などの定数を定義するヘッダファイル
#include <string>
#include <DxLib.h>

//  画面の大きさに関する設定
namespace ScreenSize
{
	static constexpr int Width = 1920;
	static constexpr int Height = 1080;

	// 半分もよく使うから追加
	static constexpr int CenterX = Width / 2;
	static constexpr int CenterY = Height / 2;
}


// 音量に関する設定
namespace SoundSetting
{
	static constexpr int VolumeMin = 0;
	static constexpr int VolumeMax = 100;
}



// タイトル画面のリソースパスを定義する名前空間
namespace  TitleResourcePath
{
	static const std::string GameStartButton = "Resource/Title/Start.png"; // ゲームスタートボタンの画像パス
	static const std::string ExplainButton = "Resource/Title/Explain.png"; // 操作説明ボタンの画像パス
	static const std::string Rogo = "Resource/Title/Rogo.png"; // ロゴの画像パス
	static const std::string Bagground = "Resource/Title/Title.png"; // タイトル画面背景の画像パス
	static const std::string MoveBagGround = "Resource/Title/move_bag_ground.png"; // 動く黒板の画像パス
	static const std::string VolumeSettingsBg = "Resource/Title/Titleboard.png"; // 音量設定背景の画像パス
	static const std::string MusicNote = "Resource/Title/MusicNote.png"; // 音符の画像パス
	static const std::string MusicClose = "Resource/Title/MusicClose.png"; // 音量設定の×ボタンの画像パス
	static const std::string OptionButton = "Resource/Title/option.png"; // 音量調整ボタンの画像パス
}

// タイトル画面の座標・サイズ
namespace TitlePosition
{
	// 画面中央からの位置
	static constexpr int CardTargetOffsetX = ScreenSize::CenterX;
	static constexpr int CardTargetOffsetY = ScreenSize::CenterY -30;

	static constexpr int StartTargetOffsetX = -320;
	static constexpr int StartTargetOffsetY = 200;

	static constexpr int ExplainTargetOffsetX = 320;
	static constexpr int ExplainTargetOffsetY = 200;

	// ボタン初期位置
	static constexpr float StartInitialX = -300.0f;
	static constexpr float ExplainInitialX = 300.0f;

	static constexpr float ButtonInitialYOffset = 150.0f;

	// オプションボタン
	static constexpr float OptionButtonX = 100.0f;
	static constexpr float OptionButtonY = 100.0f;

	// 音量設定の×ボタン
	static constexpr float MusicCloseX = 1570.0f;
	static constexpr float MusicCloseY = 370.0f;

	// 音量バー
	static constexpr int VolumeBarX = 570;
	static constexpr int VolumeBarWidth = 840;
	static constexpr int VolumeBarHeight = 40;

	static constexpr int BgmBarY = 460;
	static constexpr int SeBarY = 650;

	// 音量バーの当たり判定
	static constexpr int VolumeBarUp = 40;
	static constexpr int VolumeBarDown = 65;
	static constexpr int VolumeBarDist = 20;

	// 音量表示
	static constexpr int VolumeNumberXOffset = -20;
	static constexpr int VolumeNumberYOffset = 70;
	static constexpr int MusicNoteOffset = 15;

	// BGM・SE文字
	static constexpr int BgmTextX = 370;
	static constexpr int BgmTextY = 450;

	static constexpr int SeTextX = 370;
	static constexpr int SeTextY = 640;

	// 音量設定タイトル
	static constexpr int VolumeTitleX = 800;
	static constexpr int VolumeTitleY = 300;

	// ロゴ
	static constexpr int LogoBaseYOffset = -150;
}


// タイトル画面の拡大率
namespace TitleScale
{
	// Startボタン
	static constexpr float StartNormal = 0.4f;
	static constexpr float StartHover = 0.45f;

	// Explainボタン
	static constexpr float ExplainNormal = 0.4f;
	static constexpr float ExplainHover = 0.45f;

	// オプションボタン
	static constexpr float OptionNormal = 0.35f;
	static constexpr float OptionHover = 0.39f;

	// ×ボタン
	static constexpr float MusicCloseNormal = 0.30f;
	static constexpr float MusicCloseHover = 0.35f;

	// 背景
	static constexpr float Background = 1.12f;

	// ロゴ
	static constexpr float Logo = 0.45f;

	// 音符
	static constexpr float MusicNote = 0.3f;

	// 音量設定背景
	static constexpr float VolumeSettingsBackground = 1.0f;
}


// タイトル画面の演出
namespace TitleAnimation
{
	// ボタン移動
	static constexpr float ButtonMoveSpeed = 0.08f;
	static constexpr float ButtonStopDistance = 1.0f;

	// ロゴ
	static constexpr float LogoInitialY = -800.0f;
	static constexpr float LogoMoveSpeed = 0.05f;
	static constexpr float LogoSwingSize = 17.0f;

	// ロゴのバネ演出
	static constexpr float Gravity = 0.02f;
	static constexpr float SpringPower = 0.04f;
	static constexpr float Damping = 0.87f;
	static constexpr float VelocityStop = 0.3f;
	static constexpr float TargetStopDistance = 0.3f;

	// カード演出
	static constexpr float CardMoveSpeed = 0.09f;
	static constexpr float CardInitialX = -200.0f;
	static constexpr float CardInitialY = -200.0f;

	static constexpr float CardStopDistanceX = 14.0f;
	static constexpr float CardStopDistanceY = 14.0f;
	static constexpr float CardStopDistanceRota = 6.0f;

	static constexpr float CardRotaIncrease = 0.1f;
	static constexpr float CardRotaMax = 2.0f;

	// 白いBOX
	static constexpr float WhiteBoxAlphaIncrease = 4.0f;
	static constexpr float WhiteBoxMaxAlpha = 255.0f;
	static constexpr float WhiteBoxSceneChangeAlpha = 254.0f;
}


// よく使用する色
namespace ColorOption
{
	static const int White = GetColor(255, 255, 255);
	static const int Black = GetColor(0, 0, 0);

	static const int BgmBar = GetColor(238, 136, 214);
	static const int SeBar = GetColor(252, 235, 98);
}


// タイトル画面のその他設定
namespace TitleSetting
{
	// 音量
	static constexpr int VolumeMin = 0;
	static constexpr int VolumeMax = 100;

	// 音量設定背景の暗さ
	static constexpr int VolumeBackgroundAlpha = 120;

	// 背景・ロゴの描画位置
	static constexpr int BackgroundOffsetX = -50;
	static constexpr int LogoOffsetX = 0;
}

// アニメーションに必要な設定
namespace AnimOption
{
	static constexpr float PartSpeed = 0.2f; // パーツを動かす速さ
}

// プレイヤーのステータス関係
namespace PlayerStatus
{
	static const int InitialHp = 100; // 初期HP
	static const int InitiaMp = 3; // 初期MP
	static const int MaxSpacialCharge = 100; // 最大必殺ゲージ
	static constexpr float InitScale = 1.7f; // 画像の大きさ
}

// アイテム画像パス
namespace ItemGraphPath
{
	// はさみ、のり、ペンの画像パス
	std::string const Scissors = "Resource/Item/img_scissors.png";
	std::string const Glue = "Resource/Item/img_glue.png";
	std::string const Pen = "Resource/Item/img_pen.png";

	// 使用後のパス
	std::string const Use_Scissors = "Resource/Item/img_use_scissors.png";
	std::string const Use_Glue = "Resource/Item/img_use_glue.png";
	std::string const Use_Pen = "Resource/Item/img_use_pen.png";
}


// スコア関係の設定
namespace ScoreSetting
{
	static constexpr int HpScoreStartTime = 60; // HPスコアの表示を開始する時間
	static constexpr int TurnScoreStartTime = 120; // 経過ターンスコアの表示を開始する時間
	static constexpr int UseCardScoreStartTime = 180; // 使用カード枚数の表示を開始する時間
	static constexpr int UseItemScoreStartTime = 240; // 使用アイテム数の表示を開始する時間

	static constexpr int ScoreX = 650; // スコア文字のX座標
	static constexpr int TextSize = 45; // スコア文字の大きさ

	static constexpr int ScoreNumberXOffset = 600; // スコアの数字を表示するX座標のずらし幅
	static constexpr int ScoreTargetY = 520; // スコア文字の最終的なY座標
	static constexpr int TurnScoreYOffset = 60; // 経過ターン数のY座標のずらし幅
	static constexpr int UseCardScoreYOffset = 150; // 使用カード枚数のY座標のずらし幅
	static constexpr int UseItemScoreYOffset = 210; // 使用アイテム数のY座標のずらし幅


	static constexpr float Gravity = 0.1f; // スコア演出の重力
	static constexpr float SpringPower = 0.25f; // スコア演出のバネの強さ
	static constexpr float Damping = 0.70f; // スコア演出の減衰

	static constexpr float StopDistance = 0.6f; // スコア演出が停止したと判定する距離


	static constexpr int HpScoreMax = 75;      // HPがこの値以上なら最高ランク
	static constexpr int HpScoreNormal = 40;   // HPがこの値以上なら通常ランク
	static constexpr int TurnScoreMax = 8;     // ターン数がこの値以下なら最高ランク
	static constexpr int TurnScoreNormal = 16; // ターン数がこの値以下なら通常ランク
	static constexpr int RankImageY = 400;      // ランク画像を描画するY座標
	static constexpr float RankImageScale = 0.2f; // ランク画像の拡大率

	static constexpr float TurnMoveYInitialValue = -100.0f;		// 経過ターン数の文字のY座標の初期値
	static constexpr float UseCardMoveYInitialValue = -100.0f;  // 使用カード枚数の文字のY座標の初期値
	static constexpr float UseItemMoveYInitialValue = -100.0f;  // 使用アイテム数の文字のY座標の初期値
	static constexpr float MoveYInitialValue = -100.0f;			// HPの文字のY座標の初期値
	static constexpr float TurnBoxInitialValue = -100.0f;		// ターン数の背景のY座標の初期値
}