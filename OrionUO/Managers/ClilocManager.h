

#ifndef CLILOCMANAGER_H
#define CLILOCMANAGER_H

typedef map<uint, string> CLILOC_MAP;

class CCliloc : public CBaseQueueItem
{
public:
    string Language = "";
    bool Loaded = false;

private:
    CLILOC_MAP m_ClilocSystem;

    CLILOC_MAP m_ClilocRegular;

    CLILOC_MAP m_ClilocSupport;

    string Load(uint &id);

    inline wstring CamelCaseTest(bool toCamelCase, const string &result);

public:
    CCliloc(const string &lang);
    virtual ~CCliloc();

    WISP_FILE::CMappedFile m_File;

    wstring Get(int id, bool toCamelCase = false, string result = "");

    string GetA(int id, bool toCamelCase = false, string result = "");

    wstring GetW(int id, bool toCamelCase = false, string result = "");
};

class CClilocManager : public CBaseQueue
{
private:
    CCliloc *m_LastCliloc{ NULL };

    CCliloc *m_ENUCliloc{ NULL };

public:
    CClilocManager();
    virtual ~CClilocManager();

    CCliloc *Cliloc(const string &lang);

    wstring ParseArgumentsToClilocString(int cliloc, bool toCamelCase, wstring args);
};

extern CClilocManager g_ClilocManager;

#endif
