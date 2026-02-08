#include "Graph.cpp"

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

    PairingHeapNode* MergePairs();

    void LinkChild();

    void CutFromParent();

    void DecreaseKey();
};

PairingHeapNode* PairingHeap::Insert(int key, int vertexId){
    PairingHeapNode* newNode = new PairingHeapNode(key, vertexId);
    PairingHeapNode* root = Meld(root, newNode);
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

    root = MergePairs();
    nodeCount--;

    if (root != nullptr)
    {
        root->parent = nullptr;
    }

    return oldRoot;
}

void PairingHeap::DecreaseKey(){

}

/*---------------------------

HELPERS

---------------------------*/

PairingHeapNode* PairingHeap::Meld(PairingHeapNode* a, PairingHeapNode* b){

}

void PairingHeap::LinkChild(){

}

void PairingHeap::CutFromParent(){

}

PairingHeapNode* PairingHeap::MergePairs(){

}
