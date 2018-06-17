

#ifndef GUITILEPICHIGHTLIGHTED_H
#define GUITILEPICHIGHTLIGHTED_H

class CGUITilepicHightlighted : public CGUITilepic
{
    ushort SelectedColor = 0;

    bool DoubleDraw = false;

public:
    CGUITilepicHightlighted(
        int serial,
        ushort graphic,
        ushort color,
        ushort selectedColor,
        int x,
        int y,
        bool doubleDraw);
    virtual ~CGUITilepicHightlighted();

    virtual void SetShaderMode();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();

    virtual void OnMouseEnter();
    virtual void OnMouseExit();
};

#endif
