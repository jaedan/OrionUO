

#ifndef SERVERSCREEN_H
#define SERVERSCREEN_H

class CServerScreen : public CBaseScreen
{
public:
    int SelectionServerTempValue = 0;

private:
    CGumpScreenServer m_ServerGump;

public:
    CServerScreen();
    virtual ~CServerScreen();

    static const uchar ID_SMOOTH_SS_QUIT = 1;
    static const uchar ID_SMOOTH_SS_SELECT_SERVER = 2;
    static const uchar ID_SMOOTH_SS_GO_SCREEN_MAIN = 3;

    void Init();

    void ProcessSmoothAction(uchar action = 0xFF);

    virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};

extern CServerScreen g_ServerScreen;

#endif
