#include "GameScene.h"
#include "Master.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"
#include <memory>
#include "Utility.h"
#include "Player.h"
#include "Enemy.h"
#include "EffectManager.h"
#include "Mouse.h"

GameScene::GameScene(SceneManager::GScene gscene)
	: Scene()
	, mGScene(gscene)
	, ChargeCount(0)       // 最初は0
	, ChargeAttack(false)  // 最初はfalse
	, Action(0)            // 最初は0
{

	// 文字の大きさ関係
	mnFontBig = CreateFontToHandle(
		NULL,
		60, // すごく大きく
		3
	);

	mnFontNormal = CreateFontToHandle(
		NULL,
		45,   // 今のフォントサイズ
		3
	);

	mnFontSmall = CreateFontToHandle(
		NULL,
		35,   // 小さめのフォントサイズ
		3
	);


	// MPの画像の読み込み
	mnHandleMP = LoadGraph("Resource/2D/mp_bag.png");
	if (mnHandleMP == -1)
	{
		printfDx("MP画像ない");
	}

	// MP足りない時の画像
	mnHandleIsMpError = LoadGraph("Resource/2D/is_Mp.png");
	if (mnHandleIsMpError == -1)
	{
		printfDx("エラー画像の読み込み");
	}

	// 山札の画像
	mnHandleDeck = LoadGraph("Resource/2D/deck.png");
	if (mnHandleDeck == -1)
	{
		printfDx("山札画像の読み込み");
	}

	// ゴミ箱の画像
	mnHandleTrash = LoadGraph("Resource/2D/trash.png");
	if (mnHandleTrash == -1)
	{
		printfDx("山札画像の読み込み");
	}

	// 説明画面の画像
	if (mnItemEHandle == -1)
	{
		mnItemEHandle = LoadGraph("Resource/Mouse/Ce.png");
	}

	// ターンの背景の画像
	if (mnHandleTurnBag == -1)
	{
		mnHandleTurnBag = LoadGraph("Resource/2D/turn_Bag.png");
	}

	// ノートの画像
	if (mnNoteHandle == -1)
	{
		mnNoteHandle = LoadGraph("Resource/Title/Note.png");
	}

	// ノートの画像みに
	mnNoteMini = LoadGraph("Resource/Title/manual_note.png");

	if (mnNoteMini == -1)
	{
		printfDx("Note3.png 読み込み失敗\n");
	}

}

GameScene::~GameScene()
{

}


