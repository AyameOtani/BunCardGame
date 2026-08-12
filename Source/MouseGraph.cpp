#include "MouseGraph.h"
#include "Mouse.h"


// コンストラク
MouseGraph::MouseGraph(float x, float y, float angle, std::string filename, float rate, float changerate)
{
	// 代入
	this->mx = x;
	this->my = y;
	this->mAngle = angle;
	this->mRate = rate;
	this->mChangeRate = changerate;

	this->isActive = true; // 最初は有効
	this->isHover = false;

	// 画像の読み込み
	this->mnHandle = LoadGraph(filename.c_str());

	if (mnHandle == -1)
	{
		printfDx("画像読み込み失敗");
	}
}

MouseGraph::~MouseGraph()
{
	if (mnHandle != -1)
	{
		 DeleteGraph(mnHandle);
		 mnHandle = -1;
	}
}

void MouseGraph::Update()
{
	if (mnHandle == -1) return;

	int w, h;
	GetGraphSize(mnHandle, &w, &h);


	// ボタンが有効じゃなかったら
	if (!isActive)
	{
		isHover = false;
		return;
	}


	// マウスと重なっているかの判定 拡大率を考慮
	if (Mouse::x >= mx - (w * mRate) / 2 && Mouse::x <= mx + (w * mRate) / 2 &&
		Mouse::y >= my - (h * mRate) / 2 && Mouse::y <= my + (h * mRate) / 2)
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
	if (mnHandle == -1) return;


	int w, h;
	GetGraphSize(mnHandle, &w, &h);


	// 拡大率を変える
	float rate = isHover ? mChangeRate : mRate;


	// 2D用に設定
	//SetUseZBufferFlag(FALSE);
	//SetWriteZBufferFlag(FALSE);

	if (!isActive)
	{
		SetDrawBlendMode(DX_BLENDMODE_MULA, 255);
	}

	// 描画
	DrawRotaGraph((int)mx, (int)my, rate, mAngle, mnHandle, TRUE);

	// 3D用に設定
	//SetUseZBufferFlag(TRUE);
	//SetWriteZBufferFlag(TRUE);


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


}


bool MouseGraph::IsClicked()
{
	// マウスがのってるかつクリックされたらtrue
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