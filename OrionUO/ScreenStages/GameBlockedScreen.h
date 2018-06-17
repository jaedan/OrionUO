

#ifndef GAMEBLOCKEDSCREEN_H
#define GAMEBLOCKEDSCREEN_H

class CGameBlockedScreen : public CBaseScreen
{
public:
    uchar Code = 0;

    CGump m_GameBlockedScreenGump;

    CGameBlockedScreen();
    ~CGameBlockedScreen();

    virtual void Init();

    virtual void Render(bool mode);

    virtual void OnLeftMouseButtonDown();
    virtual void OnLeftMouseButtonUp();
    virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
    virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};

extern CGameBlockedScreen g_GameBlockedScreen;

#endif
