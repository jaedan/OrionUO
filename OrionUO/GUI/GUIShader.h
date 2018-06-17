

#ifndef GUISHADER_H
#define GUISHADER_H

class CGUIShader : public CBaseGUI
{
private:
    CGLShader *m_Shader{ NULL };

public:
    CGUIShader(CGLShader *shader, bool enabled);
    virtual ~CGUIShader();

    virtual void Draw(bool checktrans = false);
};

#endif
