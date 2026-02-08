#include <stdexcept>

class PairingHeapNode
{
public:
    int priorityKey;
    int vertexId;
    PairingHeapNode* parent;
    PairingHeapNode* firstChild;
    PairingHeapNode* nextSibling;
    PairingHeapNode* prevSibling;

    
    PairingHeapNode(int key, int v)
    {
        priorityKey = key;
        vertexId = v;

        parent = nullptr;
        firstChild = nullptr;
        nextSibling = nullptr;
        prevSibling = nullptr;
    }
};

class PairingHeap
{
private:
    PairingHeapNode* root;
    int nodeCount;
public:
    PairingHeap()
    {
        root = nullptr;
        nodeCount = 0;
    }

    PairingHeapNode* Insert(int key, int vertexId);

    PairingHeapNode* FindMin();

    PairingHeapNode* DeleteMin();

    PairingHeapNode* Meld(PairingHeapNode* a, PairingHeapNode* b);

    PairingHeapNode* MergePairs(PairingHeapNode* firstSibling);

    void LinkChild(PairingHeapNode* parent, PairingHeapNode* child);

    void CutFromParent(PairingHeapNode* node);

    void DecreaseKey(PairingHeapNode* node, int newKey);

    int Count();
};

PairingHeapNode* PairingHeap::Insert(int key, int vertexId){
    PairingHeapNode* newNode = new PairingHeapNode(key, vertexId);
    root = Meld(root, newNode);
    nodeCount++;
    return newNode;
}

PairingHeapNode* PairingHeap::FindMin()
{
    if (root == nullptr)
    {
        throw std::runtime_error("FindMin on empty heap");
    }
    return root;
}

PairingHeapNode* PairingHeap::DeleteMin(){
    if(root == nullptr)
    {
        throw std::runtime_error("DeleteMin on empty heap");
    }

    PairingHeapNode* oldRoot = root;
    PairingHeapNode* childList = root->firstChild;

    oldRoot->parent = nullptr;
    oldRoot->firstChild = nullptr;
    oldRoot->nextSibling = nullptr;
    oldRoot->prevSibling = nullptr;

    root = MergePairs(childList);
    nodeCount--;

    if (root != nullptr)
    {
        root->parent = nullptr;
    }

    return oldRoot;
}

void PairingHeap::DecreaseKey(PairingHeapNode* node, int newKey)
{
    if (newKey >= node->priorityKey)
    {
        throw std::runtime_error("New key is not smaller than current key in DecreaseKey");
    }

    node->priorityKey = newKey;

    if (node == root)
    {
        return;
    }

    CutFromParent(node);
    root = Meld(root, node);
}

/*---------------------------

HELPERS

---------------------------*/

PairingHeapNode* PairingHeap::Meld(PairingHeapNode* a, PairingHeapNode* b){
    if (a == nullptr)
    {
        return b;
    }
    if (b == nullptr)
    {
        return a;
    }

    if (a->priorityKey <= b->priorityKey)
    {
        LinkChild(a, b);
        return a;
    }
    else
    {
        LinkChild(b, a);
        return b;
    }
}

void PairingHeap::LinkChild(PairingHeapNode* parent, PairingHeapNode* child){
    child->parent = parent;
    child->prevSibling = nullptr;
    child->nextSibling = parent->firstChild;
    if (parent->firstChild != nullptr)
    {
        parent->firstChild->prevSibling = child;
    }
    parent->firstChild = child;
}

void PairingHeap::CutFromParent(PairingHeapNode* node){
    PairingHeapNode* parent = node->parent;
    if (parent == nullptr)
    {
        return;
    }

    if (node->prevSibling != nullptr)
    {
        node->prevSibling->nextSibling = node->nextSibling;
    }
    else
    {
        parent->firstChild = node->nextSibling;
    }

    if (node->nextSibling != nullptr)
    {
        node->nextSibling->prevSibling = node->prevSibling;
    }

    node->parent = nullptr;
    node->prevSibling = nullptr;
    node->nextSibling = nullptr;
}

PairingHeapNode* PairingHeap::MergePairs(PairingHeapNode* firstSibling){
    if(firstSibling == nullptr)
    {
        return nullptr;
    }
    if(firstSibling->nextSibling == nullptr)
    {
        firstSibling->parent = nullptr;
        firstSibling->prevSibling = nullptr;
        firstSibling->nextSibling = nullptr;
        return firstSibling;
    }

    PairingHeapNode* first = firstSibling;
    PairingHeapNode* second = firstSibling->nextSibling;
    PairingHeapNode* remainingSiblings = second->nextSibling;

    first->parent = nullptr;
    first->prevSibling = nullptr;
    first->nextSibling = nullptr;

    second->parent = nullptr;
    second->prevSibling = nullptr;
    second->nextSibling = nullptr;

    if(remainingSiblings != nullptr)
    {
        remainingSiblings->prevSibling = nullptr;
    }

    PairingHeapNode* mergedPair = Meld(first, second);
    PairingHeapNode* mergedRest = MergePairs(remainingSiblings);

    return Meld(mergedPair, mergedRest);
}

int PairingHeap::Count()
{
    return nodeCount;
}
