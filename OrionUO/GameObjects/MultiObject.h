

#ifndef MULTIOBJECT_H
#define MULTIOBJECT_H

class CMultiObject : public CRenderStaticObject
{
public:
    bool OnTarget = false;

    ushort OriginalGraphic = 0;

    int State = 0;

    CMultiObject(ushort graphic, short x, short y, char z, int flags);

    virtual ~CMultiObject();

    virtual void UpdateGraphicBySeason();

    virtual void Draw(int x, int y);

    virtual void Select(int x, int y);

    bool IsMultiObject() { return true; }
};

#endif
