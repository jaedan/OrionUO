

#ifndef MAPOBJECT_H
#define MAPOBJECT_H

class CMapObject : public CRenderWorldObject
{
public:
    CMapObject(
        RENDER_OBJECT_TYPE type,
        int serial,
        ushort graphic,
        ushort color,
        short x,
        short y,
        char z);

    virtual ~CMapObject();
};

#endif
