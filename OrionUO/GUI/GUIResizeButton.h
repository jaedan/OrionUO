

#ifndef GUIRESIZEBUTTON_H
#define GUIRESIZEBUTTON_H

class CGUIResizeButton : public CGUIButton
{
public:
    CGUIResizeButton(
        int serial, ushort graphic, ushort graphicSelected, ushort graphicPressed, int x, int y);
    virtual ~CGUIResizeButton();

    virtual bool IsPressedOuthit() { return true; }
};

#endif
