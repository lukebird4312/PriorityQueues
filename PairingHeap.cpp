#include "Graph.cpp"

class PairingHeapNode
{
    int priorityKey;
    int vertexId;
    PairingHeapNode* Parent;
    PairingHeapNode* firstChild;
    PairingHeapNode* nextSibling;
    PairingHeapNode* previousSibling;
};

class PairingHeap
{
    PairingHeapNode* root;
    int nodeCount;
};

PairingHeapNode* Insert(){
    return nullptr;
}

PairingHeapNode* FindMin(){

}

PairingHeapNode* DeleteMin(){

}

void DecreaseKey(){

}

/*---------------------------

HELPERS

---------------------------*/

PairingHeapNode* Meld(){

}

void LinkChild(){

}

void CutFromParent(){

}

PairingHeapNode* MergePairs(){

}
