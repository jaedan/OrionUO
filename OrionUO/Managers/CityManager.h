

#ifndef CITYMANAGER_H
#define CITYMANAGER_H

class CCity
{
public:
    string Name = "";
    wstring Description = L"";

    CCity(const string &name, const wstring &description);
    virtual ~CCity();
};

class CCityManager
{
public:
    CCityManager();
    virtual ~CCityManager();

    void Init();

    deque<CCity> m_CityList;

    CCity GetCity(const string &name);

    void Clear();
};

extern CCityManager g_CityManager;

#endif
