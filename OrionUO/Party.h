

#ifndef PARTY_H
#define PARTY_H

class CParty
{
public:
    uint Leader = 0;

    uint Inviter = 0;

    bool CanLoot = false;

    CParty();
    ~CParty();

    CPartyObject Member[10];

    void ParsePacketData(WISP_DATASTREAM::CDataReader &reader);

    bool Contains(int serial);

    void Clear();
};

extern CParty g_Party;

#endif
