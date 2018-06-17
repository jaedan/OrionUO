

#ifndef GUIGUMPPICHIGHTLIGHTED_H
#define GUIGUMPPICHIGHTLIGHTED_H

class CGUIGumppicHightlighted : public CGUIDrawObject
{
    ushort SelectedColor = 0;

public:
    CGUIGumppicHightlighted(
        int serial, ushort graphic, ushort color, ushort electedColor, int x, int y);
    virtual ~CGUIGumppicHightlighted();

    virtual void SetShaderMode();

    virtual void OnMouseEnter();
    virtual void OnMouseExit();
};

#endif
