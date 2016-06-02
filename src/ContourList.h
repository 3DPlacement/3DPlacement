#ifndef CONTOUR_LIST_H_
#define CONTOUR_LIST_H_

#include <cstdlib>

/// Defines a vertical contour doubly-linked list
class ContourList
{
public:
    /// A node of a contour list
    struct Node
    {
        double y, z1, z2; /// z1 < z2
        Node *prev, *next; /// prev->z2 == z1, z2 == next->z1

        Node(double _y, double _z1, double _z2)
            : y(_y), z1(_z1), z2(_z2), prev(NULL), next(NULL) {}

        Node(const Node &) = delete;
        Node &operator=(const Node &) = delete;
        Node(Node &&) = default;
        Node &operator=(Node &&) = default;
    };

    /// Replace a segment of nodes with a node
    /// @return : the new node
    static Node *replace(Node *p, double _z1, double _z2);

    /// Delete a segement of nodes starting from p, next to l, right to r
    /// i.e. range (l, r)
    /// p must in the range
    static void deleteSeg(Node *l, Node *p, Node *r);
};

#endif // CONTOUR_LIST_H_

