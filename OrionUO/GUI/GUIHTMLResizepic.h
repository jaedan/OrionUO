

#ifndef GUIHTMLRESIZEPIC_H
#define GUIHTMLRESIZEPIC_H

class CGUIHTMLResizepic : public CGUIResizepic
{
private:
    class CGUIHTMLGump *m_HTMLGump{ NULL };

public:
    CGUIHTMLResizepic(
        class CGUIHTMLGump *htmlGump,
        int serial,
        ushort graphic,
        int x,
        int y,
        int width,
        int height);
    virtual ~CGUIHTMLResizepic();

    void Scroll(bool up, int delay);

    virtual bool IsControlHTML() { return true; }
};

#endif
