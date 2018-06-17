

#ifndef GUIBUTTONTILEART_H
#define GUIBUTTONTILEART_H

class CGUIButtonTileart : public CGUIButton
{
    ushort TileGraphic = 0;

    ushort TileColor = 0;

    int TileX = 0;

    int TileY = 0;

public:
    CGUIButtonTileart(
        int serial,
        ushort graphic,
        ushort graphicSelected,
        ushort graphicPressed,
        int x,
        int y,
        ushort tileGraphic,
        ushort tileColor,
        int tileX,
        int tileY);
    virtual ~CGUIButtonTileart();

    virtual WISP_GEOMETRY::CSize GetSize();

    virtual void PrepareTextures();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};

#endif
