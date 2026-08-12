#include "Part.h"
#include "DxLib.h"

// パーツの更新 アニメーション動かす
void UpdatePart(Part& p)
{
    float speed = 0.2f;
    p.angle += (p.targetAngle - p.angle) * speed;
    p.xOffset += (p.targetXOffset - p.xOffset) * speed;
    p.yOffset += (p.targetYOffset - p.yOffset) * speed;
}

// パーツの描画
void DrawPart(const Part& p, float baseX, float baseY, float size)
{
    DrawRotaGraph(
        (int)(baseX + p.xOffset), //ずらしたやつとかも考慮
        (int)(baseY + p.yOffset),
        size,
        p.angle,
        p.graphHandle,
        TRUE
    );
}