void GameScene::Initialize() // 初期化 
{
	// 変数の初期化
	Master::mpTurnCount = 1; // ターン数の初期化 初期は
	Master::mpSaveCardCount = 0; // カード使用回数の初期化
	Master::mpSaveItemCount = 0; // アイテム使用回数の初期化


	// 位置設定
	float dist = 650.0f; // 幅からずらす距離
	float posY = 590.0f; // 高さ

	// プレイヤーの初期化 ダミーにした
	mpPlayer = new Player(VGet(dist, posY, 0.0f), "Resource/Player/Dummy.png", mGScene);
	
	mpPlayer->Initialize(); // 初期化呼び出さ
	mpPlayer->StartBattle(); // 手札を配る

	//シーンごとに敵を分岐
	switch (mGScene)
	{
	case SceneManager::GScene::easy://簡単なら
		//画像だけ変える
		// 敵の初期化
		mpEnemy = new Enemy(VGet(Utility::SCREEN_WIDTH - dist, posY, 0.0f), "Resource/Player/Dummy.png", Enemy::EnemyType::TypeA);
		// 画像読み込み用 背景
		mnBagHandle = LoadGraph("Resource/2D/easy_bag.png");
		mpEnemy->Initialize("Resource/Enemy/Enemy1.png"); // 初期化
		break;


	case SceneManager::GScene::normal://普通なら
		// 敵の初期化
		mpEnemy = new Enemy(VGet(Utility::SCREEN_WIDTH - dist, posY, 0.0f), "Resource/Player/Dummy.png", Enemy::EnemyType::TypeB);
		// 画像読み込み用 背景
		mnBagHandle = LoadGraph("Resource/2D/Normal_bag.png");
		mpEnemy->Initialize("Resource/Enemy/Enemy2.png"); // 初期化
		break;


	case SceneManager::GScene::hard://難しいなら
		// 敵の初期化
		mpEnemy = new Enemy(VGet(Utility::SCREEN_WIDTH - dist, posY, 0.0f), "Resource/Player/Dummy.png", Enemy::EnemyType::TypeC);
		// 画像読み込み用 背景
		mnBagHandle = LoadGraph("Resource/2D/hard_bag.png");
		mpEnemy->Initialize("Resource/Enemy/Enemy3.png"); // 初期化
		break;


	default:
		// 敵の初期化
		mpEnemy = new Enemy(VGet(Utility::SCREEN_WIDTH - dist, posY, 0.0f), "Resource/Enemy/Enemy.png", Enemy::EnemyType::TypeB);
		// 画像読み込み用 背景
		mnBagHandle = LoadGraph("Resource/2D/Normal_bag.png");
		mpEnemy->Initialize("Resource/Enemy/Enemy2.png"); // 初期化
		break;
	}

	// 背景エラー用
	if (mnBagHandle == -1)
	{
		printfDx("画像の読み込みに失敗");
	}


	if (mpEnemy != nullptr)
	{
		mpEnemy->EnemyStartBattle(); // 敵の手札の生成
		mpEnemy->CardSet(); // 敵のカードの位置
	}
	else
	{
		printfDx("mpEnemyが作成されていません\n");
	}


	//// カード描画用スクリーンの設定  pickOptionこれでとる
	//int w, h;
	//if (!mpPlayer->pickOption.empty() && mpPlayer->hand[0] != nullptr)
	//{
	//	mpPlayer->pickOption[0]->GetFrameSize(&w, &h);
	//}
	//else
	//{
	//	// 万が一手札がない場合のデフォルトサイズ
	//	w = 200; h = 300;
	//}


	// カード描画用スクリーンの設定
	int w = 256; // デフォルト値を先に設定しておく
	int h = 384;

	// mpPlayerが存在 かつ pickOptionに中身があるか
	if (mpPlayer != nullptr && !mpPlayer->pickOption.empty())
	{
		if (mpPlayer->pickOption[0] != nullptr)
		{
			mpPlayer->pickOption[0]->GetFrameSize(&w, &h);
		}
	}
	// pickOptionがダメなら handを試す
	else if (mpPlayer != nullptr && !mpPlayer->hand.empty())
	{
		if (mpPlayer->hand[0] != nullptr)
		{
			mpPlayer->hand[0]->GetFrameSize(&w, &h);
		}
	}

	mnCardWorkScreen = MakeScreen(w, h, TRUE);
	mnCardFontHandle = CreateFontToHandle(NULL, (int)(h * 0.061f), 3, DX_FONTTYPE_NORMAL);


	// ボタン画像の生成
	mpTurnEnd = new MouseGraph(1580.0f, 900.0f, 0.0f,"Resource/2D/tuenEnd.png", 0.25f, 0.28f);
	mpStop = new MouseGraph(100.0f, 100.0f, 0.0f,"Resource/2D/stop_button.png", 0.1f, 0.13f);

	int x = Utility::SCREEN_WIDTH / 2;
	int y = Utility::SCREEN_HEIGHT / 2;
	// ポーズ画面のやつ
	mpGame = new MouseGraph((float)x, (float)y - 200.0f, 0.0f, "Resource/Title/ReturnGame.png", 0.5f, 0.55f); // ゲームに戻る
	mpExplain = new MouseGraph((float)x, (float)y, 0.0f, "Resource/Title/Explain.png", 0.5f, 0.55f); // 説明を出す
	mpTitle = new MouseGraph((float)x, (float)y + 200.0f, 0.0f, "Resource/Title/Result.png", 0.5f, 0.55f); // タイトル画面



	// 音量調整のやつ
	mpHaguruma = new MouseGraph((float)x - 330.0f, (float)y - 210.0f, 0.0f, "Resource/Title/option.png", 0.21f, 0.24f); // 音量調整のやつ
	// 音量設定背景
	mnMusicBag = LoadGraph("Resource/Title/Titleboard.png");

	if (mnMusicBag == -1)
	{
		printfDx("黒板画像読み込み失敗");
	}

	// 音符画像
	mnOnpuHandle = LoadGraph("Resource/Title/MusicNote.png");

	if (mnOnpuHandle == -1)
	{
		printfDx("音符画像読み込み失敗");
	}

	// ×ボタン
	mpMusicClose = new MouseGraph(
		1570.0f,
		370.0f,
		0.0f,
		"Resource/Title/MusicClose.png",
		0.30f,
		0.35f
	);

	// 現在の音量取得
	mBgmVolume = Master::mpSoundManager->GetBGMVolume();
	mSeVolume = Master::mpSoundManager->GetSEVolume();



	//閉じるボタン
	mpClose = new MouseGraph((float)x + 600.0f, 980.0f, 0.0f, "Resource/Title/Close.png", 0.3f, 0.35f); // タイトル画面


	Master::mpSoundManager->StopBGM();
	if (mGScene == SceneManager::GScene::easy)
	{
		Master::mpSoundManager->PlayBGM(SoundManager::BGM_GAME1);
	}
	else if (mGScene == SceneManager::GScene::normal)
	{
		Master::mpSoundManager->PlayBGM(SoundManager::BGM_GAME2);
	}
	else if (mGScene == SceneManager::GScene::hard)
	{
		Master::mpSoundManager->PlayBGM(SoundManager::BGM_GAME3);
	}

	// 敵カードの裏の画像の読み込み 追加　小池　5/15
	mnEnemyCardBack = LoadGraph("Resource/Enemy/Card/Enemy01.png");



	mbInitialize = true; // 初期化終了フラグ
}

void GameScene::ShowItemMessage(const std::string& text, const int& color)
{
	mItemMessage = text;

	mnColor = color; // 色も追加

	// 右外から開始
	mfItemNoteX = 1920.0f;

	// 演出ON
	mbItemSlide = true;

	// 表示時間
	mItemMessageTimer = 360;
}

