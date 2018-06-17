

#ifndef MAINSCREEN_H
#define MAINSCREEN_H

class CMainScreen : public CBaseScreen
{
private:
    enum
    {
        MSCC_ACTID = 1,
        MSCC_ACTPWD,
        MSCC_REMEMBERPWD,
        MSCC_AUTOLOGIN,
        MSCC_SMOOTHMONITOR,
        MSCC_THE_ABYSS,
        MSCC_ASMUT,
        MSCC_CUSTOM_PATH,
        MSCC_COUNT,
    };

    string CryptPW(const char *buf, int len);

    string DecryptPW(const char *buf, int len);

    int GetConfigKeyCode(const string &key);

    CGumpScreenMain m_MainGump;

public:
    CMainScreen();
    virtual ~CMainScreen();

    static const uchar ID_SMOOTH_MS_QUIT = 1;
    static const uchar ID_SMOOTH_MS_CONNECT = 2;

    CEntryText *m_Account{ NULL };
    CEntryText *m_Password{ NULL };
    CGUICheckbox *m_SavePassword{ NULL };
    CGUICheckbox *m_AutoLogin{ NULL };

    void SetAccounting(const string &account, const string &password);

    void Paste();

    void ProcessSmoothAction(uchar action = 0xFF);

    void LoadGlobalConfig();

    void LoadCustomPath();

    void SaveGlobalConfig();

    void Init();

    virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
    virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};

extern CMainScreen g_MainScreen;

#endif
