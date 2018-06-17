

#ifndef RENDERTEXTOBJECT_H
#define RENDERTEXTOBJECT_H

class CRenderTextObject : public CRenderObject
{
public:
    bool Transparent = false;

    CRenderTextObject();
    virtual ~CRenderTextObject();

    CRenderTextObject *m_NextDraw{ NULL };
    CRenderTextObject *m_PrevDraw{ NULL };

    void UnlinkDraw();

    void ToTop();
};

#endif
