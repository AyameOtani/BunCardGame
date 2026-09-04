#include "SoundManager.h"
#include "DxLib.h"
#include "Master.h"
#include <fstream>

SoundManager::SoundManager()
	: mnNowPlayingBgm((SOUND_BGM)-1)  // 初期は何も再生されていない状態
	, mnNowPlayingSe((SOUND_SE)-1)    // 初期は何も再生されていない状態
	, mnBgmVolume(100)
	, mnSeVolume(100)
{


}

SoundManager::~SoundManager()
{

}

void SoundManager::Initialize()
{
	// BGM の読み込み  	  菊池
	LoadBGM(SOUND_BGM::BgmTitle, "Resource/BGM/bgm_title.mp3");	  // タイトル用のBGM
	LoadBGM(SOUND_BGM::BgmEasyGame, "Resource/BGM/bgm_easyGame.mp3");		  // ゲーム用のBGM
	LoadBGM(SOUND_BGM::BgmNormalGame, "Resource/BGM/bgm_normalGame.mp3"); // ゲーム用のBGM
	LoadBGM(SOUND_BGM::BgmHardGame, "Resource/BGM/bgm_hardgame.mp3");	  // ゲーム用のBGM



	// SE の読み込み	  菊池
	LoadSE(SOUND_SE::SE_DECIDE, "Resource/SE/se_decide.mp3"); // 決定音
	LoadSE(SOUND_SE::SE_SELECT, "Resource/SE/se_flip_card.mp3"); // カードをめくった時の音
	LoadSE(SOUND_SE::SE_KURAE, "Resource/SE/se_voice_attack.mp3"); // 攻撃するときのボイス 
	LoadSE(SOUND_SE::SE_DAMEIGI, "Resource/SE/se_attack.mp3"); // プレイヤーの攻撃
	LoadSE(SOUND_SE::SE_DAMEIGIEN, "Resource/SE/se_enemy_attack.mp3"); // 敵の攻撃
	LoadSE(SOUND_SE::SE_BOUGYO, "Resource/SE/se_shild.mp3"); // シールドを貼った時
	LoadSE(SOUND_SE::SE_DAMEZERO, "Resource/SE/se_voice_no_damage.mp3"); // 防御の時の声
	LoadSE(SOUND_SE::SE_DAMEARI, "Resource/SE/se_voice_damage.mp3"); // ダメージを受けた時のボイス
	LoadSE(SOUND_SE::SE_KUUU, "Resource/SE/se_damage.mp3");	// ダメージ音
	LoadSE(SOUND_SE::SE_UP, "Resource/SE/se_buff.mp3"); // バフ
	LoadSE(SOUND_SE::SE_DOWN, "Resource/SE/se_debuff.mp3"); // デバフ
	LoadSE(SOUND_SE::SE_MP_UP, "Resource/SE/se_mp_heal.mp3"); // MP回復
	LoadSE(SOUND_SE::SE_DOKU, "Resource/SE/se_poison.mp3"); // 毒
	LoadSE(SOUND_SE::SE_BOUGYO_UP, "Resource/SE/se_defense_up.mp3"); // 防御アップ
	LoadSE(SOUND_SE::SE_DOROU, "Resource/SE/se_draw_card.mp3"); // カードを引く
	LoadSE(SOUND_SE::SE_SELECT2, "Resource/SE/se_voice_card_select.mp3"); // カード選択を促すボイス
	LoadSE(SOUND_SE::SE_HITOTU, "Resource/SE/se_voice_one.mp3"); // ひとつという掛け声

	LoadSE(SOUND_SE::SE_LOSS, "Resource/SE/se_voice_lose.mp3");	// 敗北ボイス
	LoadSE(SOUND_SE::SE_WIN, "Resource/SE/se_voice_win.mp3"); // 勝利ボイス
	LoadSE(SOUND_SE::SE_GUAAAAA, "Resource/SE/se_voice_death.mp3"); // 死亡ボイス

	LoadSE(SOUND_SE::SE_SEIREI, "Resource/SE/se_voice_special_2.mp3"); // 必殺発動時のボイス2
	LoadSE(SOUND_SE::SE_OUGI, "Resource/SE/se_voice_special_1.mp3"); // 必殺発動時のボイス1
	LoadSE(SOUND_SE::SE_SAAIKUZO, "Resource/SE/se_voice_special_3.mp3"); // 必殺発動時のボイス3


	LoadSE(SOUND_SE::SE_PETA, "Resource/SE/se_item_glue.mp3"); // のりアイテムの使用時
	LoadSE(SOUND_SE::SE_KIRU, "Resource/SE/se_item_scissors.mp3"); // はさみアイテムの使用時
	LoadSE(SOUND_SE::SE_HEAL, "Resource/SE/se_heal.mp3"); // ペンで回復したとき

	LoadSE(SOUND_SE::SE_BIRIBIRI, "Resource/SE/se_stun.mp3"); // スタン時
	LoadSE(SOUND_SE::SE_HENNDOU, "Resource/SE/se_cost_change.mp3"); // 手札のコストが変わったとき
	LoadSE(SOUND_SE::SE_NOROI, "Resource/SE/se_curse.mp3"); // 呪い付与時

	LoadVolume();
}

