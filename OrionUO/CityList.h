

#ifndef CITYLIST_H
#define CITYLIST_H

class CCityItem
{
public:
    uchar LocationIndex = 0;
    string Name = "";
    string Area = "";
    bool Selected = false;

    CCityItem();
    virtual ~CCityItem();

    class CCity m_City;

    virtual bool IsNewCity() { return false; }

    void InitCity();
};

class CCityItemNew : public CCityItem
{
public:
    uint X = 0;
    uint Y = 0;
    uint Z = 0;
    uint MapIndex = 0;
    uint Cliloc = 0;

    CCityItemNew();
    virtual ~CCityItemNew();

    bool IsNewCity() { return true; }
};

class CCityList
{
private:
    vector<CCityItem *> m_CityList;

public:
    CCityList();
    virtual ~CCityList();

    size_t CityCount() { return m_CityList.size(); }

    void AddCity(CCityItem *city) { m_CityList.push_back(city); }

    CCityItem *GetCity(int index);

    void Clear();
};

extern CCityList g_CityList;

#endif
