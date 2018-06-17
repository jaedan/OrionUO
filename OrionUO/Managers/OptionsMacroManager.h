

#ifndef OPTIONSMACROMANAGER_H
#define OPTIONSMACROMANAGER_H

class COptionsMacroManager : public CBaseQueue
{
public:
    COptionsMacroManager();
    virtual ~COptionsMacroManager();

    void LoadFromMacro();
};

extern COptionsMacroManager g_OptionsMacroManager;

#endif
