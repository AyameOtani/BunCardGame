#include "MouseGraph.h"
#include "Mouse.h"

// コンストラクタ
MouseGraph::MouseGraph(float x, float y, float angle, std::string filename, float rate, float changerate)
{
	// 各種パラメータを保持して初期状態を構築するため
	this->mx = x;
	this->my = y;
	this->mAngle = angle;
	this->mRate = rate;
	this->mChangeRate = changerate;

	this->isActive = true;
	this->isHover = false;

	// 指定されたファイルパスから画像データを読み込むため
	this->mnHandle = LoadGraph(filename.c_str());

	if (mnHandle == InvalidGraphHandle)
	{
		printfDx("画像読み込み失敗");
	}
}

MouseGraph::~MouseGraph()
{
	// 保持している画像ハンドルが有効な場合のみメモリから解放するため
	if (mnHandle != InvalidGraphHandle)
	{
		DeleteGraph(mnHandle);
		mnHandle = InvalidGraphHandle;
	}
}

void MouseGraph::Update()
{
	// 画像の読み込みに失敗している場合は判定を行わないため
	if (mnHandle == InvalidGraphHandle) return;

	int w, h;
	GetGraphSize(mnHandle, &w, &h);

	// ボタンが無効な状態のときはホバー判定を無効化するため
	if (!isActive)
	{
		isHover = false;
		return;
	}

	// 現在の拡大率を考慮した当たり判定の範囲内にマウスがあるかを判定するため
	if (Mouse::x >= mx - (w * mRate) / CenterDivisor && Mouse::x <= mx + (w * mRate) / CenterDivisor &&
		Mouse::y >= my - (h * mRate) / CenterDivisor && Mouse::y <= my + (h * mRate) / CenterDivisor)
	{
		isHover = true;
	}
	else
	{
		isHover = false;
	}
}

void MouseGraph::Draw()
{
	// 描画対象の画像が正常に読み込まれている場合のみ描画を行うため
	if (mnHandle == InvalidGraphHandle) return;

	int w, h;
	GetGraphSize(mnHandle, &w, &h);

	// マウスが重なっている状態に応じて適用する拡大率を切り替えるため
	float rate = isHover ? mChangeRate : mRate;

	// 無効な状態のときは暗くして描画するためにブレンドモードを設定するため
	if (!isActive)
	{
		SetDrawBlendMode(DX_BLENDMODE_MULA, BlendModeParamMax);
	}

	// 指定された座標と拡大率で画像を中心基準で回転描画するため
	DrawRotaGraph((int)mx, (int)my, rate, mAngle, mnHandle, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool MouseGraph::IsClicked()
{
	// マウスが乗っていて、かつクリックの瞬間であるかを返すため
	return isHover && Mouse::IsTrigger();
}

void MouseGraph::SetPosition(float x, float y)
{
	// 座標を更新するため
	mx = x;
	my = y;
}

void MouseGraph::SetAngle(float angle)
{
	// 角度を更新するため
	mAngle = angle;
}