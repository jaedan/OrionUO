

#ifndef GAMEWORLD_H
#define GAMEWORLD_H

typedef map<uint, CGameObject *> WORLD_MAP;

class CGameWorld
{
public:
    uint ObjectToRemove = 0;

private:
    void CreatePlayer(int serial);

    void RemovePlayer();

public:
    CGameWorld(int serial);
    ~CGameWorld();

    WORLD_MAP m_Map;

    CGameObject *m_Items{ NULL };

    void ResetObjectHandlesState();

    void ProcessAnimation();

    void ProcessSound(int ticks, CGameCharacter *gc);

    void SetPlayer(int serial);

    CGameItem *GetWorldItem(int serial);

    CGameCharacter *GetWorldCharacter(int serial);

    CGameObject *FindWorldObject(int serial);

    CGameItem *FindWorldItem(int serial);

    CGameCharacter *FindWorldCharacter(int serial);

    void ReplaceObject(CGameObject *obj, int newSerial);

    void RemoveObject(CGameObject *obj);

    void RemoveFromContainer(CGameObject *obj);

    void ClearContainer(CGameObject *obj);

    void PutContainer(CGameObject *obj, int containerSerial)
    {
        CGameObject *cnt = FindWorldObject(containerSerial);
        if (cnt != NULL)
            PutContainer(obj, cnt);
    }

    void PutContainer(CGameObject *obj, CGameObject *container);

    void PutEquipment(CGameItem *obj, int containerSerial, int layer)
    {
        CGameObject *cnt = FindWorldObject(containerSerial);
        if (cnt != NULL)
            PutEquipment(obj, cnt, layer);
    }

    void PutEquipment(CGameItem *obj, CGameObject *container, int layer)
    {
        PutContainer(obj, container);
        obj->Layer = layer;
    }

    void MoveToTop(CGameObject *obj);

    void Dump(uchar nCount = 0, uint serial = 0xFFFFFFFF);

    CGameObject *SearchWorldObject(
        int serialStart, int scanDistance, SCAN_TYPE_OBJECT scanType, SCAN_MODE_OBJECT scanMode);

    void UpdateContainedItem(
        int serial,
        ushort graphic,
        uchar graphicIncrement,
        ushort count,
        int x,
        int y,
        int containerSerial,
        ushort color);

    void UpdateItemInContainer(CGameObject *obj, CGameObject *container, int x, int y);

    void UpdateGameObject(
        int serial,
        ushort graphic,
        uchar graphicIncrement,
        int count,
        int x,
        int y,
        char z,
        uchar direction,
        ushort color,
        uchar flags,
        int a11,
        UPDATE_GAME_OBJECT_TYPE updateType,
        ushort a13);

    void UpdatePlayer(
        int serial,
        ushort graphic,
        uchar graphicIncrement,
        ushort color,
        uchar flags,
        int x,
        int y,
        ushort serverID,
        uchar direction,
        char z);
};

extern CGameWorld *g_World;

#endif
