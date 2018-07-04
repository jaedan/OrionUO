

#ifndef ORIONWINDOW_H
#define ORIONWINDOW_H

class COrionWindow : public WISP_WINDOW::CWindow
{
public:
    COrionWindow();
    virtual ~COrionWindow();

    static const int RENDER_TIMER_ID = 1;
    static const int UPDATE_TIMER_ID = 2;
    static const int FASTLOGIN_TIMER_ID = 3;

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
    virtual HRESULT OnRepaint(const WPARAM &wParam, const LPARAM &lParam);
    virtual void OnShow(bool show);
    virtual void OnMaximize();
    virtual void OnMinimize();
    virtual void OnSetText(const LPARAM &lParam);
    virtual void OnTimer(uint id);
    virtual void OnThreadedTimer(uint nowTime, WISP_THREADED_TIMER::CThreadedTimer *timer);
    virtual LRESULT OnUserMessages(int message, const WPARAM &wParam, const LPARAM &lParam);
};

extern COrionWindow g_OrionWindow;

#endif
