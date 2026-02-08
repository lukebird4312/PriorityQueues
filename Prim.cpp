#include <vector>
#include <utility>
#include <limits>

#ifndef UNITY_BUILD
#include "Graph.cpp"
#include "PairingHeap.cpp"
#include "FibonacciHeap.cpp"
#endif

template <typename HeapType, typename HeapNodeType>
static std::pair<std::vector<int>, int>
PrimImplementation(const Graph& graph, int startVertex)
{
    const int INF = std::numeric_limits<int>::max() / 4;

    int numberOfVertices = graph.Count();

    std::vector<int> bestEdgeWeightToReachVertex(numberOfVertices, INF);
    std::vector<int> parentVertexInMST(numberOfVertices, -1);
    std::vector<bool> vertexIsAlreadyInMST(numberOfVertices, false);

    bestEdgeWeightToReachVertex[startVertex] = 0;

    HeapType priorityQueue;
    std::vector<HeapNodeType*> heapNodeHandleForVertex(numberOfVertices, nullptr);

    for (int currentVertex = 0; currentVertex < numberOfVertices; currentVertex++)
    {
        heapNodeHandleForVertex[currentVertex] =
            priorityQueue.Insert(bestEdgeWeightToReachVertex[currentVertex], currentVertex);
    }

    int totalMSTWeight = 0;

    while (priorityQueue.Count() > 0)
    {
        HeapNodeType* minimumHeapNode = priorityQueue.DeleteMin();
        int vertexWithSmallestKey = minimumHeapNode->vertexId;

        if (vertexIsAlreadyInMST[vertexWithSmallestKey])
        {
            continue;
        }

        vertexIsAlreadyInMST[vertexWithSmallestKey] = true;

        if (bestEdgeWeightToReachVertex[vertexWithSmallestKey] == INF)
        {
            break;
        }

        totalMSTWeight += bestEdgeWeightToReachVertex[vertexWithSmallestKey];

        for (int outgoingEdgeIndex = 0; outgoingEdgeIndex < (int)graph.adj[vertexWithSmallestKey].size(); outgoingEdgeIndex++)
        {
            const Edge& outgoingEdge = graph.adj[vertexWithSmallestKey][outgoingEdgeIndex];
            int neighborVertex = outgoingEdge.to;

            if (vertexIsAlreadyInMST[neighborVertex])
            {
                continue;
            }

            int weight = outgoingEdge.weight;

            if (weight < bestEdgeWeightToReachVertex[neighborVertex])
            {
                bestEdgeWeightToReachVertex[neighborVertex] = weight;
                parentVertexInMST[neighborVertex] = vertexWithSmallestKey;

                priorityQueue.DecreaseKey(heapNodeHandleForVertex[neighborVertex], weight);
            }
        }
    }

    return {parentVertexInMST, totalMSTWeight};
}

static std::pair<std::vector<int>, int>
PrimUsingPairingHeap(const Graph& graph, int startVertex)
{
    return PrimImplementation<PairingHeap, PairingHeapNode>(graph, startVertex);
}

static std::pair<std::vector<int>, int>
PrimUsingFibonacciHeap(const Graph& graph, int startVertex)
{
    return PrimImplementation<FibonacciHeap, FibonacciHeapNode>(graph, startVertex);
}