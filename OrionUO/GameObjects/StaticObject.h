

#ifndef STATICOBJECT_H
#define STATICOBJECT_H

class CStaticObject : public CRenderStaticObject
{
    ushort OriginalGraphic = 0;

public:
    CStaticObject(int serial, ushort graphic, ushort color, short x, short y, char z);
    virtual ~CStaticObject() {}

    virtual void UpdateGraphicBySeason();

    virtual void Draw(int x, int y);

    virtual void Select(int x, int y);

    bool IsStaticObject() { return true; }
};

#endif
