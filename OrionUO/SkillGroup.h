

#ifndef SKILLGROUP_H
#define SKILLGROUP_H

class CSkillGroupObject
{
public:
    int Count = 0;
    bool Maximized = false;
    string Name = "No Name";

private:
    uchar m_Items[60];

public:
    CSkillGroupObject *m_Next{ NULL };
    CSkillGroupObject *m_Prev{ NULL };

    CSkillGroupObject();
    ~CSkillGroupObject();

    uchar GetItem(intptr_t index);

    void Add(uchar index);

    void AddSorted(uchar index);

    void Remove(uchar index);

    bool Contains(uchar index);

    void Sort();

    void TransferTo(CSkillGroupObject *group);
};

#endif
