#include <cmath>
#include <cassert>
#include <algorithm>
#include "TTree.h"
#include "Random.h"
#include "Placement.h"

void TTree::initialize(const std::vector<Block> &blocks)
{
    std::vector<const Block*> blockPtrs;
    for (const auto &item : blocks)
        blockPtrs.push_back(&item);
    initialize(blockPtrs);
}

void TTree::initialize(std::vector<const Block*> blocks)
{
    pool.reserve(blocks.size());
    std::shuffle(blocks.begin(), blocks.end(), Random::getInstance().getEngine());
    initializeRecur(root, 0, blocks);
}

void TTree::initializeRecur(Node *&p, size_t id, const std::vector<const Block*> &blocks)
{
    if (id >= blocks.size()) return;
    pool.emplace_back(RotatableBlock(blocks[id], RotatableBlock::getRandRotate()));
    p = &pool.back(), p->in = &p;
    initializeRecur(p->l, id * 3 + 1, blocks);
    initializeRecur(p->m, id * 3 + 2, blocks);
    initializeRecur(p->r, id * 3 + 3, blocks);
}

Placement TTree::getPlacement() const
{
    Placement ret;
    getPlacementRecur(root, ContourList::init(0), 0, ret);
    return ret;
}

ContourList::Node *TTree::getPlacementRecur(const TTree::Node *p, ContourList::Node *q, double z, Placement &placement) const
{
    q = ContourList::replace(q, z, z + p->block.getH());
    placement.addBlock(p->block, q->y, z);

    // recur. z+ first, y+ second, x+ third. MUST.
    if (p->l)
        getPlacementRecur(p->l, q->getNext(), z + p->block.getH(), placement);
    if (p->m)
        q = getPlacementRecur(p->m, q, z, placement);
    if (p->r)
    {
        double _y = q->y;
        ContourList::deleteSeg(NULL, q, NULL);
        getPlacementRecur(p->r, ContourList::init(_y), z, placement);
    }
    return q;
}

TTree::Node::Node(Node &&rhs)
    : block(rhs.block), l(rhs.l), m(rhs.m), r(rhs.r), in(rhs.in)
{
    *in = this;
}

TTree::Node &TTree::Node::operator=(TTree::Node &&rhs)
{
    block = rhs.block, l = rhs.l, m = rhs.m, r = rhs.r, in = rhs.in;
    *in = this;
    return *this;
}

void TTree::swapNode(TTree::Node *p, TTree::Node *q)
{
    std::swap(p->block, q->block);
}

TTree::Node *TTree::detach(TTree::Node *p)
{
#ifndef NDEBUG
    const Block *oriBlock = p->block.block; // used for checking
#endif
    while (p->l || p->m || p->r)
    {
        int cnt(0);
        Node *child[3];
        if (p->l) child[cnt++] = p->l;
        if (p->m) child[cnt++] = p->m;
        if (p->r) child[cnt++] = p->r;
        Node *q = child[Random::getInstance().getRandomInt(0, cnt - 1)];
        swapNode(p, q);
        p = q;
    }
    *(p->in) = NULL, p->in = NULL;
    assert(p->block.block == oriBlock);
    return p;
}

void TTree::insert(Node *parent, Node *child)
{
    assert(!child->l && !child->m && !child->r && !child->in);
    Node* Node::*candidates[3] = { &Node::l, &Node::m, &Node::r };
    Node* Node::*c = candidates[Random::getInstance().getRandomInt(0, 2)];
    child->*c = parent->*c;
    if (child->*c) (child->*c)->in = &(child->*c);
    parent->*c = child;
    child->in = &(parent->*c);
}

#ifndef NDEBUG

void TTree::verify() const
{
    std::set<const Block*> blocks;
    verifyRecur(root, blocks);
    assert(blocks.size() == pool.size());
}

void TTree::verifyRecur(Node* const& p, std::set<const Block*> &blocks) const
{
    blocks.insert(p->block.block);
    assert(&p == p->in);
    if (p->l)
        verifyRecur(p->l, blocks);
    if (p->m)
        verifyRecur(p->m, blocks);
    if (p->r)
        verifyRecur(p->r, blocks);
}

#endif // NDEBUG

