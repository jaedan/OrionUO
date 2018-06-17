

#ifndef SPEECHMANAGER_H
#define SPEECHMANAGER_H

class CLangCode
{
public:
    int Code = 0;

    string Abbreviature = "";

    uint Unknown = 0;

    string Language = "";

    string Country = "";

    CLangCode() {}
    CLangCode(const string &abbreviature, int code, const string &language, const string &country)
        : Abbreviature(abbreviature)
        , Code(code)
        , Language(language)
        , Country(country)
    {
    }
    virtual ~CLangCode() {}
};

class CSpeechItem
{
public:
    ushort Code = 0;

    wstring Data = L"";

    bool CheckStart = false;
    bool CheckEnd = false;

    CSpeechItem() {}
    CSpeechItem(ushort code, const wstring &data);
    virtual ~CSpeechItem() {}
};

class CSpeechManager
{
    CLangCode *CurrentLanguage = NULL;

private:
    vector<CSpeechItem> m_SpeechEntries;
    vector<CLangCode> m_LangCodes;
    bool m_Loaded{ false };

public:
    CSpeechManager();
    ~CSpeechManager();

    bool LoadSpeech();

    bool LoadLangCodes();

    void GetKeywords(const wchar_t *text, UINT_LIST &codes);
};

extern CSpeechManager g_SpeechManager;

#endif
