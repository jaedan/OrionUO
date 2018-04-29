

#ifndef MAINSCREEN_H
#define MAINSCREEN_H

class CMainScreen : public CBaseScreen
{
private:
    string CryptPW(const char *buf, int len);

    string DecryptPW(const char *buf, int len);

    CGumpScreenMain m_MainGump;

public:
    CMainScreen();
    virtual ~CMainScreen();

    static const uchar ID_SMOOTH_MS_QUIT = 1;
    static const uchar ID_SMOOTH_MS_CONNECT = 2;

    CEntryText *m_Account = nullptr;
    CEntryText *m_Password = nullptr;
    CGUICheckbox *m_SavePassword = nullptr;
    CGUICheckbox *m_AutoLogin = nullptr;

    void SetAccountName(const string &account);
    void SetPassword(const string &password);
    void SetEncryptedPassword(const string &password);

    string GetEncryptedPassword();

    void Paste();

    void ProcessSmoothAction(uchar action = 0xFF);

    void Init();

    virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
    virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};

extern CMainScreen g_MainScreen;

#endif
