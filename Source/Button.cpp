#include "Button.h"
#include "Mouse.h"

Button::Button(int x1, int y1, int x2, int y2, int color, int changeColor, std::string memo)
{
    // ボタンの初期位置や色などのデータを保持するため
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->color = color;
    this->changeColor = changeColor;
    this->isHover = false;
    this->isActive = true;
    this->scale = DefaultScale;
    this->memo = memo;
    this->stringColor = GetColor(DefaultStringColorMax, DefaultStringColorMax, DefaultStringColorMax);
}

void Button::Update()
{
    // ボタンが無効な状態のときは判定を行わないようにするため
    if (!isActive)
    {
        isHover = false;
        return;
    }

    // マウスカーソルがボタンの範囲内に入っているかを判定するため
    if (Mouse::x >= x1 && Mouse::x <= x2 && Mouse::y >= y1 && Mouse::y <= y2)
    {
        isHover = true;
    }
    else
    {
        isHover = false;
    }
}

void Button::Draw()
{
    unsigned int drawColor{};

    // ボタンの状態に応じた見た目の変化を決定するため
    if (!isActive)
    {
        // プレイヤーが操作できない無効な状態であることを視覚的に伝えるため
        drawColor = GetColor(InactiveColor, InactiveColor, InactiveColor);
        scale = DefaultScale;
        stringColor = GetColor(InactiveStringColorValue, InactiveStringColorValue, InactiveStringColorValue);
    }
    else if (isHover)
    {
        scale = HoverScale;

        // マウスが重なっている状態でクリックされているかを判定するため
        if (Mouse::IsPress())
        {
            drawColor = changeColor;
            scale = DefaultScale;
        }
        else
        {
            scale = HoverScale;
            drawColor = color;
        }
    }
    else
    {
        scale = DefaultScale;
        drawColor = color;
        stringColor = GetColor(DefaultStringColorMax, DefaultStringColorMax, DefaultStringColorMax);
    }

    // 拡大縮小の中心を基準にした描画座標を算出するため
    float cx = (x1 + x2) / CenterDivisor;
    float cy = (y1 + y2) / CenterDivisor;
    float w = (float)(x2 - x1);
    float h = (float)(y2 - y1);

    float drawX1 = cx - (w * scale) / CenterDivisor;
    float drawY1 = cy - (h * scale) / CenterDivisor;
    float drawX2 = cx + (w * scale) / CenterDivisor;
    float drawY2 = cy + (h * scale) / CenterDivisor;

    int stringW = GetDrawFormatStringWidth("%s", memo.c_str());

    DrawBox((int)drawX1, (int)drawY1, (int)drawX2, (int)drawY2, drawColor, TRUE);
    DrawFormatString((int)(cx - stringW / CenterDivisor), (int)(cy - StringHeightOffset), stringColor, "%s", memo.c_str());
}

bool Button::IsClicked()
{
    // ボタンにマウスが乗っていて、かつクリックの瞬間であるかを返すため
    return isHover && Mouse::IsTrigger();
}