#ifndef T_TREE_H_
#define T_TREE_H_

#include <cstdlib>
#include <set>
#include <vector>
#include "Block.h"
#include "Random.h"
#include "ContourList.h"

class Placement;

class TTree
{
public:
    TTree() : root(NULL) {}

    /// Randomly initialize a tree with given blocks
    void initialize(const std::vector<Block> &blocks);

    /// Randomly initialize a tree with given const blocks pointers
    /// @param blocks The given blocks. Shouldn't be const &
    void initialize(std::vector<const Block*> blocks);

    /// Build a placement accroding to this tree
    Placement getPlacement() const;

    /// Randomly move one node to another position
    /// O(h). h is the height of the tree.
    void randomMove();

    /// Randomly swap two nodes
    /// O(1).
    void randomSwap();

    /// Randomly rotate one node's block
    /// O(1).
    void randomRotate();

    TTree(const TTree &) = delete;
    TTree &operator=(const TTree &) = delete;
    TTree(TTree &&) = default;
    TTree &operator=(TTree &&) = default;

#ifndef NDEBUG
    /// Verify the some correctness of the tree
    /// Uses assert
    void verify() const;
#endif // NDEBUG

private:
    /// Internal class. A node of T Tree.
    struct Node
    {
        RotatableBlock block;
        Node *l, *m, *r; /// children
        Node **in; /// points to pointer point in the node. e.g. = &(parent->l) or &root.

        Node(const RotatableBlock &_block)
            : block(_block), l(NULL), m(NULL), r(NULL), in(NULL) {}

        Node(const Node &) = delete;
        Node &operator=(const Node &) = delete;
        Node(Node &&);
        Node &operator=(Node &&);
    };

    /// Used by TTree::initialize
    void initializeRecur(Node *&p, size_t id, const std::vector<const Block*> &blocks);

    /// Used by TTree::getPlacement
    /// @return The node replaced q
    ContourList::Node *getPlacementRecur(const Node *p, ContourList::Node *q, double z, Placement &placement) const;

    /// Randomly choose a node
    Node *randNode();

    /// Randomly choose a pair of different nodes
    void randPair(Node *&p, Node *&q);

    /// Swap content of two node but not affecting the tree structure
    void swapNode(Node *p, Node *q);

    /// Detach a node from the tree
    /// This is a random procedure
    /// @return The node
    Node *detach(Node *p);

    /// Randomly insert node `child` as child of node `parent`
    void insert(Node *parent, Node *child);

#ifndef NDEBUG
    /// Used by Tree::verify
    /// @param p Should be * const &, instead of const * const &. Type conversion will change the address
    void verifyRecur(Node* const& p, std::set<const Block*> &blocks) const;
#endif // NDEBUG

    Node *root;

    /// Node pool. This helps map an id to a node
    std::vector<Node> pool;
};

inline TTree::Node *TTree::randNode()
{
    return &pool[Random::getInstance().getRandomInt(0, pool.size() - 1)];
}

inline void TTree::randPair(Node *&p, Node *&q)
{
    assert(pool.size() > 1);
    do p = randNode(), q = randNode(); while (p == q);
}

inline void TTree::randomMove()
{
    // Do not use randPair here
    Node *p(NULL), *q(randNode());
    q = detach(q);
    do p = randNode(); while (p == q);
    insert(p, q);
}

inline void TTree::randomSwap()
{
    Node *p(NULL), *q(NULL);
    randPair(p, q);
    swapNode(p, q);
}

inline void TTree::randomRotate()
{
    Node *p = randNode();
    RotatableBlock::ROTATE_NAME oriRotation = p->block.rotation;
    do
        p->block.rotation = RotatableBlock::getRandRotate();
    while (p->block.rotation == oriRotation);
}

#endif // T_TREE_H_
