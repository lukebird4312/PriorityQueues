#include <vector>
#include <cmath>
#include <stdexcept>


class FibonacciHeapNode
{
public:
    int priorityKey; // the key used for heap ordering
    int vertexId;  // graph vertex identifier
    int degree;  // number of children
    bool mark; // whether this node has lost a child since it became a child itself

    FibonacciHeapNode* parent;
    FibonacciHeapNode* child;
    FibonacciHeapNode* left;
    FibonacciHeapNode* right;

    FibonacciHeapNode(int key, int v)
    {
        priorityKey = key;
        vertexId = v;
        degree = 0;
        mark = false;

        parent = nullptr;
        child = nullptr;
        left = this;
        right = this;
    }
};


class FibonacciHeap
{
private:
    FibonacciHeapNode* minNode;
    int nodeCount;

    void Link(FibonacciHeapNode* y, FibonacciHeapNode* x);
    void Consolidate();
    void Cut(FibonacciHeapNode* x, FibonacciHeapNode* y);
    void CascadingCut(FibonacciHeapNode* y);
    void AddToRootList(FibonacciHeapNode* node);
    void RemoveFromRootList(FibonacciHeapNode* node);

public:
    FibonacciHeap();

    FibonacciHeapNode* Insert(int key, int vertexId);
    FibonacciHeapNode* FindMin();
    FibonacciHeapNode* DeleteMin();
    void DecreaseKey(FibonacciHeapNode* node, int newKey);
    int Count();
};


FibonacciHeap::FibonacciHeap()
{
    minNode = nullptr;
    nodeCount = 0;
}


FibonacciHeapNode* FibonacciHeap::Insert(int key, int vertexId)
{
    FibonacciHeapNode* node = new FibonacciHeapNode(key, vertexId);
    if (minNode == nullptr)
    {
        minNode = node;
    }
    else
    {
        AddToRootList(node);
        if (node->priorityKey < minNode->priorityKey)
        {
            minNode = node;
        }
    }

    nodeCount++;
    return node;
}


FibonacciHeapNode* FibonacciHeap::FindMin()
{
    if (minNode == nullptr)
    {
        throw std::runtime_error("FindMin on empty heap");
    }
    return minNode;
}


FibonacciHeapNode* FibonacciHeap::DeleteMin()
{
    if (minNode == nullptr)
    {
        throw std::runtime_error("DeleteMin on empty heap");
    }

    FibonacciHeapNode* oldMin = minNode;

    if (oldMin->child != nullptr)
    {
        FibonacciHeapNode* child = oldMin->child;
        do
        {
            FibonacciHeapNode* next = child->right;
            AddToRootList(child);
            child->parent = nullptr;
            child = next;
        }
        while (child != oldMin->child);
    }

    RemoveFromRootList(oldMin);

    if (oldMin == oldMin->right)
    {
        minNode = nullptr;
    }
    else
    {
        minNode = oldMin->right;
        Consolidate();
    }

    nodeCount--;

    oldMin->left = oldMin->right = oldMin;
    oldMin->parent = nullptr;
    oldMin->child = nullptr;

    return oldMin;
}


void FibonacciHeap::DecreaseKey(FibonacciHeapNode* node, int newKey)
{
    if (newKey >= node->priorityKey)
    {
        throw std::runtime_error("New key is not smaller than current key in DecreaseKey");
    }

    node->priorityKey = newKey;
    FibonacciHeapNode* p = node->parent;

    if (p != nullptr && node->priorityKey < p->priorityKey)
    {
        Cut(node, p);
        CascadingCut(p);
    }

    if (node->priorityKey < minNode->priorityKey)
    {
        minNode = node;
    }
}


int FibonacciHeap::Count()
{
    return nodeCount;
}

// =================================================
//                   HELPERS
// =================================================


void FibonacciHeap::AddToRootList(FibonacciHeapNode* node)
{
    node->left = minNode;
    node->right = minNode->right;
    minNode->right->left = node;
    minNode->right = node;
}

void FibonacciHeap::RemoveFromRootList(FibonacciHeapNode* node)
{
    node->left->right = node->right;
    node->right->left = node->left;
}

void FibonacciHeap::Link(FibonacciHeapNode* y, FibonacciHeapNode* x)
{
    RemoveFromRootList(y);

    y->parent = x;
    y->mark = false;

    if (x->child == nullptr)
    {
        x->child = y;
        y->left = y;
        y->right = y;
    }
    else
    {
        FibonacciHeapNode* z = x->child;
        y->left = z;
        y->right = z->right;
        z->right->left = y;
        z->right = y;
    }

    x->degree++;
}

void FibonacciHeap::Consolidate()
{
    int maxDegree = static_cast<int>(std::log2(nodeCount)) + 2;
    std::vector<FibonacciHeapNode*> A(maxDegree, nullptr);

    std::vector<FibonacciHeapNode*> roots;
    FibonacciHeapNode* curr = minNode;

    do
    {
        roots.push_back(curr);
        curr = curr->right;
    }
    while (curr != minNode);

    for (FibonacciHeapNode* w : roots)
    {
        FibonacciHeapNode* x = w;
        int d = x->degree;

        while (A[d] != nullptr)
        {
            FibonacciHeapNode* y = A[d];

            if (x->priorityKey > y->priorityKey)
            {
                std::swap(x, y);
            }

            Link(y, x);
            A[d] = nullptr;
            d++;
        }
        A[d] = x;
    }

    minNode = nullptr;

    for (FibonacciHeapNode* node : A)
    {
        if (node != nullptr)
        {
            if (minNode == nullptr)
            {
                node->left = node->right = node;
                minNode = node;
            }
            else
            {
                AddToRootList(node);
                if (node->priorityKey < minNode->priorityKey)
                {
                    minNode = node;
                }
            }
        }
    }
}

void FibonacciHeap::Cut(FibonacciHeapNode* x, FibonacciHeapNode* y)
{
    if (x->right == x)
    {
        y->child = nullptr;
    }
    else
    {
        if (y->child == x)
        {
            y->child = x->right;
        }

        x->left->right = x->right;
        x->right->left = x->left;
    }

    y->degree--;

    AddToRootList(x);
    x->parent = nullptr;
    x->mark = false;
}

void FibonacciHeap::CascadingCut(FibonacciHeapNode* y)
{
    FibonacciHeapNode* z = y->parent;

    if (z != nullptr)
    {
        if (!y->mark)
        {
            y->mark = true;
        }
        else
        {
            Cut(y, z);
            CascadingCut(z);
        }
    }
}