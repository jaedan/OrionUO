

#ifndef OBJECTONCURSOR_H
#define OBJECTONCURSOR_H

class CObjectOnCursor
{
public:
    bool Enabled = false;

    bool Dropped = false;

    uint Serial = 0;

    ushort Graphic = 0;

    ushort Color = 0;

    ushort Count = 0;

    ushort TotalCount = 0;

    uchar Layer = 0;

    uchar Flags = 0;

    uint Container = 0;

    ushort X = 0;

    ushort Y = 0;

    char Z = 0;

    bool IsGameFigure = false;

    STATIC_TILES *TiledataPtr = NULL;
    bool UpdatedInWorld = false;

    CObjectOnCursor() {}
    virtual ~CObjectOnCursor() {}

    void Clear();

    ushort GetDrawGraphic(bool &doubleDraw);
};

extern CObjectOnCursor g_ObjectInHand;

#endif
