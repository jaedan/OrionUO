

#ifndef LANDOBJECT_H
#define LANDOBJECT_H

class CLandObject : public CMapObject
{
public:
    char MinZ = 0;

    char AverageZ = 0;

    bool IsStretched = false;

    ushort OriginalGraphic = 0;

private:
    int GetDirectionZ(int direction);

public:
    CLandObject(int serial, ushort graphic, ushort color, short x, short y, char z);
    virtual ~CLandObject();

    RECT m_Rect;

    virtual void UpdateGraphicBySeason();

    int CalculateCurrentAverageZ(int z);

    CVector m_Normals[4];

    virtual void Draw(int x, int y);

    virtual void Select(int x, int y);

    bool IsLandObject() { return true; }

    void UpdateZ(int zTop, int zRight, int zBottom);

    bool Ignored()
    {
        return (Graphic == 2 || Graphic == 0x1DB || (Graphic >= 0x1AE && Graphic <= 0x1B5));
    }

    virtual CLandObject *LandObjectPtr() { return this; }
};

#endif
