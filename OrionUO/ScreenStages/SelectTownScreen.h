

#ifndef SELECTTOWNSCREEN_H
#define SELECTTOWNSCREEN_H

class CSelectTownScreen : public CBaseScreen
{
private:
    CGumpScreenSelectTown m_SelectTownGump;

public:
    CSelectTownScreen();
    ~CSelectTownScreen();

    static const uchar ID_SMOOTH_STS_QUIT = 1;
    static const uchar ID_SMOOTH_STS_GO_SCREEN_CHARACTER = 2;
    static const uchar ID_SMOOTH_STS_GO_SCREEN_GAME_CONNECT = 3;

    CCityItem *m_City{ NULL };

    void Init();

    void ProcessSmoothAction(uchar action = 0xFF);
};

extern CSelectTownScreen g_SelectTownScreen;

#endif
