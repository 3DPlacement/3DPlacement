#include <cmath>
#include <cassert>
#include <algorithm>
#include "ContourList.h"

ContourList::Node::Node(ContourList::Node &&rhs)
    : y(rhs.y), z1(rhs.z1), z2(rhs.z2), prev(rhs.prev), next(rhs.next)
{
    ContourList::connect(prev, this);
    ContourList::connect(this, next);
}

ContourList::Node &ContourList::Node::operator=(ContourList::Node &&rhs)
{
    y = rhs.y, z1 = rhs.z1, z2 = rhs.z2, prev = rhs.prev, next = rhs.next;
    ContourList::connect(prev, this);
    ContourList::connect(this, next);
    return *this;
}

ContourList::Node *ContourList::replace(ContourList::Node *p, double _z1, double _z2, double width)
{
    assert(p != NULL);
    Node *p1(p), *p2(p), *q1(NULL), *q2(NULL), *q3(NULL);
    double _y(p->y);

    // Original: |----- p1 -----|------ ... ------|----- p2 -----|
    // Now:      |-- q1 --|------------- q2 ------------|-- q3 --|
    //                   _z1                           _z2
    while (p1->prev && p1->prev->z2 > _z1)
        p1 = p1->prev, _y = std::max(_y, p1->y);
    while (p2->next && p2->next->z1 < _z2)
        p2 = p2->next, _y = std::max(_y, p2->y);

    if (p1->z1 < _z1)
        q1 = new Node(_y, p1->z1, _z1);
    q2 = new Node(_y + width, _z1, _z2);
    if (p2->z2 > _z2)
        q3 = new Node(_y, _z2, p2->z2);

    if (q1)
        connect(p1->prev, q1), connect(q1, q2);
    else
        connect(p1->prev, q2);
    if (q3)
        connect(q2, q3), connect(q3, p2->next);
    else
        connect(q2, p2->next);
    deleteSeg(p1->prev, p, p2->next);
    return q2;
}

void ContourList::deleteSeg(ContourList::Node *l, ContourList::Node *p, ContourList::Node *r)
{
    assert(p != NULL);
    for (Node *q = p->prev; q != l;)
    {
        Node *_q(q->prev);
        delete q;
        q = _q;
    }
    for (Node *q = p->next; q != r;)
    {
        Node *_q(q->next);
        delete q;
        q = _q;
    }
    delete p;
}

ContourList::Node *ContourList::init(double _y)
{
    /*Node *p = new Node(_y, -INFINITY, INFINITY),
         *d = new Node(_y, -INFINITY, -INFINITY),
         *u = new Node(_y, INFINITY, INFINITY);
    connect(d, p);
    connect(p, u);
    return p;*/
    return new Node(_y, -INFINITY, INFINITY);
}

void ContourList::connect(ContourList::Node *p, ContourList::Node *q)
{
    assert(!p || p != q);
    assert(!p || !q || (p->prev != q && q->next != p));
    if (p)
    {
        if (p->next)
            assert(p->next->prev == p), p->next->prev = NULL;
        p->next = q;
    }
    if (q)
    {
        if (q->prev)
            assert(q->prev->next == q), q->prev->next = NULL;
        q->prev = p;
    }
}