// 毎フレーム呼ばれる更新処理
void SoundManager::Update()
{
	// 登録されている遅延SEを全部チェック
	for (auto& s : seQueue)
	{
		s.timer--; // カウントダウン

		// タイマーが0以下になったら再生
		if (s.timer <= 0)
		{
			PlaySE((SOUND_SE)s.se); // SEを再生
		}
	}

	// 再生し終わったものを削除する
	seQueue.erase(
		std::remove_if(seQueue.begin(), seQueue.end(),
			[](DelayedSE& s)
			{
				return s.timer <= 0; // タイマー終わったやつ消す
			}),
		seQueue.end()
	);

	if (mbFadeOut)
	{
		mnFadeVolume -= 1;

		if (mnFadeVolume < 0)
			mnFadeVolume = 0;

		ChangeVolume(mnFadeVolume);

		if (mnFadeVolume == 0)
		{
			StopBGM();
			mbFadeOut = false;
		}
	}
}

void SoundManager::Finalize()
{
	// BGMの破棄
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		DeleteSoundMem(it->second);
	}

	// SEの破棄
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		DeleteSoundMem(it->second);
	}
}


void SoundManager::PlayBGM(SOUND_BGM bgm, bool isTop, int volume)
{
	mnFadeVolume = mnBgmVolume;
	mbFadeOut = false;

	// 現在再生されているBGMと同じかつ
	// 最初からの再生ではないのであれば return する
	if (mnNowPlayingBgm == bgm && !isTop)
	{
		return;
	}

	//  現在再生中のBGMを止める
	// 今流れているBGMを探して停メル
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == mnNowPlayingBgm)
		{
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second);
			}
			break;
		}
	}

	// 新しいBGMのハンドルを取得して再生
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == bgm)
		{
			// 再生する前に音量を設定する
			ChangeVolumeSoundMem(volume * mnBgmVolume / 255, it->second);
			//ChangeVolumeSoundMem(mnBgmVolume * 255 / 100, it->second);
			// BGMをループ再生
			PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, TRUE); // ループ再生
			// 現在の再生種類を更新
			mnNowPlayingBgm = bgm;
		}
	}
}


// 音量変えたい
void SoundManager::ChangeVolume(int volume)
{
	// 何も再生していないなら何もしない
	if (mnNowPlayingBgm == BgmNone) return;

	// すでに再生中のBGMがあるか確認
	int currentHandle = -1;
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == mnNowPlayingBgm)
		{
			currentHandle = it->second;
			break;
		}
	}

	// 見つかったら音量を変更
	if (currentHandle != -1)
	{
		ChangeVolumeSoundMem(volume, currentHandle);
	}
}

// 音量変えたいSE追加した
void SoundManager::Change_SE_Volume(SOUND_SE se, int volume)
{
	// 指定されたSE探し
	int targetHandle = -1;
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		if (it->first == se)
		{
			targetHandle = it->second;
			break;
		}
	}

	// 見つかったらその SE の音量だけを変更
	if (targetHandle != -1)
	{
		ChangeVolumeSoundMem(volume, targetHandle);
	}
}


// 指定したSEを個別に止める関数
void SoundManager::StopSE(SOUND_SE se)
{
	// SEのリストから 止めたいSEを探す
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		if (it->first == se)
		{
			// 再生中かどうか確認して止める
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second);
			}
			break;
		}
	}
}


void SoundManager::PlaySE(SOUND_SE se, int volume)
{
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		// 一致した種類のSEがあれば
		if (it->first == se)
		{
			// 再生前に音量を設定する
			ChangeVolumeSoundMem(mnSeVolume * 255 / 100, it->second);
			// SEをループ再生
			PlaySoundMem(it->second, DX_PLAYTYPE_BACK); // バックグラウンド再生する
			// 現在の再生種類を更新
			mnNowPlayingSe = se;
		}
	}
}

