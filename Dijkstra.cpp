#include <vector>
#include <utility>
#include <limits>

#ifndef UNITY_BUILD
#include "Graph.cpp"
#include "PairingHeap.cpp"
#include "FibonacciHeap.cpp"
#endif

template <typename HeapType, typename HeapNodeType>
static std::pair<std::vector<int>, std::vector<int>>
DijkstraImplementation(const Graph& graph, int sourceVertex)
{
    const int INF = std::numeric_limits<int>::max() / 4;

    int numberOfVertices = graph.VertexCount();

    std::vector<int> shortestDistanceToVertex(numberOfVertices, INF);
    std::vector<int> previousVertexOnShortestPath(numberOfVertices, -1);
    std::vector<bool> vertexHasFinalDistance(numberOfVertices, false);

    HeapType priorityQueue;
    std::vector<HeapNodeType*> heapNodeHandleForVertex(numberOfVertices, nullptr);

    for (int currentVertex = 0; currentVertex < numberOfVertices; currentVertex++)
    {
        if (currentVertex == sourceVertex)
        {
            shortestDistanceToVertex[currentVertex] = 0;
            heapNodeHandleForVertex[currentVertex] = priorityQueue.Insert(0, currentVertex);
        }
        else
        {
            heapNodeHandleForVertex[currentVertex] = priorityQueue.Insert(INF, currentVertex);
        }
    }

    while (priorityQueue.Count() > 0)
    {
        HeapNodeType* minimumHeapNode = priorityQueue.DeleteMin();
        int vertexWithSmallestDistance = minimumHeapNode->vertexId;

        if (vertexHasFinalDistance[vertexWithSmallestDistance])
        {
            continue;
        }

        vertexHasFinalDistance[vertexWithSmallestDistance] = true;

        if (shortestDistanceToVertex[vertexWithSmallestDistance] == INF)
        {
            break;
        }

        for (int outgoingEdgeIndex = 0; outgoingEdgeIndex < (int)graph.adjacencyList[vertexWithSmallestDistance].size(); outgoingEdgeIndex++)
        {
            const Edge& outgoingEdge = graph.adjacencyList[vertexWithSmallestDistance][outgoingEdgeIndex];
            int neighborVertex = outgoingEdge.destinationVertex;

            if (vertexHasFinalDistance[neighborVertex])
            {
                continue;
            }

            int candidateDistance =
                shortestDistanceToVertex[vertexWithSmallestDistance] + outgoingEdge.edgeWeight;

            if (candidateDistance < shortestDistanceToVertex[neighborVertex])
            {
                shortestDistanceToVertex[neighborVertex] = candidateDistance;
                previousVertexOnShortestPath[neighborVertex] = vertexWithSmallestDistance;

                priorityQueue.DecreaseKey(heapNodeHandleForVertex[neighborVertex], candidateDistance);
            }
        }
    }

    return {shortestDistanceToVertex, previousVertexOnShortestPath};
}

static std::pair<std::vector<int>, std::vector<int>>
DijkstraUsingPairingHeap(const Graph& graph, int sourceVertex)
{
    return DijkstraImplementation<PairingHeap, PairingHeapNode>(graph, sourceVertex);
}

static std::pair<std::vector<int>, std::vector<int>>
DijkstraUsingFibonacciHeap(const Graph& graph, int sourceVertex)
{
    return DijkstraImplementation<FibonacciHeap, FibonacciHeapNode>(graph, sourceVertex);
}
