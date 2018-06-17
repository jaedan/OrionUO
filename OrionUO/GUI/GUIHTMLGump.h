

#ifndef GUIHTMLGUMP_H
#define GUIHTMLGUMP_H

class CGUIHTMLGump : public CGUIPolygonal
{
public:
    bool HaveBackground = false;

    bool HaveScrollbar = false;

    WISP_GEOMETRY::CSize DataSize = WISP_GEOMETRY::CSize();

    WISP_GEOMETRY::CPoint2Di DataOffset = WISP_GEOMETRY::CPoint2Di();

    WISP_GEOMETRY::CPoint2Di CurrentOffset = WISP_GEOMETRY::CPoint2Di();

    WISP_GEOMETRY::CPoint2Di AvailableOffset = WISP_GEOMETRY::CPoint2Di();

private:
    void CalculateDataSize(CBaseGUI *item, int &startX, int &startY, int &endX, int &endY);

public:
    CGUIHTMLGump(
        int serial,
        ushort graphic,
        int x,
        int y,
        int width,
        int height,
        bool haveBackground,
        bool haveScrollbar);
    virtual ~CGUIHTMLGump();

    CGUIHTMLResizepic *m_Background{ NULL };
    CGUIHTMLButton *m_ButtonUp{ NULL };
    CGUIHTMLButton *m_ButtonDown{ NULL };
    CGUIHTMLHitBox *m_HitBoxLeft{ NULL };
    CGUIHTMLHitBox *m_HitBoxRight{ NULL };
    CGUIHTMLSlider *m_Slider{ NULL };
    CGUIScissor *m_Scissor{ NULL };

    void Initalize(bool menu = false);

    void UpdateHeight(int height);

    void ResetDataOffset();

    void CalculateDataSize();

    virtual void PrepareTextures();

    virtual bool EntryPointerHere();

    virtual bool Select();

    virtual void Scroll(bool up, int delay);

    virtual bool IsHTMLGump() { return true; }
};

#endif
