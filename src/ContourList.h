#ifndef CONTOUR_LIST_H_
#define CONTOUR_LIST_H_

#include <cstdlib>

/**
 * Defines a vertical contour doubly-linked list.
 * used by TTree.
 * static class.
 */
class ContourList
{
public:
    /// A node of a contour list
    struct Node
    {
        double y, z1, z2; /// z1 < z2

        Node(const Node &) = delete;
        Node &operator=(const Node &) = delete;
        Node(Node &&);
        Node &operator=(Node &&);

        Node *getPrev() const { return prev; }
        Node *getNext() const { return next; }
    private:
        friend ContourList;

        Node *prev, *next; /// prev->z2 == z1, z2 == next->z1

        Node(double _y, double _z1, double _z2)
            : y(_y), z1(_z1), z2(_z2), prev(NULL), next(NULL) {}
        ~Node() { connect(prev, NULL), connect(NULL, next); }
    };

    /**
     * Replace a segment of nodes with a node
     * @param _z1,_z2 The z range of the new block
     * @param width The y width of the new block
     * @return The new node
     */
    static Node *replace(Node *p, double _z1, double _z2, double width);

    /**
     * Delete a segement of nodes starting from p, next to l, right to r.
     * i.e. range (l, r).
     * @param p Must in the range.
     * @param l,r Can be set to NULL to delete all.
     */
    static void deleteSeg(Node *l, Node *p, Node *r);

    /// Initialize a node with y=_y, z1=-inf, z2=inf
    static Node *init(double _y);

    /**
     * Connect two nodes
     * @param p The front node. Can be NULL.
     * @param q The back node. Can be NULL.
     */
    static void connect(Node *p, Node *q);
};

#endif // CONTOUR_LIST_H_

