

#include "stdafx.h"

deque<CContainerStackItem> g_ContainerStack;
uint g_CheckContainerStackTimer = 0;

CContainerStackItem::CContainerStackItem(
    int serial,
    short x,
    short y,
    short minimizedX,
    short minimizedY,
    bool minimized,
    bool lockMoving)
    : Serial(serial)
    , X(x)
    , Y(y)
    , MinimizedX(minimizedX)
    , MinimizedY(minimizedY)
    , Minimized(minimized)
    , LockMoving(lockMoving)
{
}