void GameScene::Update() // 更新
{
	// ゲームの初期化が終わっていたら
	if (mbInitialize)
	{
		static float velocity = 0.0f; // 今の速度
		float targetY = 60.0f; // 目的地Y

		// 調整パラメータ
		float gravity = 0.3f;     // 落ちる速さ 重力
		float power = 0.20f; // バネの強さ
		float damping = 0.85f;     // 減衰

		// 重力で下に引っ張る
		velocity += gravity;

		// バネ
		float force = (targetY - mfTurnY) * power;
		velocity += force;

		// 減衰
		velocity *= damping;

		// 移動させてる
		mfTurnY += velocity;
		mfTurnBox += velocity; // 後ろの画像も一緒に

		// ほぼ目的地についたら止める
		if (fabs(velocity) < 0.2f && fabs(targetY - mfTurnY) < 0.2f)
		{
			mfTurnY = targetY;
			mfTurnBox = targetY; // 後ろの画像も一緒に
			velocity = 0.0f;
		}
	}



	if (mpStop)
	{
		mpStop->Update();

		// クリックされた瞬間だけ実行する
		if (mpStop->IsClicked() && !mbOption)
		{
			mbStop = !mbStop; // ここで反転
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE); // 音追加

			mpStop->SetActive(false); // ボタンを無効にする
		}

	}

	// 停止されていないかつ音量設定中じゃないなら
	if (!mbStop && !mbOption)
	{
		mpStop->SetActive(true); // ボタンを有効
		// エフェクトの生存チェック
		if (mpPlayer)
		{
			mpPlayer->UnitUpdate();
		}
		if (mpEnemy)
		{
			mpEnemy->Update(); // てきのHPのUPD
			mpEnemy->UnitUpdate();
		}

		// １ターン目だったらメモを出す
		if (Master::mpTurnCount == 1 &&
			mItemMessage.empty() && !IsMessage && !mpPlayer->GetIsPicking() && !mpPlayer->GetIsSpecialPicking())
		{
			ShowItemMessage(
				"発動させたいカードを\n\nドラッグアンドドロップ!",
				GetColor(80, 80, 80)
			);

			IsMessage = true;
		}
		else
		{
			//IsMessage = false;
		}


		//// デバック用
		//if (InputManager::CheckDownKey(KEY_INPUT_1))
		//{
		//	Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::TITLE);
		//}
		//if (InputManager::CheckDownKey(KEY_INPUT_2))
		//{
		//	Master::mpSaveHp = mpPlayer->hp; // プレイヤーのHPを保存しておく
		//	Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::LOSERESULT);

		//}
		//if (InputManager::CheckDownKey(KEY_INPUT_3))
		//{
		//	Master::mpSaveHp = mpPlayer->hp; // プレイヤーのHPを保存しておく
		//	Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::WINRESULT);
		//}


		if (mpTurnEnd)
		{
			mpTurnEnd->Update();
		}

		// アイテム通知演出

		if (!mItemMessage.empty())
		{
			// 入ってくる
			if (mbItemSlide)
			{
				mfItemNoteX -= 35.0f;

				if (mfItemNoteX <= mfItemTargetX)
				{
					mfItemNoteX = mfItemTargetX;
					mbItemSlide = false;
				}
			}

			// 表示時間
			if (mItemMessageTimer > 0)
			{
				mItemMessageTimer--;
			}
			else
			{
				// 右へ戻る
				mfItemNoteX += 35.0f;

				if (mfItemNoteX >= 1920.0f)
				{
					mItemMessage.clear();
				}
			}
		}

		// プレイヤーのターンの時だけ呼ぶ
		if (mpPlayer /* && mTurn == Turn::PLAYER_TURN*/)
		{
			mpPlayer->HpGaugeUpdate();
			for (int i = 0; i < (int)mpPlayer->hand.size(); i++)
			{
				// i番目のカードを取り出して LateUpdate を呼ぶ
				if (mpPlayer->hand[i] != nullptr)
				{
					mpPlayer->hand[i]->LateUpdate();
				}
			}

			for (int i = 0; i < (int)mpPlayer->pickOption.size(); i++)
			{
				// 初手の選択のやつも動かしたい
				if (mpPlayer->pickOption[i] != nullptr)
				{
					mpPlayer->pickOption[i]->LateUpdate();
				}
			}
		}

		if (mpEnemy)
		{
			for (int i = 0; i < (int)mpEnemy->enemyhand.size(); i++)
			{
				if (mpEnemy->enemyhand[i] != nullptr)
				{
					mpEnemy->enemyhand[i]->LateUpdate();
				}
			}
		}

		// ターンで分ける
		switch (mTurn)
		{

			// プレイヤーのターンだったら操作できるように
		case Turn::PLAYER_TURN:

			// カードのドラッグ判定とか
			if (mpPlayer && mpEnemy)
			{
				mpPlayer->Update(*mpEnemy); // 敵を渡す
				mpPlayer->CardUpdate(*mpEnemy);


				// 敵が死んだら即終了判定へ
				if (mpEnemy->hp <= 0)
				{
					mTurn = Turn::RESULT_CHECK;
					//return;
				}
			}

			if (mpPlayer)
			{
				// 初めのカードを選択中またはだったらボタン無効
				if (mpPlayer->GetIsPicking() || mpPlayer->GetIsSpecialPicking())
				{
					mpTurnEnd->SetActive(false); // ボタン無効に
				}
				else
				{
					mpTurnEnd->SetActive(true); // ボタン有効に
				}

			}

			// マウスが重なっていて、押された瞬間   (これいれるかみんなに聞く)カードが移動中じゃないなら
			// カード選択中（初めのやつ）じゃないなら入るように
			if (mpTurnEnd->IsClicked() && !mpPlayer->GetIsPicking() && !mpPlayer->IsCardMove())
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

				// 必殺技ゲージの更新
				mpPlayer->SpecialGaugeUpdate();

				// 敵の毒の処理
				UpdateUnitStatus(*mpEnemy);

				if (mpEnemy->hp <= 0)
				{
					mTurn = Turn::RESULT_CHECK;
					return;
				}

				mTurn = Turn::ENEMY_THINK;

				mWaitTimer = 180;
				mpTurnEnd->SetActive(false); // ボタン無効に
				mpEnemy->block = 0;  // 敵のブロックも0にする



				// ターンエンドでごみ箱に移動さえる演出
				for (int i = 0; i < (int)mpPlayer->hand.size(); i++)
				{
					if (mpPlayer->hand[i] != nullptr)
					{
						// キープされているカードはなにもしない
						if (mpPlayer->hand[i]->isKeep)
						{
							continue; // 次のカードの処理へ
						}

						// 右下に移動
						mpPlayer->hand[i]->SetCardPosition(1800.0f, 1000.0f, 0.0f);
						// 目標サイズを 0 にする
						mpPlayer->hand[i]->SetTargetScale(0.0f);
					}
				}

			}
			break;


		//case Turn::ENEMY_THINK:

		//	mWaitTimer--;
		//	if (mWaitTimer <= 0)
		//	{
		//		//mTurn = Turn::ENEMY_ACTION;
		//		// 攻撃演出のために少し時間を取るならここでまたタイマーをセット
		//		//mWaitTimer = 60;

		//		//行動する前に抽選する
		//		Action = GetRand(4);

		//		// 表示するカード決定 追加　小池
		//		if (!mpEnemy->enemyhand.empty())
		//		{
		//			mpEnemyActionCard =
		//				mpEnemy->enemyhand[Action];
		//		}

		//		// 演出開始
		//		mbEnemyCardShow = true;
		//		mfEnemyCardY = -300.0f;

		//		// カード表示フェーズへ
		//		mTurn = Turn::ENEMY_CARD_SHOW;
		//		mWaitTimer = 60;
		//	}
		//	break;
			
		case Turn::ENEMY_THINK:

			mWaitTimer--;

			if (mWaitTimer <= 0)
			{
				// 手札からランダム1枚選ぶ
				int index = GetRand(
					(int)mpEnemy->enemyhand.size() - 1
				);

				mpEnemyActionCard =
					mpEnemy->enemyhand[index];

				// カードIDから行動決定
				Action = mpEnemyActionCard->GetId() - 1;

				// 演出開始
				mbEnemyCardShow = true;
				mfEnemyCardY = -300.0f;

				//ここからカードの回転するフェイズに入る　追加　小池　5/15
				
				// 回転初期化
				mEnemyCardAngleY = 0.0f;
				// 最初は裏向き
				mbEnemyCardFront = false;
				// 表切替フラグ初期化
				mbEnemyCardSwitched = false;

				// 追加：Card の表示フラグと同期
				if (mpEnemyActionCard) mpEnemyActionCard->SetShowFront(false);

				// 最初は裏→表から開始
				mEnemyCardState = EnemyCardState::FLIP_TO_FRONT;
			
				mTurn = Turn::ENEMY_CARD_SHOW;
				mWaitTimer = 60;
			}
			break;

		case Turn::ENEMY_CARD_SHOW:  // 小池
		{
			// カードを下に落とす
			float speed = 25.0f;

			if (mfEnemyCardY < mfEnemyTargetY)
			{
				mfEnemyCardY += speed;
			}
			// ここから下をカードの回転する処理を追加　小池　5/15
			else
			{
				mfEnemyCardY = mfEnemyTargetY;

				// 裏 → 表
				if (mEnemyCardState ==
					EnemyCardState::FLIP_TO_FRONT)
				{
					mEnemyCardAngleY +=
						mEnemyCardAnimSpeed;

					// 真横で表へ
					if (!mbEnemyCardSwitched &&
						mEnemyCardAngleY >=
						DX_PI_F / 2.0f)
					{
						mbEnemyCardFront = true;
						mbEnemyCardSwitched = true;

						// 追加：Card の表示フラグと同期（表を表示）
						if (mpEnemyActionCard) mpEnemyActionCard->SetShowFront(true);
					}

					// 回転終了
					if (mEnemyCardAngleY >= DX_PI_F)
					{
						mEnemyCardAngleY = 0.0f;

						// 表を2秒表示
						mWaitTimer = 120;

						mEnemyCardState =
							EnemyCardState::SHOW_FRONT;
					}
				}

				// 表を見せる
				else if (mEnemyCardState ==
					EnemyCardState::SHOW_FRONT)
				{
					mWaitTimer--;

					if (mWaitTimer <= 0)
					{
						mEnemyCardAngleY = 0.0f;
						mbEnemyCardSwitched = false;

						// 裏へ戻す
						mEnemyCardState =
							EnemyCardState::FLIP_TO_BACK;
					}
				}

				// 表 → 裏
				else if (mEnemyCardState ==
					EnemyCardState::FLIP_TO_BACK)
				{
					mEnemyCardAngleY +=
						mEnemyCardAnimSpeed;

					// 真横で裏へ
					if (!mbEnemyCardSwitched &&
						mEnemyCardAngleY >=
						DX_PI_F / 2.0f)
					{
						mbEnemyCardFront = false;
						mbEnemyCardSwitched = true;

						// 追加：Card の表示フラグと同期（裏を表示）
						if (mpEnemyActionCard) mpEnemyActionCard->SetShowFront(false);
					}

					// 回転終了
					if (mEnemyCardAngleY >= DX_PI_F)
					{
						mEnemyCardAngleY = 0.0f;

						// 行動へ
						mbEnemyCardShow = false;

						mTurn = Turn::ENEMY_ACTION;
						mWaitTimer = 60;
					}
				}
			}
		}
		break;

		case Turn::ENEMY_ACTION:

			// ここで実際のダメージ処理を一回だけ呼ぶ工夫が必要	  今はタイマーだよ
			if (mWaitTimer == 60)
			{
				// 敵がスタンしているか
				if (mpEnemy->stunTurns > 0)
				{
					// スタン中なら行動をせずにターンを減らすだけ
					mpEnemy->stunTurns--;
				}
				else
				{
					//チャージを1貯める
					ChargeCount += 1;

					//チャージが5なら
					if (ChargeCount == 5)
					{
						ChargeAttack = true; // 代入した

						//trueだったら
						if (ChargeAttack == true)
						{
							// 攻撃  30ダメージ	  必殺と同じエフェクトを出すあためにフラグONや
							Effect effect = { EffectType::DAMAGE, 30, 0, true};
							EffectManager::ApplyEffect(effect, *mpEnemy, *mpPlayer);

							ChargeCount = 0; // カウントリセット
						}
					}
					else	// 必殺と行動を同時にしないようにしている
					{
						// 敵の行動
						switch (Action)
						{
						case 0:
							// 攻撃 10ダメージ
							EffectManager::ApplyEffect({ EffectType::DAMAGE, mpEnemy->GetDamage(), 0}, *mpEnemy, *mpPlayer);
							break;

						case 1:
							//防御　自分に８ブロック
							EffectManager::ApplyEffect({ EffectType::BLOCK, mpEnemy->GetBlock(), 0}, *mpEnemy, *mpEnemy);
							break;
						case 2:
							// バフ（攻撃力アップ） 5アップ　2ターン継続　自分に
							EffectManager::ApplyEffect({ EffectType::APPLY_BUFF, mpEnemy->GetBuff(), 2}, *mpEnemy, *mpEnemy);
							break;
						case 3:
							// 毒  ５ダメージ　２ターン継続
							EffectManager::ApplyEffect({ EffectType::APPLY_POISON, mpEnemy->GetPoison(), 2}, *mpEnemy, *mpPlayer);
							break;
						case 4:
							//弱体化 プレイヤーの被ダメージを５増加 2ターン継続
							EffectManager::ApplyEffect({ EffectType::APPLY_WEAKEN, mpEnemy->GetWeaken(), 2}, *mpEnemy, *mpPlayer);
							break;

						default:
							// 攻撃 10ダメージ
							EffectManager::ApplyEffect({ EffectType::DAMAGE, 10, 0 }, *mpEnemy, *mpPlayer);
							break;
						}
					}
				}
			}


			mWaitTimer--;
			if (mWaitTimer <= 0)
			{

				// プレイヤーの毒ダメージの処理
				UpdateUnitStatus(*mpPlayer);
				// 今の手札を捨てる
				mpPlayer->ReshuffleTurnEnd();
				// プレイヤーのバフリセット
				mpPlayer->buffs.clear();

				// 追加　小池　敵のカードを隠す演出
				mTurn = Turn::ENEMY_CARD_HIDE;

				// 全ての演出が終わったらHPチェックへ
				//mTurn = Turn::RESULT_CHECK;
			}
			break;

			// 追加　小池　敵のカードを隠す演出
			// 画面外に行ったら終了
		case Turn::ENEMY_CARD_HIDE:
		{
			// 上に戻す
			mfEnemyCardY -= 25.0f;

			// 画面外に行ったら終了
			if (mfEnemyCardY <= -300.0f)
			{
				mbEnemyCardShow = false;

				if (mpEnemy)
				{

					auto it = std::find(
						mpEnemy->enemyhand.begin(),
						mpEnemy->enemyhand.end(),
						mpEnemyActionCard
					);

					if (it != mpEnemy->enemyhand.end())
					{
						// 使ったカードを保存
						Card* usedCard = *it;

						// 一旦リストから外す
						mpEnemy->enemyhand.erase(it);

						// 状態リセット
						usedCard->SetScale(0.68f);
						usedCard->SetTargetScale(0.68f);

						// 最後尾へ
						mpEnemy->enemyhand.push_back(usedCard);
					}

					// 位置更新
					mpEnemy->CardSet();
				}

				// 選択カードリセット
				mpEnemyActionCard = nullptr;

				// 結果チェックへ
				mTurn = Turn::RESULT_CHECK;
			}
		}
		break;

		case Turn::RESULT_CHECK:
			mpPlayer->DrawCard(Player::FIRST_HAND_CARD);  // 山札から新しい手札を引く
			mpPlayer->SetMp(mpPlayer->GetMaxMp()); // MPリセット
			mpPlayer->block = 0; // ブロック0にしてる

			//Master::mpTurnCount += 1; // ターン数を増やす


			/// シーンのいどうの処理　
			// NULLチェックもしてからシーン移動している
			if (mpPlayer && mpEnemy)
			{
				// プレイヤーの敗北判定
				if (mpPlayer->hp <= 0)
				{
					// まだ DAMAGE アニメを再生していなければ開始
					if (mpPlayer->animator.GetCurrentAnimType() != AnimationData::AnimType::DEATH)
					{
						Master::mpSoundManager->PlaySE(SoundManager::SE_GUAAAAA);

						mpPlayer->PlayMotion(AnimationData::AnimType::DEATH);
						Master::mpSaveHp = mpPlayer->hp; // プレイヤーのHPを保存しておく
					}

					// アニメーションが最後まで終わったらシーン移動
					if (mpPlayer->animator.IsFinished())
					{
						Master::mpSceneManager->SetNextScene(SceneManager::LOSERESULT);
					}

					Master::mpSoundManager->StartFadeOut();

					return; // アニメ中は下に行かせない
				}


				// プレイヤーの勝利判定
				if (mpEnemy->hp <= 0)
				{
					// まだ DAMAGE アニメを再生していなければ開始
					if (mpEnemy->animator.GetCurrentAnimType() != AnimationData::AnimType::DEATH)
					{
						mpEnemy->PlayMotion(AnimationData::AnimType::DEATH);
						Master::mpSaveHp = mpPlayer->hp; // プレイヤーのHPを保存しておく

					}

					// アニメーションが最後まで終わったらシーン移動
					if (mpEnemy->animator.IsFinished())
					{
						Master::mpSceneManager->SetNextScene(SceneManager::WINRESULT);
					}

					Master::mpSoundManager->StartFadeOut();

					return; // アニメ中はここで止めてる
				}
			}


			// ゲージが100なら必殺技選択開始
			if (mpPlayer->GetSpecialGauge() >= mpPlayer->GetMaxSpecialGauge())
			{
				mpPlayer->StartSpecialPick();  // 呼ぶ
				// ゲージをリセットは選択直後に呼ぶように変更した　大谷
				//mpPlayer->SetSpecialGauge(0);
			}



			mpTurnEnd->SetActive(true); // ボタンを有効に

			/*if (mpPlayer)
			{
				mpPlayer->itemEffectText = "";
			}*/

			Master::mpTurnCount += 1; // ターン数を増やす

			mTurn = Turn::PLAYER_TURN;

			break;
		}

		Scene::Update();
	}
	else
	{

		// ポーズ中の更新
		mpGame->Update();
		mpTitle->Update();
		mpExplain->Update();
		mpHaguruma->Update();

		// 歯車押したら音量設定
		if (mpHaguruma->IsClicked() && !mbOption && !mbSelectON)
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

			mbOption = true;
		}
					

		// 音量設定中じゃないならクリック判定させてる
		if (!mbOption && !mbSelectON)
		{
			// ゲームに戻る
			if (mpGame->IsClicked())
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
				mbStop = false; // 停止中をOFF
			}
			// タイトルに戻る
			if (mpTitle->IsClicked())
			{
				Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::TITLE);
				Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			}
			if (mpExplain->IsClicked())
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
				mbSelectON = true;//選択が選ばれたのをON
				//Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SELECT_SCENE);
			}
		}
		


		if (mbSelectON)
		{
			mpClose->Update();
			if (mpClose->IsClicked())
			{
				mbSelectON = false;
				Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			}
		}

		// 音量設定中
		if (mbOption)
		{
			if (mpMusicClose)
			{
				mpMusicClose->Update();

				if (mpMusicClose->IsClicked())
				{
					Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

					mbOption = false;
				}
			}

			int mouseX, mouseY;

			GetMousePoint(&mouseX, &mouseY);

			if (GetMouseInput() & MOUSE_INPUT_LEFT)
			{
				// BGMバー
				if (mouseX >= mBarX &&
					mouseX <= mBarX + mBarWidth + mnBarDist &&
					mouseY >= mBgmBarY - mnBarUp &&
					mouseY <= mBgmBarY + mnBarDown)
				{
					mBgmVolume =
						(mouseX - mBarX) * 100 / mBarWidth;

					if (mBgmVolume < 0) mBgmVolume = 0;
					if (mBgmVolume > 100) mBgmVolume = 100;

					Master::mpSoundManager->SetBGMVolume(mBgmVolume);
					Master::mpSoundManager->SaveVolume();
				}

				// SEバー
				if (mouseX >= mBarX &&
					mouseX <= mBarX + mBarWidth + mnBarDist &&
					mouseY >= mSeBarY - mnBarUp &&
					mouseY <= mSeBarY + mnBarDown)
				{
					mSeVolume =
						(mouseX - mBarX) * 100 / mBarWidth;

					if (mSeVolume < 0) mSeVolume = 0;
					if (mSeVolume > 100) mSeVolume = 100;

					Master::mpSoundManager->SetSEVolume(mSeVolume);
					Master::mpSoundManager->SaveVolume();
				}
			}
		}
	}


}

