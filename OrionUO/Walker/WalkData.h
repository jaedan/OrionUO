

#ifndef WALKDATA_H
#define WALKDATA_H

class CWalkData
{
public:
    short X = 0;
    short Y = 0;
    char Z = 0;
    uchar Direction = 0;
    ushort Graphic = 0;
    uchar Flags = 0;

    CWalkData() {}
    CWalkData(short x, short y, char z, uchar direction, ushort graphic, uchar flags)
        : X(x)
        , Y(y)
        , Z(z)
        , Direction(direction)
        , Graphic(graphic)
        , Flags(flags)
    {
    }
    ~CWalkData() {}

    bool Run() { return (Direction & 0x80); }

    void GetOffset(float &x, float &y, float &steps);
};

#endif
