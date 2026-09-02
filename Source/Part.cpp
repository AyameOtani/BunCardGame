#include "Part.h"
#include "DxLib.h"

/// <summary>
/// 各パーツを目標の角度や位置へ動かすための更新処理
/// </summary>
void UpdatePart(Part& p)
{
    float speed = partSpeed;
    p.angle += (p.targetAngle - p.angle) * speed;
    p.xOffset += (p.targetXOffset - p.xOffset) * speed;
    p.yOffset += (p.targetYOffset - p.yOffset) * speed;
}

/// <summary>
/// ベースの位置に現在のずれを考慮した位置で描画するための処理
/// </summary>
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