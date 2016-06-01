#ifndef T_TREE_H_
#define T_TREE_H_

#include <cstdlib>
#include <vector>
#include "Block.h"
#include "ContourList.h"

class Placement;

class TTree
{
public:
    /// Randomly initialize a tree with given blocks
    /// @1 : blocks. The given blocks. Shouldn't be const &
    void initialize(std::vector<const Block*> blocks);

    /// Build a placement accroding to this tree
    Placement getPlacement() const;

private:
    /// Internal class. A node of T Tree.
    struct Node
    {
        RotatableBlock block;
        Node *l, *m, *r; /// children

        Node(const RotatableBlock &_block) : block(_block), l(NULL), m(NULL), r(NULL) {}

        Node(const Node &) = delete;
        Node &operator=(const Node &) = delete;
    };

    /// Used by TTree::initialize
    void initializeRecur(Node *&p, size_t id, const std::vector<const Block*> &blocks);

    /// Used by TTree::getPlacement
    void getPlacementRecur(const Node *p, ContourList::Node *q, double z, Placement &placement) const;

    Node *root;
};

#endif // T_TREE_H_
