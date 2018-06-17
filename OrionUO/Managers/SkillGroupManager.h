

#ifndef SKILLGROUPMANAGER_H
#define SKILLGROUPMANAGER_H

class CSkillGroupManager
{
    int Count = 0;

private:
    void MakeDefaultMiscellaneous();
    void MakeDefaultCombat();
    void MakeDefaultTradeSkills();
    void MakeDefaultMagic();
    void MakeDefaultWilderness();
    void MakeDefaultThieving();
    void MakeDefaultBard();

public:
    CSkillGroupObject *m_Groups{ NULL };

    CSkillGroupManager();
    ~CSkillGroupManager();

    void MakeDefault();

    void Clear();

    void Add(CSkillGroupObject *group);

    bool Remove(CSkillGroupObject *group);

    bool Load(string path);

    void Save(string path);
};

extern CSkillGroupManager g_SkillGroupManager;

#endif
