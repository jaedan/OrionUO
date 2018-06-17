

#ifndef GUIBUTTON_H
#define GUIBUTTON_H

class CGUIButton : public CGUIDrawObject
{
public:
    ushort GraphicSelected = 0;

    ushort GraphicPressed = 0;

    int ToPage = -1;

    bool ProcessPressedState = false;

    CGUIButton(
        int serial, ushort graphic, ushort graphicSelected, ushort graphicPressed, int x, int y);
    virtual ~CGUIButton();

    virtual void PrepareTextures();
    virtual ushort GetDrawGraphic();

    virtual void OnMouseEnter();
    virtual void OnMouseExit();
};

#endif
