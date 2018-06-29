

#include "stdafx.h"

CGLEngine g_GL;

CGLEngine::CGLEngine()
{
}

CGLEngine::~CGLEngine()
{
    WISPFUN_DEBUG("c29_f1");
    Uninstall();
}

bool CGLEngine::GLSetupPixelFormat()
{
    WISPFUN_DEBUG("c29_f2");
    PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR),
                                  1,
                                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
                                  PFD_TYPE_RGBA,
                                  16,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  16,
                                  1,
                                  0,
                                  PFD_MAIN_PLANE,
                                  0,
                                  0,
                                  0,
                                  0 };

    int pixelformat = ChoosePixelFormat(DC, &pfd);

    if (!pixelformat)
    {
        MessageBox(NULL, L"ChoosePixelFormat failed", L"Error", MB_OK);
        return false;
    }

    if (!SetPixelFormat(DC, pixelformat, &pfd))
    {
        MessageBox(NULL, L"SetPixelFormat failed", L"Error", MB_OK);
        return false;
    }

    return true;
}

bool CGLEngine::Install()
{
    WISPFUN_DEBUG("c29_f3");
    OldTexture = -1;

    DC = ::GetDC(g_OrionWindow.Handle);
    if (!GLSetupPixelFormat())
        return false;

    RC = wglCreateContext(DC);
    if (!RC)
        return false;

    if (!wglMakeCurrent(DC, RC))
        return false;

    int glewInitResult = glewInit();
    LOG("glewInit() = %i rt=%i fb=%i v(%s) (shader: %i)\n",
        glewInitResult,
        WGLEW_ARB_render_texture,
        GL_ARB_framebuffer_object,
        glGetString(GL_VERSION),
        GL_ARB_shader_objects);

    if (!glewInitResult)
    {
        CanUseFrameBuffer =
            (GL_ARB_framebuffer_object && glBindFramebuffer && glDeleteFramebuffers &&
             glFramebufferTexture2D && glGenFramebuffers);
    }
    else
        return false;

    LOG("g_UseFrameBuffer = %i\n", CanUseFrameBuffer);

    if (!CanUseFrameBuffer && g_ShowWarnings)
        g_OrionWindow.ShowMessage("Your graphics card does not support Frame Buffers!", "Warning!");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glDisable(GL_DITHER);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    glEnable(GL_TEXTURE_2D);

    typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int interval);
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
        (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

    if (wglSwapIntervalEXT != NULL)
        wglSwapIntervalEXT(0);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glClearStencil(0);
    glStencilMask(1);

    glEnable(GL_LIGHT0);

    GLfloat lightPosition[] = { -1.0f, 0.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    SetAmbientLightIntensity(0.5f);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

    ViewPort(0, 0, g_OrionWindow.GetSize().Width, g_OrionWindow.GetSize().Height);

    return true;
}

void CGLEngine::Uninstall()
{
    WISPFUN_DEBUG("c29_f4");
    DC = 0;
    wglMakeCurrent(NULL, NULL);

    if (RC != 0)
    {
        wglDeleteContext(RC);
        RC = 0;
    }
}

void CGLEngine::SetAmbientLightIntensity(float val)
{
    GLfloat lightAmbient[] = { val, val, val, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
}

void CGLEngine::UpdateRect()
{
    WISPFUN_DEBUG("c29_f5");
    RECT cr;
    GetClientRect(g_OrionWindow.Handle, &cr);
    int width = cr.right - cr.left;
    int height = cr.bottom - cr.top;

    ViewPort(0, 0, width, height);

    g_GumpManager.RedrawAll();
}

void CGLEngine::BindTexture16(CGLTexture &texture, int width, int height, pushort pixels)
{
    WISPFUN_DEBUG("c29_f6");
    GLuint tex = 0;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB5_A1,
        width,
        height,
        0,
        GL_BGRA,
        GL_UNSIGNED_SHORT_1_5_5_5_REV,
        pixels);

    texture.Width = width;
    texture.Height = height;
    texture.Texture = tex;

    if (IgnoreHitMap)
        return;

    HIT_MAP_TYPE &hitMap = texture.m_HitMap;
    hitMap.resize(width * height);
    int pos = 0;

    IFOR (y, 0, height)
    {
        IFOR (x, 0, width)
        {
            hitMap[pos] = (pixels[pos] != 0);
            pos++;
        }
    }
}

void CGLEngine::BindTexture32(CGLTexture &texture, int width, int height, puint pixels)
{
    WISPFUN_DEBUG("c29_f7");
    GLuint tex = 0;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA4, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, pixels);

    texture.Width = width;
    texture.Height = height;
    texture.Texture = tex;

    if (IgnoreHitMap)
        return;

    HIT_MAP_TYPE &hitMap = texture.m_HitMap;
    hitMap.resize(width * height);
    int pos = 0;

    IFOR (y, 0, height)
    {
        IFOR (x, 0, width)
        {
            hitMap[pos] = (pixels[pos] != 0);
            pos++;
        }
    }
}

