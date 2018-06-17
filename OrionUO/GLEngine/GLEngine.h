

#ifndef GLENGINE_H
#define GLENGINE_H

typedef deque<WISP_GEOMETRY::CRect> SCISSOR_LIST;

class CGLEngine;

typedef void (CGLEngine::*BIND_TEXTURE_16_FUNCTION)(CGLTexture &, int, int, pushort);
typedef void (CGLEngine::*BIND_TEXTURE_32_FUNCTION)(CGLTexture &, int, int, puint);

typedef void (CGLEngine::*DRAW_LAND_TEXTURE_FUNCTION)(
    const CGLTexture &, int, int, class CLandObject *);
typedef void (CGLEngine::*DRAW_TEXTURE_FUNCTION)(const CGLTexture &, int, int);
typedef void (CGLEngine::*DRAW_TEXTURE_ROTATED_FUNCTION)(const CGLTexture &, int, int, float);
typedef void (CGLEngine::*DRAW_TEXTURE_MIRRORED_FUNCTION)(const CGLTexture &, int, int, bool);
typedef void (CGLEngine::*DRAW_TEXTURE_SITTING_FUNCTION)(
    const CGLTexture &, int, int, bool, float, float, float);
typedef void (CGLEngine::*DRAW_TEXTURE_SHADOW_FUNCTION)(const CGLTexture &, int, int, bool);
typedef void (CGLEngine::*DRAW_TEXTURE_STRETCHED_FUNCTION)(const CGLTexture &, int, int, int, int);
typedef void (CGLEngine::*DRAW_TEXTURE_RESIZEPIC_FUNCTION)(CGLTexture **, int, int, int, int);

extern BIND_TEXTURE_16_FUNCTION g_GL_BindTexture16_Ptr;
extern BIND_TEXTURE_32_FUNCTION g_GL_BindTexture32_Ptr;

extern DRAW_LAND_TEXTURE_FUNCTION g_GL_DrawLandTexture_Ptr;

extern DRAW_TEXTURE_FUNCTION g_GL_Draw_Ptr;

extern DRAW_TEXTURE_ROTATED_FUNCTION g_GL_DrawRotated_Ptr;

extern DRAW_TEXTURE_MIRRORED_FUNCTION g_GL_DrawMirrored_Ptr;

extern DRAW_TEXTURE_SITTING_FUNCTION g_GL_DrawSitting_Ptr;

extern DRAW_TEXTURE_SHADOW_FUNCTION g_GL_DrawShadow_Ptr;

extern DRAW_TEXTURE_STRETCHED_FUNCTION g_GL_DrawStretched_Ptr;

extern DRAW_TEXTURE_RESIZEPIC_FUNCTION g_GL_DrawResizepic_Ptr;

#define g_GL_BindTexture16 (g_GL.*(g_GL_BindTexture16_Ptr))
#define g_GL_BindTexture32 (g_GL.*(g_GL_BindTexture32_Ptr))

#define g_GL_DrawLandTexture (g_GL.*(g_GL_DrawLandTexture_Ptr))
#define g_GL_Draw (g_GL.*(g_GL_Draw_Ptr))
#define g_GL_DrawRotated (g_GL.*(g_GL_DrawRotated_Ptr))
#define g_GL_DrawMirrored (g_GL.*(g_GL_DrawMirrored_Ptr))
#define g_GL_DrawSitting (g_GL.*(g_GL_DrawSitting_Ptr))
#define g_GL_DrawShadow (g_GL.*(g_GL_DrawShadow_Ptr))
#define g_GL_DrawStretched (g_GL.*(g_GL_DrawStretched_Ptr))
#define g_GL_DrawResizepic (g_GL.*(g_GL_DrawResizepic_Ptr))

class CGLEngine
{
public:
    HDC DC = 0;
    HGLRC RC = 0;
    GLuint OldTexture = 0;
    bool Drawing = false;
    bool CanUseFrameBuffer = false;
    bool CanUseBuffer = false;
    float SittingCharacterOffset = 8.0f;
    GLuint PositionBuffer = 0;
    bool IgnoreHitMap = false;

private:
    bool GLSetupPixelFormat();

    SCISSOR_LIST m_ScissorList;

public:
    CGLEngine();
    virtual ~CGLEngine();

    bool Install();

    void Uninstall();

    void UpdateRect();

    void BeginDraw();

    void EndDraw();

    void BeginStencil();

    void EndStencil();

    void ViewPortScaled(int x, int y, int width, int height);

    void ViewPort(int x, int y, int width, int height);

    void RestorePort();

    void PushScissor(int x, int y, int width, int height);
    void PushScissor(const WISP_GEOMETRY::CPoint2Di &position, int width, int height);
    void PushScissor(int x, int y, const WISP_GEOMETRY::CSize &size);
    void PushScissor(const WISP_GEOMETRY::CPoint2Di &position, const WISP_GEOMETRY::CSize &size);
    void PushScissor(const WISP_GEOMETRY::CRect &rect);

    void PopScissor();

    void ClearScissorList();

    inline void BindTexture(GLuint texture);

    void DrawLine(int x, int y, int targetX, int targetY);

    void DrawPolygone(int x, int y, int width, int height);

    void DrawCircle(float x, float y, float radius, int gradientMode = 0);

    void GL1_BindTexture16(CGLTexture &texture, int width, int height, pushort pixels);
    void GL1_BindTexture32(CGLTexture &texture, int width, int height, puint pixels);

    void GL1_DrawLandTexture(const CGLTexture &texture, int x, int y, CLandObject *land);

    void GL1_Draw(const CGLTexture &texture, int x, int y);

    void GL1_DrawRotated(const CGLTexture &texture, int x, int y, float angle);

    void GL1_DrawMirrored(const CGLTexture &texture, int x, int y, bool mirror);

    void GL1_DrawSitting(
        const CGLTexture &texture,
        int x,
        int y,
        bool mirror,
        float h3mod,
        float h6mod,
        float h9mod);

    void GL1_DrawShadow(const CGLTexture &texture, int x, int y, bool mirror);

    void GL1_DrawStretched(const CGLTexture &texture, int x, int y, int drawWidth, int drawHeight);

    void GL1_DrawResizepic(CGLTexture **th, int x, int y, int width, int height);

    void GL2_CreateArrays(CGLTexture &texture, int width, int height);

    void GL2_BindTexture16(CGLTexture &texture, int width, int height, pushort pixels);
    void GL2_BindTexture32(CGLTexture &texture, int width, int height, puint pixels);

    void GL2_DrawLandTexture(const CGLTexture &texture, int x, int y, CLandObject *land);

    void GL2_Draw(const CGLTexture &texture, int x, int y);

    void GL2_DrawRotated(const CGLTexture &texture, int x, int y, float angle);

    void GL2_DrawMirrored(const CGLTexture &texture, int x, int y, bool mirror);

    void GL2_DrawSitting(
        const CGLTexture &texture,
        int x,
        int y,
        bool mirror,
        float h3mod,
        float h6mod,
        float h9mod);

    void GL2_DrawShadow(const CGLTexture &texture, int x, int y, bool mirror);

    void GL2_DrawStretched(const CGLTexture &texture, int x, int y, int drawWidth, int drawHeight);

    void GL2_DrawResizepic(CGLTexture **th, int x, int y, int width, int height);
};

extern CGLEngine g_GL;

#endif
