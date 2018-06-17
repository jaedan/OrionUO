

#ifndef CUSTOMHOUSEOBJECTS_H
#define CUSTOMHOUSEOBJECTS_H

class CCustomHouseObject
{
public:
    int Category = 0;

    int FeatureMask = 0;

    CCustomHouseObject() {}

    virtual ~CCustomHouseObject() {}

    virtual bool Parse(const char *text) { return false; }
};

class CCustomHouseObjectWall : public CCustomHouseObject
{
public:
    int Style = 0;

    int TID = 0;

    int South1 = 0;

    int South2 = 0;

    int South3 = 0;

    int Corner = 0;

    int East1 = 0;

    int East2 = 0;

    int East3 = 0;

    int Post = 0;

    int WindowS = 0;

    int AltWindowS = 0;

    int WindowE = 0;

    int AltWindowE = 0;

    int SecondAltWindowS = 0;

    int SecondAltWindowE = 0;

    CCustomHouseObjectWall()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
        memset(&m_WindowGraphics[0], 0, sizeof(m_WindowGraphics));
    }

    virtual ~CCustomHouseObjectWall() {}

    virtual bool Parse(const char *text);

    int Contains(ushort graphic) const;

    static const int GRAPHICS_COUNT = 8;

    ushort m_Graphics[GRAPHICS_COUNT];

    ushort m_WindowGraphics[GRAPHICS_COUNT];
};

class CCustomHouseObjectWallCategory
{
public:
    int Index = 0;

    CCustomHouseObjectWallCategory() {}

    virtual ~CCustomHouseObjectWallCategory() {}

    vector<CCustomHouseObjectWall> m_Items;
};

class CCustomHouseObjectFloor : public CCustomHouseObject
{
    int F1 = 0;

    int F2 = 0;

    int F3 = 0;

    int F4 = 0;

    int F5 = 0;

    int F6 = 0;

    int F7 = 0;

    int F8 = 0;

    int F9 = 0;

    int F10 = 0;

    int F11 = 0;

    int F12 = 0;

    int F13 = 0;

    int F14 = 0;

    int F15 = 0;

    int F16 = 0;

public:
    CCustomHouseObjectFloor()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

    virtual ~CCustomHouseObjectFloor() {}

    virtual bool Parse(const char *text);

    int Contains(ushort graphic) const;

    static const int GRAPHICS_COUNT = 16;

    ushort m_Graphics[GRAPHICS_COUNT];
};

class CCustomHouseObjectDoor : public CCustomHouseObject
{
public:
    int Piece1 = 0;

    int Piece2 = 0;

    int Piece3 = 0;

    int Piece4 = 0;

    int Piece5 = 0;

    int Piece6 = 0;

    int Piece7 = 0;

    int Piece8 = 0;

    CCustomHouseObjectDoor()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

    virtual ~CCustomHouseObjectDoor() {}

    virtual bool Parse(const char *text);

    int Contains(ushort graphic) const;

    static const int GRAPHICS_COUNT = 8;

    ushort m_Graphics[GRAPHICS_COUNT];
};

class CCustomHouseObjectMisc : public CCustomHouseObject
{
public:
    int Style = 0;

    int TID = 0;

    int Piece1 = 0;

    int Piece2 = 0;

    int Piece3 = 0;

    int Piece4 = 0;

    int Piece5 = 0;

    int Piece6 = 0;

    int Piece7 = 0;

    int Piece8 = 0;

    CCustomHouseObjectMisc()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

    virtual ~CCustomHouseObjectMisc() {}

    virtual bool Parse(const char *text);

    int Contains(ushort graphic) const;

    static const int GRAPHICS_COUNT = 8;

    ushort m_Graphics[GRAPHICS_COUNT];
};

class CCustomHouseObjectMiscCategory
{
public:
    int Index = 0;

    CCustomHouseObjectMiscCategory() {}

    virtual ~CCustomHouseObjectMiscCategory() {}

    vector<CCustomHouseObjectMisc> m_Items;
};

class CCustomHouseObjectStair : public CCustomHouseObject
{
public:
    int Block = 0;

    int North = 0;

    int East = 0;

    int South = 0;

    int West = 0;

    int Squared1 = 0;

    int Squared2 = 0;

    int Rounded1 = 0;

    int Rounded2 = 0;

    int MultiNorth = 0;

    int MultiEast = 0;

    int MultiSouth = 0;

    int MultiWest = 0;

    CCustomHouseObjectStair()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

    virtual ~CCustomHouseObjectStair() {}

    virtual bool Parse(const char *text);

    int Contains(ushort graphic) const;

    static const int GRAPHICS_COUNT = 9;

    ushort m_Graphics[GRAPHICS_COUNT];
};

class CCustomHouseObjectTeleport : public CCustomHouseObject
{
    int F1 = 0;

    int F2 = 0;

    int F3 = 0;

    int F4 = 0;

    int F5 = 0;

    int F6 = 0;

    int F7 = 0;

    int F8 = 0;

    int F9 = 0;

    int F10 = 0;

    int F11 = 0;

    int F12 = 0;

    int F13 = 0;

    int F14 = 0;

    int F15 = 0;

    int F16 = 0;

public:
    CCustomHouseObjectTeleport()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

    virtual ~CCustomHouseObjectTeleport() {}

    virtual bool Parse(const char *text);

    int Contains(ushort graphic) const;

    static const int GRAPHICS_COUNT = 16;

    ushort m_Graphics[GRAPHICS_COUNT];
};

class CCustomHouseObjectRoof : public CCustomHouseObject
{
public:
    int Style = 0;

    int TID = 0;

    int North = 0;

    int East = 0;

    int South = 0;

    int West = 0;

    int NSCrosspiece = 0;
    int EWCrosspiece = 0;
    int NDent = 0;
    int SDent = 0;
    int WDent = 0;
    int NTPiece = 0;
    int ETPiece = 0;
    int STPiece = 0;
    int WTPiece = 0;
    int XPiece = 0;
    int Extra = 0;
    int Piece = 0;

    CCustomHouseObjectRoof()
        : CCustomHouseObject()
    {
        memset(&m_Graphics[0], 0, sizeof(m_Graphics));
    }

    virtual ~CCustomHouseObjectRoof() {}

    virtual bool Parse(const char *text);

    int Contains(ushort graphic) const;

    static const int GRAPHICS_COUNT = 16;

    ushort m_Graphics[GRAPHICS_COUNT];
};

class CCustomHouseObjectRoofCategory
{
public:
    int Index = 0;

    CCustomHouseObjectRoofCategory() {}

    virtual ~CCustomHouseObjectRoofCategory() {}

    vector<CCustomHouseObjectRoof> m_Items;
};

class CCustomHouseObjectPlaceInfo
{
public:
    int Graphic = 0;

    int Top = 0;

    int Bottom = 0;

    int AdjUN = 0;

    int AdjLN = 0;

    int AdjUE = 0;

    int AdjLE = 0;

    int AdjUS = 0;

    int AdjLS = 0;

    int AdjUW = 0;

    int AdjLW = 0;

    int DirectSupports = 0;

    int CanGoW = 0;

    int CanGoN = 0;

    int CanGoNWS = 0;

    int FeatureMask = 0;

    CCustomHouseObjectPlaceInfo() { memset(&m_Graphics[0], 0, sizeof(m_Graphics)); }

    virtual ~CCustomHouseObjectPlaceInfo() {}

    bool Parse(const char *text);

    int Contains(ushort graphic) const;

    static const int GRAPHICS_COUNT = 1;

    ushort m_Graphics[GRAPHICS_COUNT];
};

#endif
