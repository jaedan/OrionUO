

#ifndef GUIGUMPPICTILED_H
#define GUIGUMPPICTILED_H

class CGUIGumppicTiled : public CGUIDrawObject
{
    int Width = 0;

    int Height = 0;

public:
    CGUIGumppicTiled(ushort graphic, int x, int y, int width, int height);
    virtual ~CGUIGumppicTiled();

    virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(Width, Height); }

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};

#endif
