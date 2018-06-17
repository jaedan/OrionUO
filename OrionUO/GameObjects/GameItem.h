

#ifndef GAMEITEM_H
#define GAMEITEM_H

class CGameItem : public CGameObject
{
public:
    uchar Layer = 0;

    ushort AnimID = 0;

    uchar UsedLayer = 0;

    bool Opened = false;

    bool Dragged = false;

    bool MultiBody = false;

    bool WantUpdateMulti = true;

    ushort FieldColor = 0;

    short MultiDistanceBonus = 0;

    uint Price = 0;

    bool NameFromCliloc = false;

    ushort MultiTileGraphic = 0;

    uchar LightID = 0;

protected:
    void AddMultiObject(CMultiObject *obj);

public:
    CGameItem(int serial = 0);
    virtual ~CGameItem();

    void CalculateFieldColor();

    CMulti *GetMulti();

    void ClearMultiItems();

    virtual void Draw(int x, int y);

    virtual void Select(int x, int y);

    void ClearCustomHouseMultis(int state);

    CMultiObject *
    AddMulti(ushort graphic, ushort color, char x, char y, char z, bool isCustomHouseMulti);

    void OnGraphicChange(int direction = 0);

    ushort GetMountAnimation();

    virtual bool IsInternal() { return ((m_TiledataPtr->Flags & 0x00010000) && !IsCorpse()); }

    bool IsHuman()
    {
        return (
            IsCorpse() && (IN_RANGE(Count, 0x0192, 0x0193) || IN_RANGE(Count, 0x025F, 0x0260) ||
                           IN_RANGE(Count, 0x02B6, 0x02B7)));
    }

    void LoadMulti(bool dropAlpha);

    CMulti *GetMultiAtXY(short x, short y);

    CGameItem *FindItem(ushort graphic, ushort color = 0xFFFF);
};

#endif
