

#ifndef MAPBLOCK_H
#define MAPBLOCK_H

class CMapBlock : public CBaseQueueItem
{
public:
    uint Index = 0;
    uint LastAccessTime = 0;
    short X = 0;
    short Y = 0;

private:
    char GetLandZ(int x, int y, int map);

    bool TestStretched(int x, int y, char z, int map, bool recurse);

public:
    CMapBlock(int index);
    virtual ~CMapBlock();

    CMapObject *Block[8][8];

    CMapObject *AddObject(CMapObject *obj, int x, int y);

    CLandObject *GetLand(int x, int y);

    void AddRender(CRenderWorldObject *item, int x, int y);

    CRenderWorldObject *GetRender(int x, int y);

    ushort GetRadarColor(int x, int y);

    void CreateLandTextureRect();

    bool HasNoExternalData();
};

#endif
