

#ifndef GUITILEPIC_H
#define GUITILEPIC_H

class CGUITilepic : public CGUIDrawObject
{
public:
    CGUITilepic(ushort graphic, ushort color, int x, int y);
    virtual ~CGUITilepic();

    virtual WISP_GEOMETRY::CSize GetSize();

    virtual void PrepareTextures();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};

#endif
