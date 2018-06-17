

#ifndef GUIHTMLHITBOX_H
#define GUIHTMLHITBOX_H

class CGUIHTMLHitBox : public CGUIHitBox
{
private:
    class CGUIHTMLGump *m_HTMLGump{ NULL };

public:
    CGUIHTMLHitBox(
        class CGUIHTMLGump *htmlGump,
        int serial,
        int x,
        int y,
        int width,
        int height,
        bool callOnMouseUp = false);
    virtual ~CGUIHTMLHitBox();

    void Scroll(bool up, int delay);

    virtual bool IsControlHTML() { return true; }
};

#endif
