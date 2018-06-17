

#ifndef CONTAINER_H
#define CONTAINER_H

class CContainerOffsetRect
{
public:
    int MinX = 0;
    int MinY = 0;
    int MaxX = 0;
    int MaxY = 0;

    CContainerOffsetRect() {}
    CContainerOffsetRect(int minX, int minY, int maxX, int maxY)
        : MinX(minX)
        , MinY(minY)
        , MaxX(maxX)
        , MaxY(maxY)
    {
    }
    ~CContainerOffsetRect() {}
};

class CContainerOffset
{
public:
    ushort Gump = 0;

    ushort OpenSound = 0;

    ushort CloseSound = 0;

    CContainerOffsetRect Rect = CContainerOffsetRect();

    CContainerOffset() {}
    CContainerOffset(
        ushort gump, ushort openSound, ushort closeSound, const CContainerOffsetRect &rect)
        : Gump(gump)
        , OpenSound(openSound)
        , CloseSound(closeSound)
        , Rect(rect)
    {
    }
    ~CContainerOffset() {}
};

class CContainerRect
{
public:
    short DefaultX = 40;
    short DefaultY = 40;

    short X = 40;
    short Y = 40;

    CContainerRect() {}
    ~CContainerRect() {}

    void Calculate(ushort gumpID);

    void MakeDefault()
    {
        X = DefaultX;
        Y = DefaultY;
    }
};

extern vector<CContainerOffset> g_ContainerOffset;

extern CContainerRect g_ContainerRect;

#endif
