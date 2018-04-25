

#ifndef CREATECHARACTERSCREEN_H
#define CREATECHARACTERSCREEN_H

class CCreateCharacterScreen : public CBaseScreen
{
protected:
    int m_StyleSelection = 0;

public:
    int GetStyleSelection() { return m_StyleSelection; };
    void SetStyleSelection(int val);

protected:
    int m_ColorSelection = 0;

public:
    int GetColorSelection() { return m_ColorSelection; };
    void SetColorSelection(int val);
    string Name = "";

private:
    CGumpScreenCreateCharacter m_CreateCharacterGump;

public:
    CCreateCharacterScreen();
    virtual ~CCreateCharacterScreen();

    static const uchar ID_SMOOTH_CCS_QUIT = 1;
    static const uchar ID_SMOOTH_CCS_GO_SCREEN_CHARACTER = 2;
    static const uchar ID_SMOOTH_CCS_GO_SCREEN_CONNECT = 3;

    void Init();

    void ProcessSmoothAction(uchar action = 0xFF);

    virtual void OnLeftMouseButtonDown();

    void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);

    void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};

extern CCreateCharacterScreen g_CreateCharacterScreen;

#endif
