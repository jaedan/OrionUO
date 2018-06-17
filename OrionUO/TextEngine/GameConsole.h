

#ifndef GAMECONSOLE_H
#define GAMECONSOLE_H

class CGameConsole : public CEntryText
{
private:
    GAME_CONSOLE_TEXT_TYPE m_Type{ GCTT_NORMAL };

    wstring m_ConsoleStack[MAX_CONSOLE_STACK_SIZE];

    int m_ConsoleStackCount{ 0 };

    int m_ConsoleSelectedIndex{ 0 };

    bool m_PositionChanged{ false };

public:
    CGameConsole();
    virtual ~CGameConsole();

    void Send();

    static void Send(wstring text, ushort defaultColor = 0);

    static wstring
    IsSystemCommand(const wchar_t *text, size_t &len, int &member, GAME_CONSOLE_TEXT_TYPE &type);

    void
    DrawW(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);

    void SaveConsoleMessage();

    void ChangeConsoleMessage(bool next);

    void ClearStack();
};

static const wstring g_ConsolePrefix[] = { L"",   L"! ", L"; ",  L": ", L".",
                                           L"? ", L"/ ", L"\\ ", L"| " };

extern CGameConsole g_GameConsole;

#endif
