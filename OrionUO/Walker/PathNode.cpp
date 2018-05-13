

#include "stdafx.h"

CPathNode::CPathNode()
{
}

CPathNode::~CPathNode()
{
    m_Parent = NULL;
}

void CPathNode::Reset()
{
    m_Parent = NULL;
    Used = false;
    X = Y = Z = Cost = DistFromStartCost = DistFromGoalCost = 0;
    Dir = DIR_NORTH;
}
