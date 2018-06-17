

#ifndef MOUSEMANAGER_H
#define MOUSEMANAGER_H

extern ushort g_CursorData[2][16];

class CMouseManager : public WISP_MOUSE::CMouse
{
private:
    int m_CursorOffset[2][16];

public:
    CMouseManager()
        : WISP_MOUSE::CMouse()
    {
        memset(&m_CursorOffset[0][0], 0, sizeof(m_CursorOffset));
    }
    ~CMouseManager() {}

    bool LoadCursorTextures();

    int Sgn(int val);

    int GetFacing(int x1, int y1, int to_x, int to_y, int current_facing);

    ushort GetGameCursor();

    void ProcessWalking();

    void Draw(ushort id);
};

extern CMouseManager g_MouseManager;

#endif
