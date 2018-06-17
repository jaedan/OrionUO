

#ifndef TARGET_H
#define TARGET_H

class CTarget
{
public:
    uchar Type = 0;

    uchar CursorType = 0;

    ushort MultiGraphic = 0;

    ushort MultiX = 0;

    ushort MultiY = 0;

    uint CursorID = 0;

    bool Targeting = false;

private:
    uchar m_Data[19];
    uchar m_LastData[19];

    CMulti *m_Multi{ NULL };

    void AddMultiObject(CMultiObject *obj);

public:
    CTarget();
    ~CTarget() {}

    void SetLastTargetObject(int serial);

    void SetData(WISP_DATASTREAM::CDataReader &reader);

    void SetMultiData(WISP_DATASTREAM::CDataReader &reader);

    bool IsTargeting() const { return Targeting; }

    void Reset();

    void RequestFromCustomHouse();

    void SendTargetObject(int Serial);

    void SendTargetTile(ushort tileID, short x, short Y, char z);

    void SendCancelTarget();

    void Plugin_SendTargetObject(int Serial);

    void Plugin_SendTargetTile(ushort tileID, short x, short Y, char z);

    void Plugin_SendCancelTarget();

    void Plugin_SendTarget();

    void SendLastTarget();

    void SendTarget();

    void LoadMulti(int offsetX, int offsetY, char offsetZ);

    void UnloadMulti();

    CMulti *GetMultiAtXY(short x, short y);
};

extern CTarget g_Target;

#endif
