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
	// 音楽の音量を設定できるものs
	//int SHandle;
	//SHandle = LoadSoundMem("Resource/BGM/3D/maou_bgm_cyber01.mp3");
	//PlaySoundMem(SHandle, DX_PLAYTYPE_BACK);
	//// 音量の設定
	//ChangeVolumeSoundMem(255 * 80 / 100, SHandle);

	// BGM の読み込み  	  菊池
	//LoadBGM(SOUND_BGM::BGM_TITLE, "Resource/BGM/Happy_Street.mp3");		// タイトル用のBGM
	LoadBGM(SOUND_BGM::BGM_TITLE, "Resource/BGM/TitleBGM.mp3");		   // タイトル用のBGM  碧いなんたらの夜明け
	LoadBGM(SOUND_BGM::BGM_SELECT, "Resource/BGM/Select.mp3");		   // 選択中
	LoadBGM(SOUND_BGM::BGM_GAME1, "Resource/BGM/Rumbling.mp3");		   // ゲーム用のBGM
	LoadBGM(SOUND_BGM::BGM_GAME2, "Resource/BGM/Planetarium.mp3");	   // ゲーム用のBGM
	LoadBGM(SOUND_BGM::BGM_GAME3, "Resource/BGM/Awaking_City.mp3");	   // ゲーム用のBGM
	LoadBGM(SOUND_BGM::BGM_RESULT, "Resource/BGM/Select.mp3");		   // リザルト用のBGM



	// SE の読み込み	  菊池
	LoadSE(SOUND_SE::SE_DECIDE, "Resource/SE/Enter.mp3");			// 決定音   ピコっ

	LoadSE(SOUND_SE::SE_SELECT, "Resource/SE/Card.mp3");			    // 選択音   シュ

	LoadSE(SOUND_SE::SE_KURAE, "Resource/SE/Attack1.mp3");				    // 攻撃するとき  

	LoadSE(SOUND_SE::SE_DAMEIGI, "Resource/SE/BOM.mp3");				    // 攻撃

	LoadSE(SOUND_SE::SE_DAMEIGIEN, "Resource/SE/SE1.mp3");				    // 攻撃

	LoadSE(SOUND_SE::SE_BOUGYO, "Resource/SE/Shild.mp3");				    // 防御

	LoadSE(SOUND_SE::SE_DAMEZERO, "Resource/SE/Nodamage.mp3");		    // 防御

	LoadSE(SOUND_SE::SE_DAMEARI, "Resource/SE/DAMAGE2.mp3");					// 選択音

	LoadSE(SOUND_SE::SE_KUUU, "Resource/SE/se_buwawa.mp3");						// 	くぅぅ

	LoadSE(SOUND_SE::SE_UP, "Resource/SE/StatusUP.mp3");				// バフ

	LoadSE(SOUND_SE::SE_DOWN, "Resource/SE/se_down.mp3");						// デバフ

	LoadSE(SOUND_SE::SE_MP_UP, "Resource/SE/heal_02_long.wav");				    // MPアップ

	LoadSE(SOUND_SE::SE_DOKU, "Resource/SE/DL.mp3");				    // 毒

	LoadSE(SOUND_SE::SE_BOUGYO_UP, "Resource/SE/Bougyo_UP.mp3");			    // 防御アップ

	LoadSE(SOUND_SE::SE_DOROU, "Resource/SE/spo_ge_toranpu_hiku03.mp3");        // ドロー

	LoadSE(SOUND_SE::SE_SELECT2, "Resource/SE/Select1.mp3");       // 選択してください

	LoadSE(SOUND_SE::SE_HITOTU, "Resource/SE/OWN.mp3");                  // ひとつ

	LoadSE(SOUND_SE::SE_LOSS, "Resource/SE/DAMAGE3.mp3");	// 敗北

	LoadSE(SOUND_SE::SE_WIN, "Resource/SE/WIN1.mp3");				    // 勝利

	LoadSE(SOUND_SE::SE_MOSUTAADAME, "Resource/SE/YU1.mp3");

	LoadSE(SOUND_SE::SE_GUAAAAA, "Resource/SE/DAMAGE1.mp3");

	LoadSE(SOUND_SE::SE_SEIREI, "Resource/SE/fairy.mp3");

	LoadSE(SOUND_SE::SE_OUGI, "Resource/SE/Special.mp3");

	LoadSE(SOUND_SE::SE_SAAIKUZO, "Resource/SE/GO.mp3");

	LoadSE(SOUND_SE::SE_PETA, "Resource/SE/PE.mp3");

	LoadSE(SOUND_SE::SE_KIRU, "Resource/SE/SE3.mp3");

	LoadSE(SOUND_SE::SE_HEAL, "Resource/SE/HEAL.mp3");

	LoadSE(SOUND_SE::SE_BIRIBIRI, "Resource/SE/BIRI.mp3");

	LoadSE(SOUND_SE::SE_HENNDOU, "Resource/SE/NEWS.mp3");

	LoadSE(SOUND_SE::SE_NOROI, "Resource/SE/Gravite.mp3");

	LoadSE(SOUND_SE::SE_NOROI, "Resource/SE/Gravite.mp3");

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
	if (mnNowPlayingBgm == BGM_NONE) return;

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
	 mnNowPlayingBgm = BGM_NONE;
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


