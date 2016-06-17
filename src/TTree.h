#ifndef T_TREE_H_
#define T_TREE_H_

#include <cstdlib>
#include <set>
#include <list>
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

    /**
     * Randomly initialize a tree with given const blocks pointers
     * @param blocks The given blocks. Shouldn't be const &
     */
    void initialize(std::vector<const Block*> blocks);

    /**
     * Build a placement accroding to this tree.
     * O(n). n is the number of the nodes.
     */
    Placement getPlacement() const;

    /**
     * Randomly move one node to another position.
     * Requires at least 3 nodes in the tree.
     * O(h). h is the height of the tree.
     */
    void randomMove();

    /**
     * Randomly swap two nodes.
     * Requires at least 2 nodes in the tree.
     * O(1).
     */
    void randomSwap();

    /**
     * Randomly rotate one node's block.
     * O(1).
     */
    void randomRotate();

    /**
     * Undo last operation
     */
    void undo();

    TTree(const TTree &) = delete;
    TTree &operator=(const TTree &) = delete;
    TTree(TTree &&) = default;
    TTree &operator=(TTree &&) = default;

#ifndef NDEBUG
    /**
     * Verify the some correctness of the tree.
     * Uses assert.
     */
    void verify() const;
#endif // NDEBUG

private:
    /// Internal class. A node of T Tree.
    class Node
    {
    public:
        RotatableBlock block;
        Node *l, *m, *r; /// children
        Node **in; /// points to pointer point in the node. e.g. = &(parent->l) or &root.

        Node(const RotatableBlock &_block, Node *_l = NULL, Node *_m = NULL, Node *_r = NULL, Node **_in = NULL)
            : block(_block), l(_l), m(_m), r(_r), in(_in), backup(NULL) {}
        ~Node();

        Node(const Node &) = delete;
        Node &operator=(const Node &) = delete;
        Node(Node &&);
        Node &operator=(Node &&);

        /// save to backup if backup is empty
        void save();
        /// retrieve from backup and clean backup
        void retrieve();
        /// discard backup
        void discard();

#ifndef NDEBUG
        /// Print debug message
        void printMsg() const;
#endif // NDEBUG

    private:
        Node *backup; /// backup to undo
    };

    /// Used by TTree::initialize
    void initializeRecur(Node *&p, size_t id, const std::vector<const Block*> &blocks);

    struct subtree_t
    {
        const Node *root;
        double minY, minZ;
        subtree_t(const Node *_root, double _minY, double _minZ)
            : root(_root), minY(_minY), minZ(_minZ) {}
    };

    /**
     * Used by TTree::getPlacement
     * @return The node replaced q
     */
    ContourList::Node *getPlacementRecur(
        const Node *p, ContourList::Node *q, double z, Placement &placement, std::list<subtree_t> &subtrees) const;

    /// Randomly choose a node
    Node *randNode();

    /// Randomly choose a pair of different nodes
    void randPair(Node *&p, Node *&q);

    /// Swap content of two node but not affecting the tree structure
    void swapNode(Node *p, Node *q);

    /**
     * Detach a node from the tree.
     * This is a random procedure.
     * @return The node
     */
    Node *detach(Node *p);

    /// Randomly insert node `child` as child of node `parent`
    void insert(Node *parent, Node *child);

#ifndef NDEBUG
    /**
     * Used by Tree::verify
     * @param p Should be * const &, instead of const * const &. Type conversion will change the address
     */
    void verifyRecur(Node* const& p, std::set<const Block*> &blocks) const;
#endif // NDEBUG

    Node *root;

    /// Node pool. This helps map an id to a node
    std::vector<Node> pool;

    /// Nodes that need to undo
    std::vector<Node*> undoList;
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

#endif // T_TREE_H_