void CGLEngine::BeginDraw()
{
    WISPFUN_DEBUG("c29_f11");
    Drawing = true;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
}

void CGLEngine::EndDraw()
{
    WISPFUN_DEBUG("c29_f12");
    Drawing = false;

    glDisable(GL_ALPHA_TEST);

    SwapBuffers(DC);
}

void CGLEngine::BeginStencil()
{
    WISPFUN_DEBUG("c29_f13");
    glEnable(GL_STENCIL_TEST);

    glColorMask(false, false, false, false);

    glStencilFunc(GL_ALWAYS, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void CGLEngine::EndStencil()
{
    WISPFUN_DEBUG("c29_f14");
    glColorMask(true, true, true, true);

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_NOTEQUAL, 1, 1);

    glDisable(GL_STENCIL_TEST);
}

void CGLEngine::ViewPortScaled(int x, int y, int width, int height)
{
    WISPFUN_DEBUG("c29_f15");
    glViewport(x, g_OrionWindow.GetSize().Height - y - height, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble left = (GLdouble)x;
    GLdouble right = (GLdouble)(width + x);
    GLdouble top = (GLdouble)y;
    GLdouble bottom = (GLdouble)(height + y);

    GLdouble newRight = right * g_GlobalScale;
    GLdouble newBottom = bottom * g_GlobalScale;

    left = (left * g_GlobalScale) - (newRight - right);
    top = (top * g_GlobalScale) - (newBottom - bottom);

    glOrtho(left, newRight, newBottom, top, -150.0, 150.0);
    glMatrixMode(GL_MODELVIEW);
}

void CGLEngine::ViewPort(int x, int y, int width, int height)
{
    WISPFUN_DEBUG("c29_f16");
    glViewport(x, g_OrionWindow.GetSize().Height - y - height, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(x, width + x, height + y, y, -150.0, 150.0);
    glMatrixMode(GL_MODELVIEW);
}

void CGLEngine::RestorePort()
{
    WISPFUN_DEBUG("c29_f17");
    glViewport(0, 0, g_OrionWindow.GetSize().Width, g_OrionWindow.GetSize().Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, g_OrionWindow.GetSize().Width, g_OrionWindow.GetSize().Height, 0.0, -150.0, 150.0);
    glMatrixMode(GL_MODELVIEW);
}

void CGLEngine::PushScissor(int x, int y, int width, int height)
{
    WISPFUN_DEBUG("c29_f18");
    PushScissor(WISP_GEOMETRY::CRect(x, y, width, height));
}

void CGLEngine::PushScissor(const WISP_GEOMETRY::CPoint2Di &position, int width, int height)
{
    WISPFUN_DEBUG("c29_f19");
    PushScissor(WISP_GEOMETRY::CRect(position, width, height));
}

void CGLEngine::PushScissor(int x, int y, const WISP_GEOMETRY::CSize &size)
{
    WISPFUN_DEBUG("c29_f20");
    PushScissor(WISP_GEOMETRY::CRect(x, y, size));
}

void CGLEngine::PushScissor(
    const WISP_GEOMETRY::CPoint2Di &position, const WISP_GEOMETRY::CSize &size)
{
    WISPFUN_DEBUG("c29_f21");
    PushScissor(WISP_GEOMETRY::CRect(position, size));
}

void CGLEngine::PushScissor(const WISP_GEOMETRY::CRect &rect)
{
    WISPFUN_DEBUG("c29_f22");
    m_ScissorList.push_back(rect);

    glEnable(GL_SCISSOR_TEST);

    glScissor(rect.Position.X, rect.Position.Y, rect.Size.Width, rect.Size.Height);
}

void CGLEngine::PopScissor()
{
    WISPFUN_DEBUG("c29_f23");
    if (!m_ScissorList.empty())
        m_ScissorList.pop_back();

    if (m_ScissorList.empty())
        glDisable(GL_SCISSOR_TEST);
    else
    {
        WISP_GEOMETRY::CRect &rect = m_ScissorList.back();
        glScissor(rect.Position.X, rect.Position.Y, rect.Size.Width, rect.Size.Height);
    }
}

void CGLEngine::ClearScissorList()
{
    WISPFUN_DEBUG("c29_f24");
    m_ScissorList.clear();

    glDisable(GL_SCISSOR_TEST);
}

inline void CGLEngine::BindTexture(GLuint texture)
{
    WISPFUN_DEBUG("c29_f25");
    if (OldTexture != texture)
    {
        OldTexture = texture;
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}

void CGLEngine::DrawLine(int x, int y, int targetX, int targetY)
{
    WISPFUN_DEBUG("c29_f26");
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_LINES);
    glVertex2i(x, y);
    glVertex2i(targetX, targetY);
    glEnd();

    glEnable(GL_TEXTURE_2D);
}

void CGLEngine::DrawPolygone(int x, int y, int width, int height)
{
    WISPFUN_DEBUG("c29_f27");
    glDisable(GL_TEXTURE_2D);

    glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2i(0, height);
    glVertex2i(width, height);
    glVertex2i(0, 0);
    glVertex2i(width, 0);
    glEnd();

    glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);

    glEnable(GL_TEXTURE_2D);
}

void CGLEngine::DrawCircle(float x, float y, float radius, int gradientMode)
{
    WISPFUN_DEBUG("c29_f28");
    glDisable(GL_TEXTURE_2D);

    glTranslatef(x, y, 0.0f);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2i(0, 0);

    if (gradientMode)
        glColor4f(0.0f, 0.0f, 0.0f, 0.0f);

    float pi = (float)M_PI * 2.0f;

    for (int i = 0; i <= 360; i++)
    {
        float a = (i / 180.0f) * pi;
        glVertex2f(cos(a) * radius, sin(a) * radius);
    }

    glEnd();

    glTranslatef(-x, -y, 0.0f);

    glEnable(GL_TEXTURE_2D);
}

void CGLEngine::DrawLandTexture(const CGLTexture &texture, int x, int y, CLandObject *land)
{
    WISPFUN_DEBUG("c29_f29");
    BindTexture(texture.Texture);

    float translateX = x - 22.0f;
    float translateY = y - 22.0f;

    const RECT &rc = land->m_Rect;
    CVector *normals = land->m_Normals;

    glTranslatef(translateX, translateY, 0.0f);

    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f((GLfloat)normals[0].X, (GLfloat)normals[0].Y, (GLfloat)normals[0].Z);
    glTexCoord2i(0, 0);
    glVertex2i(22, -rc.left);

    glNormal3f((GLfloat)normals[3].X, (GLfloat)normals[3].Y, (GLfloat)normals[3].Z);
    glTexCoord2i(0, 1);
    glVertex2i(0, 22 - rc.top);

    glNormal3f((GLfloat)normals[1].X, (GLfloat)normals[1].Y, (GLfloat)normals[1].Z);
    glTexCoord2i(1, 0);
    glVertex2i(44, 22 - rc.bottom);

    glNormal3f((GLfloat)normals[2].X, (GLfloat)normals[2].Y, (GLfloat)normals[2].Z);
    glTexCoord2i(1, 1);
    glVertex2i(22, 44 - rc.right);
    glEnd();

    glTranslatef(-translateX, -translateY, 0.0f);
}

void CGLEngine::Draw(const CGLTexture &texture, int x, int y)
{
    WISPFUN_DEBUG("c29_f30");
    BindTexture(texture.Texture);

    int width = texture.Width;
    int height = texture.Height;

    glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2i(0, 1);
    glVertex2i(0, height);
    glTexCoord2i(1, 1);
    glVertex2i(width, height);
    glTexCoord2i(0, 0);
    glVertex2i(0, 0);
    glTexCoord2i(1, 0);
    glVertex2i(width, 0);
    glEnd();

    glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}

void CGLEngine::DrawRotated(const CGLTexture &texture, int x, int y, float angle)
{
    WISPFUN_DEBUG("c29_f31");
    BindTexture(texture.Texture);

    int width = texture.Width;
    int height = texture.Height;

    GLfloat translateY = (GLfloat)(y - height);

    glTranslatef((GLfloat)x, translateY, 0.0f);

    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2i(0, 1);
    glVertex2i(0, height);
    glTexCoord2i(1, 1);
    glVertex2i(width, height);
    glTexCoord2i(0, 0);
    glVertex2i(0, 0);
    glTexCoord2i(1, 0);
    glVertex2i(width, 0);
    glEnd();

    glRotatef(angle, 0.0f, 0.0f, -1.0f);
    glTranslatef((GLfloat)-x, -translateY, 0.0f);
}

void CGLEngine::DrawMirrored(const CGLTexture &texture, int x, int y, bool mirror)
{
    WISPFUN_DEBUG("c29_f32");
    BindTexture(texture.Texture);

    int width = texture.Width;
    int height = texture.Height;

    glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

    glBegin(GL_TRIANGLE_STRIP);

    if (mirror)
    {
        glTexCoord2i(0, 1);
        glVertex2i(width, height);
        glTexCoord2i(1, 1);
        glVertex2i(0, height);
        glTexCoord2i(0, 0);
        glVertex2i(width, 0);
        glTexCoord2i(1, 0);
        glVertex2i(0, 0);
    }
    else
    {
        glTexCoord2i(0, 1);
        glVertex2i(0, height);
        glTexCoord2i(1, 1);
        glVertex2i(width, height);
        glTexCoord2i(0, 0);
        glVertex2i(0, 0);
        glTexCoord2i(1, 0);
        glVertex2i(width, 0);
    }

    glEnd();

    glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}

void CGLEngine::DrawSitting(
    const CGLTexture &texture, int x, int y, bool mirror, float h3mod, float h6mod, float h9mod)
{
    WISPFUN_DEBUG("c29_f33");
    BindTexture(texture.Texture);

    glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

    float width = (float)texture.Width;
    float height = (float)texture.Height;

    float h03 = height * h3mod;
    float h06 = height * h6mod;
    float h09 = height * h9mod;

    float widthOffset = (float)(width + SittingCharacterOffset);
    glBegin(GL_TRIANGLE_STRIP);

    if (mirror)
    {
        if (h3mod)
        {
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(width, 0);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(0, 0);
            glTexCoord2f(0.0f, h3mod);
            glVertex2f(width, h03);
            glTexCoord2f(1.0f, h3mod);
            glVertex2f(0, h03);
        }

        if (h6mod)
        {
            if (!h3mod)
            {
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(width, 0);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(0, 0);
            }

            glTexCoord2f(0.0f, h6mod);
            glVertex2f(widthOffset, h06);
            glTexCoord2f(1.0f, h6mod);
            glVertex2f(SittingCharacterOffset, h06);
        }

        if (h9mod)
        {
            if (!h6mod)
            {
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(widthOffset, 0);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(SittingCharacterOffset, 0);
            }

            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(widthOffset, h09);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(SittingCharacterOffset, h09);
        }
    }
    else
    {
        if (h3mod)
        {
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(SittingCharacterOffset, 0);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(widthOffset, 0);
            glTexCoord2f(0.0f, h3mod);
            glVertex2f(SittingCharacterOffset, h03);
            glTexCoord2f(1.0f, h3mod);
            glVertex2f(widthOffset, h03);
        }

        if (h6mod)
        {
            if (!h3mod)
            {
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(SittingCharacterOffset, 0);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(width + SittingCharacterOffset, 0);
            }

            glTexCoord2f(0.0f, h6mod);
            glVertex2f(0, h06);
            glTexCoord2f(1.0f, h6mod);
            glVertex2f(width, h06);
        }

        if (h9mod)
        {
            if (!h6mod)
            {
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(0, 0);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(width, 0);
            }

            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(0, h09);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(width, h09);
        }
    }

    glEnd();

    glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}

void CGLEngine::DrawShadow(const CGLTexture &texture, int x, int y, bool mirror)
{
    WISPFUN_DEBUG("c29_f34");
    BindTexture(texture.Texture);

    float width = (float)texture.Width;
    float height = texture.Height / 2.0f;

    GLfloat translateY = (GLfloat)(y + height * 0.75);

    glTranslatef((GLfloat)x, translateY, 0.0f);

    glBegin(GL_TRIANGLE_STRIP);

    float ratio = height / width;

    if (mirror)
    {
        glTexCoord2f(0, 1);
        glVertex2f(width, height);
        glTexCoord2f(1, 1);
        glVertex2f(0, height);
        glTexCoord2f(0, 0);
        glVertex2f(width * (ratio + 1.0f), 0);
        glTexCoord2f(1, 0);
        glVertex2f(width * ratio, 0);
    }
    else
    {
        glTexCoord2f(0, 1);
        glVertex2f(0, height);
        glTexCoord2f(1, 1);
        glVertex2f(width, height);
        glTexCoord2f(0, 0);
        glVertex2f(width * ratio, 0);
        glTexCoord2f(1, 0);
        glVertex2f(width * (ratio + 1.0f), 0);
    }

    glEnd();

    glTranslatef((GLfloat)-x, -translateY, 0.0f);
}

void CGLEngine::DrawStretched(
    const CGLTexture &texture, int x, int y, int drawWidth, int drawHeight)
{
    WISPFUN_DEBUG("c29_f35");
    BindTexture(texture.Texture);

    int width = texture.Width;
    int height = texture.Height;

    glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

    float drawCountX = drawWidth / (float)width;
    float drawCountY = drawHeight / (float)height;

    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(0.0f, drawCountY);
    glVertex2i(0, drawHeight);
    glTexCoord2f(drawCountX, drawCountY);
    glVertex2i(drawWidth, drawHeight);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(0, 0);
    glTexCoord2f(drawCountX, 0.0f);
    glVertex2i(drawWidth, 0);
    glEnd();

    glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}

void CGLEngine::DrawResizepic(CGLTexture **th, int x, int y, int width, int height)
{
    WISPFUN_DEBUG("c29_f36");

    int offsetTop = max(th[0]->Height, th[2]->Height) - th[1]->Height;
    int offsetBottom = max(th[5]->Height, th[7]->Height) - th[6]->Height;
    int offsetLeft = max(th[0]->Width, th[5]->Width) - th[3]->Width;
    int offsetRight = max(th[2]->Width, th[7]->Width) - th[4]->Width;

    IFOR (i, 0, 9)
    {
        BindTexture(th[i]->Texture);

        int drawWidth = th[i]->Width;
        int drawHeight = th[i]->Height;
        float drawCountX = 1.0f;
        float drawCountY = 1.0f;
        int drawX = x;
        int drawY = y;

        switch (i)
        {
            case 1:
            {
                drawX += th[0]->Width;

                drawWidth = width - th[0]->Width - th[2]->Width;

                drawCountX = drawWidth / (float)th[i]->Width;

                break;
            }
            case 2:
            {
                drawX += width - drawWidth;
                drawY += offsetTop;

                break;
            }
            case 3:
            {
                drawY += th[0]->Height;
                drawX += offsetLeft;

                drawHeight = height - th[0]->Height - th[5]->Height;

                drawCountY = drawHeight / (float)th[i]->Height;

                break;
            }
            case 4:
            {
                drawX += width - drawWidth - offsetRight;
                drawY += th[2]->Height;

                drawHeight = height - th[2]->Height - th[7]->Height;

                drawCountY = drawHeight / (float)th[i]->Height;

                break;
            }
            case 5:
            {
                drawY += height - drawHeight;

                break;
            }
            case 6:
            {
                drawX += th[5]->Width;
                drawY += height - drawHeight - offsetBottom;

                drawWidth = width - th[5]->Width - th[7]->Width;

                drawCountX = drawWidth / (float)th[i]->Width;

                break;
            }
            case 7:
            {
                drawX += width - drawWidth;
                drawY += height - drawHeight;

                break;
            }
            case 8:
            {
                drawX += th[0]->Width;
                drawY += th[0]->Height;

                drawWidth = width - th[0]->Width - th[2]->Width;

                drawHeight = height - th[2]->Height - th[7]->Height;

                drawCountX = drawWidth / (float)th[i]->Width;
                drawCountY = drawHeight / (float)th[i]->Height;

                break;
            }
            default:
                break;
        }

        if (drawWidth < 1 || drawHeight < 1)
            continue;

        glTranslatef((GLfloat)drawX, (GLfloat)drawY, 0.0f);

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0.0f, drawCountY);
        glVertex2i(0, drawHeight);
        glTexCoord2f(drawCountX, drawCountY);
        glVertex2i(drawWidth, drawHeight);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(0, 0);
        glTexCoord2f(drawCountX, 0.0f);
        glVertex2i(drawWidth, 0);
        glEnd();

        glTranslatef((GLfloat)-drawX, (GLfloat)-drawY, 0.0f);
    }
}
