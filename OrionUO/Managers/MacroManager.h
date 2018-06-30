

#ifndef MACROMANAGER_H
#define MACROMANAGER_H

class CMacroManager : public CBaseQueue
{
public:
    bool WaitingBandageTarget = false;

    uint WaitForTargetTimer = 0;

private:
    uint m_NextTimer{ 0 };

    static uchar m_SkillIndexTable[24];

    void ProcessSubMenu();

    ushort ConvertStringToKeyCode(const STRING_LIST &strings);

public:
    CMacroManager();
    virtual ~CMacroManager();

    class CMacro *FindMacro(ushort key, bool alt, bool ctrl, bool shift);

    bool Convert(const string &path);

    bool Load(const string &path, const string &originalPath);

    void Save(const string &path);

    void LoadFromOptions();

    void ChangePointer(CMacroObject *macro);

    void Execute();

    MACRO_RETURN_CODE Process();

    MACRO_RETURN_CODE Process(CMacroObject *macro);
};

extern CMacroManager g_MacroManager;

#endif
