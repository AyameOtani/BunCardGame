#include "MouseGraph.h"
#include "Mouse.h"

// コンストラクタ
MouseGraph::MouseGraph(float x, float y, float angle, std::string filename, float rate, float changerate)
{
	// 各種パラメータを保持して初期状態を構築するため
	mx = x;
	my = y;
	mAngle = angle;
	mRate = rate;
	mChangeRate = changerate;
	isActive = true;
	isHover = false;
	mnHandle = LoadGraph(filename.c_str());

	if (mnHandle == InvalidGraphHandle)
	{
		printfDx("マウスホバー画像読み込み失敗");
	}
}

MouseGraph::~MouseGraph()
{
	if (mnHandle != InvalidGraphHandle)
	{
		DeleteGraph(mnHandle);
		mnHandle = InvalidGraphHandle;
	}
}

void MouseGraph::Update()
{
	if (mnHandle == InvalidGraphHandle) return;

	int w, h;
	GetGraphSize(mnHandle, &w, &h);

	if (!isActive)
	{
		isHover = false;
		return;
	}

	float halfWidth = (w * mRate) / CenterDivisor;
	float halfHeight = (h * mRate) / CenterDivisor;

	// 現在の拡大率を考慮した当たり判定の範囲内にマウスがあるかを判定するため
	if (Mouse::x >= mx - halfWidth
		&& Mouse::x <= mx + halfWidth
		&&Mouse::y >= my - halfHeight
		&& Mouse::y <= my + halfHeight)
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
	if (mnHandle == InvalidGraphHandle) return;

	int w, h;
	GetGraphSize(mnHandle, &w, &h);
	float rate = isHover ? mChangeRate : mRate;

	// 無効な状態のときは暗くして描画するため
	if (!isActive)
	{
		SetDrawBlendMode(DX_BLENDMODE_MULA, BlendModeParamMax);
	}

	// 指定された座標と拡大率で画像を中心基準で回転描画するため
	DrawRotaGraph((int)mx, (int)my, rate, mAngle, mnHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


// マウスが乗っててかつ押されたらの返すアクセサの中身
bool MouseGraph::IsClicked()
{
	return isHover && Mouse::IsTrigger();
}


void MouseGraph::SetPosition(float x, float y)
{
	mx = x;
	my = y;
}

void MouseGraph::SetAngle(float angle)
{
	mAngle = angle;
}