void SoundManager::LoadBGM(SOUND_BGM bgm, std::string filename)
{
	bool check = false; // 重複して読み込んでいるかどうか
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == bgm)
		{
			check = true;
			break;
		}
	}

	// 重複して読み込まれていたら何もしない
	if (check)
	{
		return;
	}

	// ファイル読み込み
	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1) // -1だと失敗
	{
		return;   // 読み込み失敗したら何もしない
	}

	// 読み込んだハンドルをリストに追加
	// 読み込ん札ハンドルと、タイプ（種類）を一緒に追加
	// std::pair の一個めの変数（種類）がfirst とされている
	// pair の一つ目の要素を表していた
	// strust
	// {
	//  SOUND_BGM bgm;
	//  int handle;
	// }
	// が、std::pair<SOUND_BGM, int>

	mnBgmHandleList.push_back(std::pair<SOUND_BGM, int>(bgm, handle));
}


void SoundManager::LoadSE(SOUND_SE se, std::string filename)
{
	bool check = false; // 重複して読み込んでいるかどうか
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		if (it->first == se)
		{
			check = true;
			break;
		}
	}

	// 重複して読み込まれていたら何もしない
	if (check)
	{
		return;
	}

	// ファイル読み込み
	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1) // -1だと失敗
	{
		return;   // 読み込み失敗したら何もしない
	}

	mnSeHandleList.push_back(std::pair<SOUND_SE, int>(se, handle));
}


void SoundManager::StopBGM()
{
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		// 今現在再生しているBGMと比較させる
		if (it->first == mnNowPlayingBgm)
		{
			// BGMが再生されているかのif文
			//          もし再生されていたら
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second); // BGMの停止
				break;
			}
		}
	}

	// チュートリアルから戻ったときにBGMが再生されないから
	 mnNowPlayingBgm = BgmNone;
}

void SoundManager::PlaySE_Loop(SOUND_SE se, int volume)
{
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		if (it->first == se)
		{
			// すでに鳴っていたら二重に鳴らさない
			if (CheckSoundMem(it->second))
			{
				return;
			}

			// ボリューム
			ChangeVolumeSoundMem(volume, it->second);
			PlaySoundMem(it->second, DX_PLAYTYPE_LOOP);
			break;
		}
	}
}

// 菊池
// 遅延してSEを再生する
void SoundManager::PlaySEDelay(SOUND_SE se, int delay)
{
	// 「このSEを〇フレーム後に鳴らす」という予約を追加
	seQueue.push_back({ se, delay });
}


void SoundManager::StartFadeOut()
{
	if (mbFadeOut) return;

	mbFadeOut = true;
}

// BGMの音量変えるやつ
void SoundManager::SetBGMVolume(int volume)
{
	mnBgmVolume = volume;

	for (auto& bgm : mnBgmHandleList)
	{
		if (bgm.first == mnNowPlayingBgm)
		{
			ChangeVolumeSoundMem(
				mnBgmVolume,
				bgm.second
			);
		}
	}
}

// SEの音量を変えるやつ
void SoundManager::SetSEVolume(int volume)
{
	mnSeVolume = volume;
}

// ボリュームのロード
void SoundManager::SaveVolume()
{
	std::ofstream ofs("Volume.txt");

	if (ofs)
	{
		ofs << "BGM=" << mnBgmVolume << std::endl;
		ofs << "SE=" << mnSeVolume << std::endl;
	}
}


void SoundManager::LoadVolume()
{
	// デフォルト値
	mnBgmVolume = 100;
	mnSeVolume = 100;

	std::ifstream ifs("Volume.txt");

	// ファイルが存在しない場合
	if (!ifs)
	{
		SaveVolume();
		return;
	}

	std::string line;

	// BGM
	if (std::getline(ifs, line))
	{
		size_t pos = line.find('=');

		if (pos != std::string::npos)
		{
			try
			{
				int volume = std::stoi(line.substr(pos + 1));

				// 0から100なら採用
				if (volume >= 0 && volume <= 100)
				{
					mnBgmVolume = volume;
				}
			}
			catch (...)
			{
				// 数値として読めなかったらデフォルト100のまま
			}
		}
	}

	// SE
	if (std::getline(ifs, line))
	{
		size_t pos = line.find('=');

		if (pos != std::string::npos)
		{
			try
			{
				int volume = std::stoi(line.substr(pos + 1));

				// 0～100なら採用
				if (volume >= 0 && volume <= 100)
				{
					mnSeVolume = volume;
				}
			}
			catch (...)
			{
				// 数値として読めなかったらデフォルト100のまま
			}
		}
	}
}



// ボリュームのセーブ
int SoundManager::GetBGMVolume()
{
	return mnBgmVolume;
}

// SEのセーブ
int SoundManager::GetSEVolume()
{
	return mnSeVolume;
}


