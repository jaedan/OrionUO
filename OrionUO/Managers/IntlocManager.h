

#ifndef INTLOCMANAGER_H
#define INTLOCMANAGER_H

class CIntloc : public CBaseQueueItem
{
public:
    string Language = "";
    int FileIndex = -1;
    bool Loaded = false;

private:
    WSTRING_LIST m_Strings;

public:
    CIntloc(int fileIndex, const string &lang);
    virtual ~CIntloc();

    WISP_FILE::CMappedFile m_File;

    wstring Get(int id, bool toCamelCase = false);
};

class CIntlocManager : public CBaseQueue
{
private:
    CIntloc *Intloc(int fileIndex, const string &lang);

public:
    CIntlocManager();
    virtual ~CIntlocManager();

    wstring Intloc(const string &lang, uint clilocID, bool isNewCliloc);
};

extern CIntlocManager g_IntlocManager;

#endif
