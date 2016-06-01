#include <cmath>
#include <algorithm>
#include "TTree.h"
#include "Random.h"
#include "Placement.h"

void TTree::initialize(std::vector<const Block*> blocks)
{
    std::shuffle(blocks.begin(), blocks.end(), Random::getInstance().getEngine());
    initializeRecur(root, 0, blocks);
}

void TTree::initializeRecur(Node *&p, size_t id, const std::vector<const Block*> &blocks)
{
    if (id >= blocks.size()) return;
    p = new Node(RotatableBlock(blocks[id], RotatableBlock::getRandRotate()));
    initializeRecur(p->l, id * 3 + 1, blocks);
    initializeRecur(p->m, id * 3 + 2, blocks);
    initializeRecur(p->r, id * 3 + 3, blocks);
}

Placement TTree::getPlacement() const
{
    Placement ret;
    getPlacementRecur(root, new ContourList::Node(0, -INFINITY, INFINITY), 0, ret);
    return ret;
}

void TTree::getPlacementRecur(const TTree::Node *p, ContourList::Node *q, double z, Placement &placement) const
{
    q = ContourList::replace(q, z, z + p->block.getH());
    placement.addBlock(p->block, q->y, z);

    // recur. z+ first, y+ second, x+ third. MUST.
    if (p->l)
        getPlacementRecur(p->l, q->next, z + p->block.getH(), placement);
    if (p->m)
        getPlacementRecur(p->m, q, z, placement);
    if (p->r)
    {
        double _y = q->y;
        ContourList::deleteSeg(q, NULL, NULL);
        getPlacementRecur(p->r, new ContourList::Node(_y, -INFINITY, INFINITY), z, placement);
    }
}

