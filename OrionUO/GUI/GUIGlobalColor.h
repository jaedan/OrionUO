

#ifndef GUIGLOBALCOLOR_H
#define GUIGLOBALCOLOR_H

class CGUIGlobalColor : public CBaseGUI
{
    GLfloat R = 0.0f;

    GLfloat G = 0.0f;

    GLfloat B = 0.0f;

    GLfloat A = 0.0f;

public:
    CGUIGlobalColor(bool enabled, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    virtual ~CGUIGlobalColor();

    virtual void Draw(bool checktrans = false);
};

#endif
