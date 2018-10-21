

#ifndef ORIONWINDOW_H
#define ORIONWINDOW_H

#include "Resource.h"
#include <uxtheme.h>
#include <vssym32.h>
#include <dwmapi.h>

class COrionWindow : public WISP_WINDOW::CWindow
{
public:
    COrionWindow();
    virtual ~COrionWindow();

    static const int RENDER_TIMER_ID = 1;
    static const int UPDATE_TIMER_ID = 2;
    static const int FASTLOGIN_TIMER_ID = 3;
	virtual void SetTitle(const string &text);
	virtual void SetTitle(const wstring &text);
	void OnMainScreen();

    void SetRenderTimerDelay(int delay);

    void EmulateOnLeftMouseButtonDown();

protected:
    virtual bool OnCreate();
    virtual void OnDestroy();
    virtual void OnResize(WISP_GEOMETRY::CSize &newSize);
    virtual void OnLeftMouseButtonDown();
    virtual void OnLeftMouseButtonUp();
    virtual bool OnLeftMouseButtonDoubleClick();
    virtual void OnRightMouseButtonDown();
    virtual void OnRightMouseButtonUp();
    virtual bool OnRightMouseButtonDoubleClick();
    virtual void OnMidMouseButtonDown();
    virtual void OnMidMouseButtonUp();
    virtual bool OnMidMouseButtonDoubleClick();
    virtual void OnMidMouseButtonScroll(bool up);
    virtual void OnXMouseButton(ushort button);
    virtual void OnDragging();
    virtual void OnActivate();
    virtual void OnDeactivate();
    virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
    virtual bool OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
    virtual bool OnKeyUp(const WPARAM &wParam, const LPARAM &lParam);
    virtual void OnRepaint();
    virtual void OnShow(bool show);
    virtual void OnMaximize();
    virtual void OnMinimize();
    virtual void OnSetText(const LPARAM &lParam);
    virtual void OnTimer(uint id);
    virtual void OnThreadedTimer(uint nowTime, WISP_THREADED_TIMER::CThreadedTimer *timer);
    virtual LRESULT OnUserMessages(int message, const WPARAM &wParam, const LPARAM &lParam);

private:
	typedef HTHEME(__stdcall *OPENTHEMEDATA)(HWND, LPCWSTR);
	typedef HRESULT(__stdcall *DRAWTHEMEBACKGROUND)(HTHEME, HDC, int, int, const RECT *, OPTIONAL const RECT *);
	typedef HRESULT(__stdcall *CLOSETHEMEDATA)(HTHEME);
	typedef HRESULT(__stdcall *GETTHEMESYSFONT)(HTHEME, int, OUT LOGFONTW*);
	typedef COLORREF(__stdcall *GETTHEMESYSCOLOR)(HTHEME, int);
	typedef HRESULT(__stdcall *GETTHEMEMETRIC)(HTHEME, HDC, int, int, int, int*);
	typedef HRESULT(__stdcall *DWMSETWINDOWATTRIBUTE)(HWND, DWORD, LPCVOID, DWORD);
#define Color16to32(c16) (((c16) & 0x7C00) >> 7) | (((c16) & 0x3E0) << 6) | (((c16) & 0x1F) << 19)
	struct UOItem
	{
		int RealWidth;
		int RealHeight;

		int Left;
		int Top;
		int Bottom;
		int Right;

		int GetWidth() const { return Right - Left; }
		int GetHeight() const { return Bottom - Top; }

		unsigned short **Data; // [ReadlWidth][RealHeight] 32bit color data

		int ItemID;
		UOItem *pNext;
	};
	struct ArtIdx
	{
		long FilePos;
		long Length;
		long Unused;
	};
	struct ArtHeader
	{
		long Unknown;
		short Width;
		short Height;
	};
	char* TitleBar;
	UOItem *ArtCache = NULL;
	unsigned short **Hues = NULL;
	int NumHues = 0;
	bool OldTitlePresent = true;
	bool TitleActive = false;
	bool WindowActive = false;
	bool curNCRP = true;
	int GetHex2(LPCSTR hex);
	int GetHex4(LPCSTR hex);
	void DoStat(HDC hDC, int v, int t, int l, int h, int w);
	int DrawStatBar(HDC hDC, RECT rect, int width, int status, int hp, int mn, int st);
	void DrawColorTitleBar(HTHEME hTheme, HDC hOutDC, bool maximized, LPCSTR str, int len, RECT orig);
	void RedrawTitleBar();
	int Round(float n);
	unsigned short* GetHue(int index);
	unsigned short ApplyHueToPixel(unsigned short *hue, unsigned short pix);
	UOItem* ReadUOItem(int item, int bh);
	UOItem* FindItem(int item);
	COLORREF COrionWindow::Brightness(int shift, COLORREF c);
	int DrawUOItem(HDC hDC, RECT rect, int item, int hueIdx);
	void FreeItem(UOItem *node);
	void FreeArt();
};

extern COrionWindow g_OrionWindow;

#endif