void GameScene::Draw() // 描画
{			  
	// 背景の描画
	DrawRotaGraph(Utility::SCREEN_WIDTH / 2, Utility::SCREEN_HEIGHT / 2, 1.0f, 0.0f, mnBagHandle, TRUE);


	Scene::Draw();


	// HPバーはエフェクトで隠れないように  でもカードよりは後ろに出したいからここ
	if (mpPlayer)
	{
		mpPlayer->HpGaugeDraw();
		mpPlayer->IconDraw(); // アイコンの描画
		mpPlayer->SpecialGaugeDraw(); // 必殺技ゲージの描画
	}


	if (mpEnemy)
	{
		mpEnemy->HpGaugeDraw();
		mpEnemy->EnemyIcon(); // アイコンの描画
	}



	// 手札の描画 
	if (mpPlayer)
	{
		mpPlayer->DrawHand(mnCardWorkScreen, mnCardFontHandle);
	}
	//敵の手札の描画
	if (mpEnemy)
	{
		mpEnemy->EnemyCardDraw(mnCardWorkScreen, mnCardFontHandle);

		//if (mbEnemyCardShow && mpEnemyActionCard != nullptr)
		if ((mbEnemyCardShow || mTurn == Turn::ENEMY_CARD_HIDE)
			&& mpEnemyActionCard != nullptr) // カード表示フェーズかカード隠すフェーズで、カードがセットされているなら　変更　小池
		{
			// 一時的に位置変更
			mpEnemyActionCard->SetCardPosition(
				mfEnemyCardX,
				mfEnemyCardY,
				0.0f
			);

			// ここの下にあった処理を回転するための処理に変更　追加　小池　5/15

			mpEnemyActionCard->SetScale(1.0f);

			float targetW = 240.0f;
			float targetH = 340.0f;

			float cosVal =
				cosf(mEnemyCardAngleY);

			float sinVal =
				sinf(mEnemyCardAngleY);

			float halfW =
				(targetW / 2.0f) * cosVal;

			float halfH =
				targetH / 2.0f;

			float pShift =
				sinVal *
				mEnemyCardPerspective;

			// 四隅計算
			float lx = mfEnemyCardX - halfW;
			float rx = mfEnemyCardX + halfW;

			float ty_l =
				mfEnemyCardY - halfH - pShift;

			float ty_r =
				mfEnemyCardY - halfH + pShift;

			float by_r =
				mfEnemyCardY + halfH - pShift;

			float by_l =
				mfEnemyCardY + halfH + pShift;

			int x1, y1, x2, y2;
			int x3, y3, x4, y4;

			// 左右反転防止
			if (cosVal >= 0)
			{
				x1 = (int)lx;
				y1 = (int)ty_l;

				x2 = (int)rx;
				y2 = (int)ty_r;

				x3 = (int)rx;
				y3 = (int)by_r;

				x4 = (int)lx;
				y4 = (int)by_l;
			}
			else
			{
				x1 = (int)rx;
				y1 = (int)ty_r;

				x2 = (int)lx;
				y2 = (int)ty_l;

				x3 = (int)lx;
				y3 = (int)by_l;

				x4 = (int)rx;
				y4 = (int)by_r;
			}

			// 表か裏か
			int handle;

			// 裏向きのときはカードの裏の画像を描画するように変更　追加　小池　5/15

			// 表裏を逆にする
			if (mbEnemyCardFront)
			{
				// 表の時は共通裏画像を表示
				handle = mnEnemyCardBack;
			}
			else
			{
				// 裏の時はカード本体を描画
				SetDrawScreen(mnCardWorkScreen);

				// 前の絵を消す
				ClearDrawScreen();

				// カード描画
				mpEnemyActionCard->Draw(
					mnCardWorkScreen,
					mnCardFontHandle,
					false
				);

				// 描画先を戻す
				SetDrawScreen(DX_SCREEN_BACK);

				handle = mnCardWorkScreen;
			}

			// 暗くする
			int bright =
				(int)(180 + 75 *
					fabsf(cosVal));

			SetDrawBright(
				bright,
				bright,
				bright
			);

			/*DrawModiGraph(
				x1, y1,
				x2, y2,
				x3, y3,
				x4, y4,
				handle,
				TRUE
			);*/

			SetDrawBright(
				255, 255, 255);
		}
	}

	int MouseX, MouseY;
	GetMousePoint(&MouseX, &MouseY); // マウスの位置の取得

	int y = 70; // 表示開始位置
	int fontSize = 24;
	unsigned int color = GetColor(255, 255, 255);

	if (mpPlayer) // MPの描画
	{
		DrawRotaGraph(125, 740, 0.3f, 0.0f, mnHandleMP, TRUE);

		// MPの描画
		DrawFormatStringToHandle(103, 718, color, Master::mpFontManager->GetMpFontHandle(), "%d", mpPlayer->mp);

		// MPが足りなかったら
		if (mpPlayer->GetMpError())
		{
			DrawRotaGraph((Utility::SCREEN_WIDTH / 2), 630, 0.2f, 0.0f, mnHandleIsMpError, TRUE);
		}

		// ゴミ箱の画像描画
		DrawRotaGraph(Utility::SCREEN_WIDTH - 110, 970, 0.3f, 0.0f, mnHandleTrash, TRUE);

		// 山札の画像と円と文字
		DrawRotaGraph(125, 940, 0.44f, 0.0f, mnHandleDeck, TRUE);
		DrawCircle(190, 994, 24, GetColor(255, 0, 0), TRUE);


		// 山札の文字を合わせるため
		int value = (int)mpPlayer->deck.size();
		char str[32];
		sprintf_s(str, "%d", value);
		// 文字列の横幅を取得
		int width = GetDrawStringWidth(str, (int)strlen(str));
		// 中央座標に対して補正している
		int centerX = 190;
		int drawX = centerX - width / 2;
		// 描画
		DrawFormatStringToHandle(drawX, 985, color,Master::mpFontManager->GetItemFontHandle(), "%s", str);


	}

	if (mpTurnEnd != nullptr)
	{
		mpTurnEnd->Draw();
	}

	if (mpStop)
	{
		mpStop->Draw();
	}


	
	/*// デバック用
	if (mpPlayer->IsCardMove())
	{
		//DrawFormatString(100, 980, color, "カード移動中");
	}
	if (ChargeCount == 4)
	{
		//DrawFormatString(0, 600, GetColor(255, 0, 0), "次は３０ダメージがくる！！！", TRUE);
	}

	if (mpEnemy && mpEnemy->stunTurns > 0)
	{
		//DrawFormatString(Utility::SCREEN_WIDTH - 400, 450, GetColor(255, 255, 0), "【スタン中：残り %d ターン】", mpEnemy->stunTurns);
	}*/


    // アイテム通知
	if (!mItemMessage.empty())
	{
		// ノート
		DrawGraph(
			(int)mfItemNoteX,
			520,
			mnNoteMini,
			TRUE
		);

		// メッセージ
		DrawFormatStringToHandle(
			(int)mfItemNoteX + 50,
			590,
			mnColor,
			Master::mpFontManager->GetStatusFontHandle(),
			"%s",
			mItemMessage.c_str()
		);
	}


	// ターン数の表示　バウンドありverにしてる
	DrawRotaGraph(330, (int)mfTurnBox + 30, 0.20f, 0.0f, mnHandleTurnBag, TRUE); // 背景のミニミニ黒板ちゃん
	DrawFormatStringToHandle(215, (int)mfTurnY, GetColor(255, 255, 255), Master::mpFontManager->GetScoreFontHandle(),
		"%dターン目", (int)Master::mpTurnCount);
	


	// 止まっている時は画面を暗くしている  黒いBOXだしている
	if (mbStop)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		int x = Utility::SCREEN_WIDTH / 2;
		int y = Utility::SCREEN_HEIGHT / 2;
		DrawRotaGraph(x, y, 0.9f, 0.0f,mnNoteHandle, TRUE);

		mpTitle->Draw();
		mpGame->Draw();
		mpExplain->Draw();
		mpHaguruma->Draw();

		// 選択が選ばれたら
		if (mbSelectON)
		{
			DrawRotaGraph(x, Utility::SCREEN_HEIGHT/2 - 20, 0.80f, 0.0f, mnItemEHandle, TRUE);
			mpClose->Draw();
		}



		// 音量設定画面
		if (mbOption)
		{
			int color = GetColor(255, 255, 255);

			// 背景暗く
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);

			DrawBox(
				0,
				0,
				Utility::SCREEN_WIDTH,
				Utility::SCREEN_HEIGHT,
				GetColor(0, 0, 0),
				TRUE
			);

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// 黒板
			DrawRotaGraph(
				Utility::SCREEN_WIDTH / 2,
				Utility::SCREEN_HEIGHT / 2,
				1.0f,
				0.0f,
				mnMusicBag,
				TRUE
			);

			// タイトル
			DrawStringToHandle(
				800,
				300,
				"音量設定",
				color,
				Master::mpFontManager->GetSelectFontHandle()
			);

			// =========================
			// BGM
			// =========================

			DrawStringToHandle(
				370,
				450,
				"BGM",
				color,
				Master::mpFontManager->GetTextFontHandle()
			);

			DrawBox(
				mBarX,
				mBgmBarY,
				mBarX + (mBgmVolume * mBarWidth / 100),
				mBgmBarY + mBarHeight,
				GetColor(238, 136, 214),
				TRUE
			);

			DrawBox(
				mBarX,
				mBgmBarY,
				mBarX + mBarWidth,
				mBgmBarY + mBarHeight,
				color,
				FALSE
			);

			DrawFormatStringToHandle(
				mBarX + (mBgmVolume * mBarWidth / 100) - 20,
				mBgmBarY - mBarString,
				color,
				Master::mpFontManager->GetMusicFontHandle(),
				"%d",
				mBgmVolume
			);

			DrawRotaGraph(
				mBarX + (mBgmVolume * mBarWidth / 100),
				mBgmBarY + mMusicNote,
				0.3f,
				0.0f,
				mnOnpuHandle,
				TRUE
			);

			// =========================
			// SE
			// =========================

			DrawStringToHandle(
				370,
				640,
				"SE",
				color,
				Master::mpFontManager->GetTextFontHandle()
			);

			DrawBox(
				mBarX,
				mSeBarY,
				mBarX + (mSeVolume * mBarWidth / 100),
				mSeBarY + mBarHeight,
				GetColor(252, 235, 98),
				TRUE
			);

			DrawBox(
				mBarX,
				mSeBarY,
				mBarX + mBarWidth,
				mSeBarY + mBarHeight,
				color,
				FALSE
			);

			DrawFormatStringToHandle(
				mBarX + (mSeVolume * mBarWidth / 100) - 20,
				mSeBarY - mBarString,
				color,
				Master::mpFontManager->GetMusicFontHandle(),
				"%d",
				mSeVolume
			);

			DrawRotaGraph(
				mBarX + (mSeVolume * mBarWidth / 100),
				mSeBarY + mMusicNote,
				0.3f,
				0.0f,
				mnOnpuHandle,
				TRUE
			);

			// ×ボタン
			if (mpMusicClose)
			{
				mpMusicClose->Draw();
			}
		}

	}
	// PS.ここから下に描画したらポーズ中でも暗くならないから注意ね   大谷

}


