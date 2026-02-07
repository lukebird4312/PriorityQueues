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

    PairingHeapNode* MergePairs(PairingHeapNode* first);

    void LinkChild(PairingHeapNode* parent, PairingHeapNode* child);

    void CutFromParent(PairingHeapNode* node);

    void DecreaseKey(PairingHeapNode* node, int newKey);
};

PairingHeapNode* PairingHeap::Insert(int key, int vertexId){
    PairingHeapNode* newNode = new PairingHeapNode(key, vertexId);
    PairingHeapNode* root = Meld(root, newNode);
    nodeCount++;
    return newNode;
}

PairingHeapNode* FindMin(){

}

PairingHeapNode* DeleteMin(){

}

void DecreaseKey(PairingHeapNode* node, int newKey){

}

/*---------------------------

HELPERS

---------------------------*/

PairingHeapNode* Meld(PairingHeapNode* a, PairingHeapNode* b){

}

void LinkChild(){

}

void CutFromParent(){

}

PairingHeapNode* MergePairs(){

}
