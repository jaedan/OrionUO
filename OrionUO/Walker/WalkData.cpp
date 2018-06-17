

#include "stdafx.h"

void CWalkData::GetOffset(float &x, float &y, float &steps)
{
    WISPFUN_DEBUG("c178_f1");
    float step_NESW_D = 44.0f / steps;
    float step_NESW = 22.0f / steps;

    int checkX = 22;
    int checkY = 22;

    switch (Direction & 7)
    {
        case 0:
        {
            x *= step_NESW;
            y *= -step_NESW;
            break;
        }
        case 1:
        {
            x *= step_NESW_D;
            checkX = 44;
            y = 0.0f;
            break;
        }
        case 2:
        {
            x *= step_NESW;
            y *= step_NESW;
            break;
        }
        case 3:
        {
            x = 0.0f;
            y *= step_NESW_D;
            checkY = 44;
            break;
        }
        case 4:
        {
            x *= -step_NESW;
            y *= step_NESW;
            break;
        }
        case 5:
        {
            x *= -step_NESW_D;
            checkX = 44;
            y = 0.0f;
            break;
        }
        case 6:
        {
            x *= -step_NESW;
            y *= -step_NESW;
            break;
        }
        case 7:
        {
            x = 0.0f;
            y *= -step_NESW_D;
            checkY = 44;
            break;
        }
        default:
            break;
    }

    int valueX = (int)x;

    if (abs(valueX) > checkX)
    {
        if (valueX < 0)
            x = -(float)checkX;
        else
            x = (float)checkX;
    }

    int valueY = (int)y;

    if (abs(valueY) > checkY)
    {
        if (valueY < 0)
            y = -(float)checkY;
        else
            y = (float)checkY;
    }
}
