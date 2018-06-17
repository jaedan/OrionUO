

#ifndef GUIHTMLBUTTON_H
#define GUIHTMLBUTTON_H

class CGUIHTMLButton : public CGUIButton
{
private:
    class CGUIHTMLGump *m_HTMLGump{ NULL };

public:
    CGUIHTMLButton(
        class CGUIHTMLGump *htmlGump,
        int serial,
        ushort graphic,
        ushort graphicSelected,
        ushort graphicPressed,
        int x,
        int y);
    virtual ~CGUIHTMLButton();

    virtual void SetShaderMode();

    void Scroll(bool up, int delay);

    virtual bool IsControlHTML() { return true; }
};

#endif
