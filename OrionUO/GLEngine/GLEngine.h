/***********************************************************************************
**
** GLEngine.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GLENGINE_H
#define GLENGINE_H
//----------------------------------------------------------------------------------
typedef deque<WISP_GEOMETRY::CRect> SCISSOR_LIST;
//----------------------------------------------------------------------------------
class CGLEngine
{
public:
	HDC DC = 0;
	HGLRC RC = 0;
	GLuint OldTexture = 0;
	bool Drawing = false;
	bool CanUseFrameBuffer = false;
	float SittingCharacterOffset = 8.0f;
	bool IgnoreHitMap = false;

private:
	bool GLSetupPixelFormat();

	SCISSOR_LIST m_ScissorList;

public:
	CGLEngine();
	virtual ~CGLEngine();
	
	//Инициализация OpenGL
	bool Install();

	//Завершение работы с OpenGL
	void Uninstall();

	//Обновление области вывода
	void UpdateRect();

	//Очистить экран и начать рисование
	void BeginDraw();

	//Завершить рисование и вывести все нарисованное на экран
	void EndDraw();

	//Включить буфер трафарета
	void BeginStencil();

	//Выключить буфер трафарета
	void EndStencil();

	//Указать область рисования
	void ViewPortScaled(int x, int y, int width, int height);

	//Указать область рисования
	void ViewPort(int x, int y, int width, int height);

	//Восстановить стандартную область рисования
	void RestorePort();

	//Указать область рисования (ножницами, сохраняет мартицу)
	void PushScissor(int x, int y, int width, int height);
	void PushScissor(const WISP_GEOMETRY::CPoint2Di &position, int width, int height);
	void PushScissor(int x, int y, const WISP_GEOMETRY::CSize &size);
	void PushScissor(const WISP_GEOMETRY::CPoint2Di &position, const WISP_GEOMETRY::CSize &size);
	void PushScissor(const WISP_GEOMETRY::CRect &rect);

	void PopScissor();

	void ClearScissorList();

	inline void BindTexture(GLuint texture);

	//Нарисовать линию
	void DrawLine(int x, int y, int targetX, int targetY);

	//Нарисовать прямоугольник
	void DrawPolygone(int x, int y, int width, int height);

	//Нарисовать круг
	void DrawCircle(float x, float y, float radius, int gradientMode = 0);

	void BindTexture16(CGLTexture &texture, int width, int height, pushort pixels);
	void BindTexture32(CGLTexture &texture, int width, int height, puint pixels);

	void DrawLandTexture(const CGLTexture &texture, int x, int y, class CLandObject *land);
	void Draw(const CGLTexture &texture, int x, int y);
	void DrawRotated(const CGLTexture &texture, int x, int y, float angle);
	void DrawMirrored(const CGLTexture &texture, int x, int y, bool mirror);
	void DrawSitting(const CGLTexture &texture, int x, int y, bool mirror, float h3mod, float h6mod, float h9mod);
	void DrawShadow(const CGLTexture &texture, int x, int y, bool mirror);
	void DrawStretched(const CGLTexture &texture, int x, int y, int drawWidth, int drawHeight);
	void DrawResizepic(CGLTexture **th, int x, int y, int width, int height);
};
//---------------------------------------------------------------------------
extern CGLEngine g_GL;
//---------------------------------------------------------------------------
#endif