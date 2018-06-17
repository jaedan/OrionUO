

#ifndef GUICOLOREDPOLYGONE_H
#define GUICOLOREDPOLYGONE_H

class CGUIColoredPolygone : public CGUIPolygonal
{
public:
    uchar ColorR = 0;

    uchar ColorG = 0;

    uchar ColorB = 0;

    uchar ColorA = 0;

    bool DrawDot = false;

    bool Focused = false;

    CGUIColoredPolygone(
        int serial, ushort color, int x, int y, int width, int height, int polygoneColor);
    virtual ~CGUIColoredPolygone();

    void UpdateColor(ushort color, int polygoneColor);

    virtual void Draw(bool checktrans = false);

    virtual void OnMouseEnter();
    virtual void OnMouseExit();
};

#endif