// 毒とかターンのやつ
void GameScene::UpdateUnitStatus(Unit& unit)
{
	// 毒ダメージの処理
	for (auto& d : unit.debuffs)
	{
		if (d.type == DebuffType::DEBUFF_POISON)
		{
			unit.hp -= d.value;
		}
	}

	// ターン数を減らす
	for (auto& b : unit.buffs) { b.turns--; }
	for (auto& d : unit.debuffs) { d.turns--; }

	// 0以下になったバフ・デバフを消去する
	// std::remove_if... 消したいやつを端っこに寄せるやつ  メモリから消えるわけじゃないから注意ね
	// ↑は こっから先はゴミや　っていう境界線の場所返す
	// turnsが0以下ならtrue(削除対象と判定)　という意味

	// unit.なんか.erase... ゴミを削除するやつ	remove_ifが教えてくれた妖怪からendまで切り捨てる
	unit.buffs.erase(
		std::remove_if(unit.buffs.begin(), unit.buffs.end(), [](const Buff& b) { return b.turns <= 0; }),
		unit.buffs.end());

	unit.debuffs.erase(
		std::remove_if(unit.debuffs.begin(), unit.debuffs.end(), [](const Debuff& d) { return d.turns <= 0; }),
		unit.debuffs.end());

}



void GameScene::Finalize() // 終了処理
{
	//  フォントハンドルの削除
	DeleteFontToHandle(mnFontBig);
	DeleteFontToHandle(mnFontNormal);
	DeleteFontToHandle(mnFontSmall);

	// スクリーン等の破棄
	if (mnCardWorkScreen != -1) DeleteGraph(mnCardWorkScreen);
	if (mnCardFontHandle != -1) DeleteFontToHandle(mnCardFontHandle);

	delete mpTurnEnd;
	delete mpStop;
	delete mpTitle;
	delete mpExplain;
	delete mpGame;
	delete mpClose;
	
	// 画像の削除
	if (mnHandleMP != -1) DeleteGraph(mnHandleMP);
	if (mnBagHandle != -1) DeleteGraph(mnBagHandle);
	if (mnHandleIsMpError != -1) DeleteGraph(mnHandleIsMpError);
	if (mnHandleDeck != -1) DeleteGraph(mnHandleDeck);
	if (mnHandleTrash != -1) DeleteGraph(mnHandleTrash);
	if (mnItemEHandle != -1) DeleteGraph(mnItemEHandle);
	if (mnHandleTurnBag != -1) DeleteGraph(mnHandleTurnBag);
	if (mnNoteHandle != -1) DeleteGraph(mnNoteHandle);
	if (mnNoteMini != -1) DeleteGraph(mnNoteMini);

	delete mpMusicClose;
	mpMusicClose = nullptr;

	if (mnMusicBag != -1) DeleteGraph(mnMusicBag);
	if (mnOnpuHandle != -1) DeleteGraph(mnOnpuHandle);
}



