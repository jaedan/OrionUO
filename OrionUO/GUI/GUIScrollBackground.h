

#ifndef GUISCROLLBACKGROUND_H
#define GUISCROLLBACKGROUND_H

class CGUIScrollBackground : public CBaseGUI
{
public:
    int Height = 0;

    int OffsetX = 0;

    int BottomOffsetX = 0;

    int Width = 0;

    WISP_GEOMETRY::CRect WorkSpace = WISP_GEOMETRY::CRect();

    CGUIScrollBackground(int serial, ushort graphic, int x, int y, int height);
    virtual ~CGUIScrollBackground();

    void UpdateHeight(int height);

    virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(Width, Height); }

    virtual void PrepareTextures();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};

#endif
