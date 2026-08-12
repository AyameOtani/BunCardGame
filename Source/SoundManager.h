#pragma once
#include <string>
#include <vector>


class SoundManager
{
public:   // enum, struct などの定義

	// enumで番号をふっている
	enum SOUND_BGM  // BGMの種類
	{
		BGM_NONE = -1,		// BGMを消す
		BGM_TITLE = 0,		// タイトル用のBGM
		BGM_SELECT = 1,		// 選択シーン
		BGM_GAME1 = 2,		// ゲーム用のBGM
		BGM_GAME2 = 3,		// ゲーム用のBGM
		BGM_GAME3 = 4,		// ゲーム用のBGM
		BGM_RESULT = 5,     // リザルト用のBGM


		// 増やしても良い
	};

	enum SOUND_SE  // SEの種類
	{
		SE_DECIDE = 0,		   // 決定音

		SE_SELECT = 1,		   // 選択音

		SE_KURAE = 2,          // くらえ！

		SE_DAMEIGI = 3,        // 攻撃

		SE_DAMEIGIEN = 4,      // 攻撃

		SE_BOUGYO = 5,         // 防御

		SE_DAMEZERO = 6,       // 効かないぞ

		SE_DAMEARI = 7,        // ぐっ！

		SE_KUUU = 8,           // くぅぅ

		SE_UP = 9,             // バフ

		SE_DOWN = 10,          // デバフ

		SE_MP_UP = 11,         // MPアップ

		SE_DOKU = 12,          // 毒

		SE_BOUGYO_UP = 13,     // 防御アップ

		SE_DOROU = 14,         // ドロー

		SE_SELECT2 = 15,       // 選択してください

		SE_HITOTU = 16,        // ひとつ

		SE_LOSS = 17,          // 敗北

		SE_WIN = 18,           // 勝利

		SE_MOSUTAADAME = 19,   // ヒュッ 

		SE_GUAAAAA = 20,	   // グワァァァ

        SE_SEIREI = 21,        // 精霊

		SE_OUGI = 22,          // 奥義

		SE_SAAIKUZO = 23,      // さあ、いくぞ！

		SE_PETA = 24,          // ペタ 

		SE_KIRU = 25,          // キル

		SE_HEAL = 26,          // 回復

		SE_BIRIBIRI = 27,      // ビリビリ

		SE_HENNDOU = 28,       // 変動

		SE_NOROI = 29,         // 呪い

		// 増やしても良い
	};

	// 遅延再生用の構造体
	struct DelayedSE
	{
		SOUND_SE se;     // 再生する効果音のID
		int timer;  // 何フレーム後に再生するか
	};

public:  // 公開

	SoundManager(); // コンストラクタ

	~SoundManager(); // デストラクタ

	// 初期化
	void Initialize();

	// 毎フレーム呼ぶ更新処理
	// 遅延しているSEをカウントして、時間が来たら再生する
	void Update();

	// 終了処理
	void Finalize();


public:   // サウンド再生系  こっちは関数


	// BGM再生
	// SOUND_BGM... 鳴らしたいBGMの種類
	// voli\ume... 大きさ　追加した
	// isTop... 最初から再生するかどうか
	// bool isTop = true -> デフォルト引数と呼ばれる書き方。館数を呼びだす際に渡さなくても認識される。
	void PlayBGM(SOUND_BGM bgm, bool isTop = true, int volume = 255);

	// 音量変えるやつ	BGM
	void ChangeVolume(int volume);
	
	// ループするSE
	void PlaySE_Loop(SOUND_SE se, int volume);
	// 音量変えるやつ	SE
	void Change_SE_Volume(SOUND_SE se, int volume);
	void StopSE(SOUND_SE se); // 止める SE SEというかながめのやつ

	// SE再生
	// SEは短い音なので最初から再生のは入れなくて良い
	// 大きさ追した 0306 マスターまでに間に合うとよいがな
	void PlaySE(SOUND_SE se, int volume = 255);

	// BGMの読み込み
	void LoadBGM(SOUND_BGM bgm, std::string filename);

	// SEの読み込み
	void LoadSE(SOUND_SE se, std::string filename);

	// BGMの停止
	void StopBGM();

	// 遅延してSEを再生する
	// se    : 鳴らしたいSEの種類
	// delay : 何フレーム後に鳴らすか（60FPSなら60で1秒）
	void PlaySEDelay(SOUND_SE se, int delay);

	// 曲のフェードアウト開始
	void StartFadeOut();

	// BGMの音量変えるやつ
	void SetBGMVolume(int volume);

	// SEの音量変えるやつ
	void SetSEVolume(int volume);

	// ボリュームのセーブ
	void SaveVolume();

	// ボリュームのロード
	void LoadVolume();

	int GetBGMVolume();

	int GetSEVolume();

private:  // メンバ変数

	SOUND_BGM mnNowPlayingBgm;  // 現在再生されているBGMの種類
	SOUND_SE mnNowPlayingSe;  // 現在再生されているSEの種類


	// pair... 構造体みたいな要素？ 二ついれられるやつ
	// 
	// よみこんだBGMハンドルのリスト
	std::vector<std::pair<SOUND_BGM, int>> mnBgmHandleList;
	// よみこんだSEハンドルのリスト
	std::vector<std::pair<SOUND_SE, int>> mnSeHandleList;

	// 遅延再生のリスト
	// 「どのSEを・あと何フレームで鳴らすか」を管理する
	std::vector<DelayedSE> seQueue;

	bool mbFadeOut = false;// フェードアウト中かどうかのフラグ
	int mnFadeVolume = 255;

	int mnBgmVolume = 255;
	int mnSeVolume = 255;
};



