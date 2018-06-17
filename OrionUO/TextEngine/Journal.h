

#ifndef TextDataH
#define TextDataH

class CJournal : public CBaseQueue
{
    int MaxSize = 0;
    int Size = 0;

public:
    CJournal(int maxSize);
    ~CJournal();

    void Add(CTextData *obj);

    virtual void Clear();
};

extern CJournal g_Journal;

#endif
