

#ifndef MACRO_H
#define MACRO_H

class CMacroObject : public CBaseQueueItem
{
public:
    MACRO_CODE Code = MC_NONE;

    MACRO_SUB_CODE SubCode = MSC_NONE;

    char HasSubMenu = 0;

    CMacroObject(const MACRO_CODE &code, const MACRO_SUB_CODE &subCode);
    virtual ~CMacroObject();

    virtual bool HaveString() { return false; }
};

class CMacroObjectString : public CMacroObject
{
public:
    string String = "";

    CMacroObjectString(const MACRO_CODE &code, const MACRO_SUB_CODE &subCode, const string &str);
    virtual ~CMacroObjectString();

    virtual bool HaveString() { return true; }
};

class CMacro : public CBaseQueueItem
{
public:
    ushort Key = 0;

    bool Alt = false;
    bool Ctrl = false;
    bool Shift = false;

    CMacro(ushort key, bool alt, bool ctrl, bool shift);
    virtual ~CMacro();

    static const int MACRO_ACTION_NAME_COUNT = 61;
    static const int MACRO_ACTION_COUNT = 210;

    static const char *m_MacroActionName[MACRO_ACTION_NAME_COUNT];

    static const char *m_MacroAction[MACRO_ACTION_COUNT];

    static const char *GetActionName(int index) { return m_MacroActionName[index]; }
    static const char *GetAction(int index) { return m_MacroAction[index]; }

    void ChangeObject(CMacroObject *source, CMacroObject *obj);

    static CMacro *Load(WISP_FILE::CMappedFile &file);

    void Save(WISP_FILE::CBinaryFileWriter &writer);

    static CMacro *CreateBlankMacro();

    static CMacroObject *CreateMacro(const MACRO_CODE &code);

    static void GetBoundByCode(const MACRO_CODE &code, int &count, int &offset);

    CMacro *GetCopy();
};

extern CMacroObject *g_MacroPointer;

#endif
