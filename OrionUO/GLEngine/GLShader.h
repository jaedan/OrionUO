

#ifndef GLSHADER_H
#define GLSHADER_H

void UnuseShader();

class CGLShader
{
protected:
    GLhandleARB m_Shader{ 0 };

    GLhandleARB m_VertexShader{ 0 };

    GLhandleARB m_FragmentShader{ 0 };

    GLuint m_TexturePointer{ 0 };

public:
    CGLShader();
    virtual ~CGLShader();

    virtual bool Init(const char *vertexShaderData, const char *fragmentShaderData);

    virtual bool Use();

    virtual void Pause();

    virtual void Resume();
};

class CDeathShader : public CGLShader
{
public:
    CDeathShader();

    virtual bool Init(const char *vertexShaderData, const char *fragmentShaderData);
};

class CColorizerShader : public CGLShader
{
protected:
    GLuint m_ColorTablePointer{ 0 };

    GLuint m_DrawModePointer{ 0 };

public:
    CColorizerShader();

    virtual bool Init(const char *vertexShaderData, const char *fragmentShaderData);

    virtual bool Use();
};

extern CDeathShader g_DeathShader;
extern CColorizerShader g_ColorizerShader;
extern CColorizerShader g_FontColorizerShader;
extern CColorizerShader g_LightColorizerShader;

#endif
