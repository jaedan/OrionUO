

#ifndef GUIBLENDING_H
#define GUIBLENDING_H

class CGUIBlending : public CBaseGUI
{
    GLenum SFactor = GL_SRC_ALPHA;

    GLenum DFactor = GL_ONE_MINUS_SRC_ALPHA;

public:
    CGUIBlending(bool enabled, GLenum sFactor, GLenum dFactor);
    virtual ~CGUIBlending();

    virtual void Draw(bool checktrans = false);
};

#endif
