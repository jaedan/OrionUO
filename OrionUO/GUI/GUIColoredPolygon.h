

#ifndef GUICOLOREDPOLYGON_H
#define GUICOLOREDPOLYGON_H

class CGUIColoredPolygon : public CGUIPolygonal
{
public:
    uchar ColorR = 0;

    uchar ColorG = 0;

    uchar ColorB = 0;

    uchar ColorA = 0;

    bool DrawDot = false;

    bool Focused = false;

    CGUIColoredPolygon(
        int serial, ushort color, int x, int y, int width, int height, int polygonColor);
    virtual ~CGUIColoredPolygon();

    void UpdateColor(ushort color, int polygonColor);

    virtual void Draw(bool checktrans = false);

    virtual void OnMouseEnter();
    virtual void OnMouseExit();
};

#endif
