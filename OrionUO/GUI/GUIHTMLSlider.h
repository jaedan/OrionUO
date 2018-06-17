

#ifndef GUIHTMLSLIDER_H
#define GUIHTMLSLIDER_H

class CGUIHTMLSlider : public CGUISlider
{
private:
    class CGUIHTMLGump *m_HTMLGump{ NULL };

public:
    CGUIHTMLSlider(
        class CGUIHTMLGump *htmlGump,
        int serial,
        ushort graphic,
        ushort graphicSelected,
        ushort graphicPressed,
        ushort backgroundGraphic,
        bool compositeBackground,
        bool vertical,
        int x,
        int y,
        int lenght,
        int minValue,
        int maxValue,
        int value);
    virtual ~CGUIHTMLSlider();

    virtual void CalculateOffset();

    virtual bool IsControlHTML() { return true; }
};

#endif
