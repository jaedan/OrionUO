

#ifndef GUMPSTATUSBAR_H
#define GUMPSTATUSBAR_H
enum StatusBarToolTips
{
    ID_GSB_MINIMIZE = 1,
    ID_GSB_TEXT_FIELD,
    ID_GSB_BUTTON_HEAL_1,
    ID_GSB_BUTTON_HEAL_2,
    ID_GSB_BUTTON_REMOVE_FROM_GROUP,
    ID_GSB_BUFF_GUMP,
    ID_GSB_BUFF_LOCKER_STR,
    ID_GSB_BUFF_LOCKER_DEX,
    ID_GSB_BUFF_LOCKER_INT,
    ID_GSB_TEXT_STR,
    ID_GSB_TEXT_DEX,
    ID_GSB_TEXT_INT,
    ID_GSB_TEXT_SEX,
    ID_GSB_TEXT_ARMOR,
    ID_GSB_TEXT_HITS,
    ID_GSB_TEXT_STAM,
    ID_GSB_TEXT_MANA,
    ID_GSB_TEXT_WEIGHT,
    ID_GSB_TEXT_SATIETY,
    ID_GSB_TEXT_MURDER_COUNT,
    ID_GSB_TEXT_DAMAGE,
    ID_GSB_TEXT_GOLD,
    ID_GSB_TEXT_FOLLOWERS,
    ID_GSB_TEXT_MURDER_COUNT_DECAY,
    ID_GSB_TEXT_CRIMINAL_TIMER,
    ID_GSB_TEXT_PVP_TIMER,
    ID_GSB_TEXT_BANDAGE_TIMER,
    ID_GSB_TEXT_MAX,
};

class CGumpStatusbar : public CGump
{
protected:
    void SendRenameRequest();

    virtual void CalculateGumpState();

    bool m_WantFullUpdateContent{ false };

    CGUIButton *m_StatusbarUnlocker{ NULL };
    CGUIGumppic *m_Body{ NULL };
    CGUIGumppic *m_HitsBody{ NULL };
    CGUITextEntry *m_Entry{ NULL };

    string m_Name = string("");

public:
    CGumpStatusbar(uint serial, short x, short y, bool minimized);
    virtual ~CGumpStatusbar();

    static const int ID_GSB_LOCK_MOVING = 10;

    static int m_StatusbarDefaultWidth;
    static int m_StatusbarDefaultHeight;

    CGumpStatusbar *m_GroupNext{ NULL };
    CGumpStatusbar *m_GroupPrev{ NULL };

    CGumpStatusbar *GetTopStatusbar();
    CGumpStatusbar *GetNearStatusbar(int &x, int &y);
    bool GetStatusbarGroupOffset(int &x, int &y);
    void UpdateGroup(int x, int y);
    void AddStatusbar(CGumpStatusbar *bar);
    void RemoveFromGroup();

    bool InGroup() { return (m_GroupNext != NULL || m_GroupPrev != NULL); }

    virtual void InitToolTip();

    virtual void PrepareContent();

    virtual void UpdateContent();

    GUMP_BUTTON_EVENT_H;

    virtual void OnLeftMouseButtonDown();
    virtual bool OnLeftMouseButtonDoubleClick();
    virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
    virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};

#endif
