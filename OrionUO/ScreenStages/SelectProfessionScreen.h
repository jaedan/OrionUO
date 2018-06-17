

#ifndef SELECTPROFESSIONSCREEN_H
#define SELECTPROFESSIONSCREEN_H

class CSelectProfessionScreen : public CBaseScreen
{
protected:
    int m_SkillSelection = 0;

public:
    int GetSkillSelection() { return m_SkillSelection; };
    void SetSkillSelection(int val);

private:
    CGumpScreenSelectProfession m_SelectProfessionGump;

public:
    CSelectProfessionScreen();
    virtual ~CSelectProfessionScreen();

    static const uchar ID_SMOOTH_SPS_QUIT = 1;
    static const uchar ID_SMOOTH_SPS_GO_SCREEN_CHARACTER = 2;
    static const uchar ID_SMOOTH_SPS_GO_SCREEN_GAME_CONNECT = 3;
    static const uchar ID_SMOOTH_SPS_GO_SCREEN_CREATE = 4;

    void Init();

    void ProcessSmoothAction(uchar action = 0xFF);
};

extern CSelectProfessionScreen g_SelectProfessionScreen;

#endif
