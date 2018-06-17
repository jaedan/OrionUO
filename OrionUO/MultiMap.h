

#ifndef MULTIMAP_H
#define MULTIMAP_H

class CMultiMap
{
public:
    CMultiMap();
    ~CMultiMap();

    void LoadMap(class CGumpMap *gump, class CGUIExternalTexture *map);

    bool LoadFacet(class CGumpMap *gump, class CGUIExternalTexture *map, int facet);
};

extern CMultiMap g_MultiMap;

#endif
