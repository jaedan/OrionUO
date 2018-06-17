

#ifndef GUIPOLYGONAL_H
#define GUIPOLYGONAL_H

class CGUIPolygonal : public CBaseGUI
{
public:
    int Width = 0;

    int Height = 0;

    bool CallOnMouseUp = false;

    CGUIPolygonal(
        GUMP_OBJECT_TYPE type, int x, int y, int width, int height, bool callOnMouseUp = false);
    virtual ~CGUIPolygonal();

    virtual bool Select();

    virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(Width, Height); }
};

#endif
