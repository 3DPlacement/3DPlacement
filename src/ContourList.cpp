#include <cmath>
#include <cassert>
#include <algorithm>
#include "ContourList.h"

ContourList::Node *ContourList::replace(ContourList::Node *p, double _z1, double _z2)
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
    q2 = new Node(_y, _z1, _z2);
    if (p2->z2 > _z2)
        q3 = new Node(_y, _z2, p2->z2);

    if (q1)
        q2->prev = q1, q1->prev = p1->prev, p1->prev->next = q1;
    else
        q2->prev = p1->prev, p1->prev->next = q2;
    if (q3)
        q2->next = q3, q3->next = p2->next, p2->next->prev = q3;
    else
        q2->next = p2->next, p2->next->prev = q2;
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

