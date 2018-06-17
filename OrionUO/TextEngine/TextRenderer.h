

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

class CTextRenderer : public CRenderTextObject
{
private:
    deque<CTextImageBounds> m_TextRect;

    bool ProcessTextRemoveBlending(CTextData &text);

public:
    CTextRenderer();
    virtual ~CTextRenderer();

    CRenderTextObject *m_TextItems{ NULL };

    CRenderTextObject *m_DrawPointer{ NULL };

    CRenderTextObject *AddText(CRenderTextObject *obj);

    void ToTop(CRenderTextObject *obj);

    void AddRect(const CTextImageBounds &rect) { m_TextRect.push_back(rect); }

    void ClearRect() { m_TextRect.clear(); }

    bool InRect(CTextData *text, CRenderWorldObject *rwo);

    bool CalculatePositions(bool noCalculate);

    void Draw();

    void Select(class CGump *gump);

    bool CalculateWorldPositions(bool noCalculate);

    void WorldDraw();
};

extern CTextRenderer g_WorldTextRenderer;

#endif
