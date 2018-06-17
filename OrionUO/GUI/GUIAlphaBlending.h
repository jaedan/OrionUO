

#ifndef GUIALPHABLENDING_H
#define GUIALPHABLENDING_H

class CGUIAlphaBlending : public CGUIBlending
{
public:
    float Alpha = 0.0f;

    CGUIAlphaBlending(bool enabled, float alpha);
    virtual ~CGUIAlphaBlending();

    virtual void Draw(bool checktrans = false);
};

#endif
