#include <vector>
#include <cmath>
#include <stdexcept>
#include <chrono>

struct FibonacciHeapStats
{
    long long insertCount;
    long long deleteMinCount;
    long long decreaseKeyCount;
    long long insertNs;
    long long deleteMinNs;
    long long decreaseKeyNs;
};

static FibonacciHeapStats fibonacciHeapStats = {0, 0, 0, 0, 0, 0};

static void ResetFibonacciHeapStats()
{
    fibonacciHeapStats = {0, 0, 0, 0, 0, 0};
}

static FibonacciHeapStats GetFibonacciHeapStats()
{
    return fibonacciHeapStats;
}

class FibonacciHeapNode
{
public:
    int priorityKey;
    int vertexId;
    int degree;
    bool mark;

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
    auto startTime = std::chrono::steady_clock::now();

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

    auto endTime = std::chrono::steady_clock::now();
    fibonacciHeapStats.insertCount = fibonacciHeapStats.insertCount + 1;
    fibonacciHeapStats.insertNs = fibonacciHeapStats.insertNs + std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

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
    auto startTime = std::chrono::steady_clock::now();

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

    oldMin->left = oldMin;
    oldMin->right = oldMin;
    oldMin->parent = nullptr;
    oldMin->child = nullptr;

    auto endTime = std::chrono::steady_clock::now();
    fibonacciHeapStats.deleteMinCount = fibonacciHeapStats.deleteMinCount + 1;
    fibonacciHeapStats.deleteMinNs = fibonacciHeapStats.deleteMinNs + std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

    return oldMin;
}

void FibonacciHeap::DecreaseKey(FibonacciHeapNode* node, int newKey)
{
    auto startTime = std::chrono::steady_clock::now();

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

    auto endTime = std::chrono::steady_clock::now();
    fibonacciHeapStats.decreaseKeyCount = fibonacciHeapStats.decreaseKeyCount + 1;
    fibonacciHeapStats.decreaseKeyNs = fibonacciHeapStats.decreaseKeyNs + std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
}

int FibonacciHeap::Count()
{
    return nodeCount;
}

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
    int maxDegree = (int)std::log2((double)nodeCount) + 2;
    std::vector<FibonacciHeapNode*> A(maxDegree, nullptr);

    std::vector<FibonacciHeapNode*> roots;
    FibonacciHeapNode* curr = minNode;

    do
    {
        roots.push_back(curr);
        curr = curr->right;
    }
    while (curr != minNode);

    for (int i = 0; i < (int)roots.size(); i++)
    {
        FibonacciHeapNode* x = roots[i];
        int d = x->degree;

        while (A[d] != nullptr)
        {
            FibonacciHeapNode* y = A[d];

            if (x->priorityKey > y->priorityKey)
            {
                FibonacciHeapNode* temp = x;
                x = y;
                y = temp;
            }

            Link(y, x);
            A[d] = nullptr;
            d++;
        }

        A[d] = x;
    }

    minNode = nullptr;

    for (int d = 0; d < (int)A.size(); d++)
    {
        FibonacciHeapNode* node = A[d];
        if (node != nullptr)
        {
            if (minNode == nullptr)
            {
                node->left = node;
                node->right = node;
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
