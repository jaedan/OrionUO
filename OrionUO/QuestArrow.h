

#ifndef QUESTARROW_H
#define QUESTARROW_H

class CQuestArrow
{
public:
    uint Timer = 0;
    ushort X = 0;
    ushort Y = 0;
    bool Enabled = false;

    CQuestArrow();
    ~CQuestArrow();

    static const ushort m_Gump = 0x1194;

    void Draw();
};

extern CQuestArrow g_QuestArrow;

#endif
