#include <vector>
#include <random>

struct Edge
{
    int destinationVertex;
    int edgeWeight;

    Edge(int destinationVertex, int edgeWeight)
    {
        this->destinationVertex = destinationVertex;
        this->edgeWeight = edgeWeight;
    }
};

class Graph
{
public:
    std::vector<std::vector<Edge>> adjacencyList;

    explicit Graph(int numberOfVertices)
    {
        adjacencyList.resize(numberOfVertices);
    }

    int VertexCount() const
    {
        return static_cast<int>(adjacencyList.size());
    }

    void AddDirectedEdge(int fromVertex, int toVertex, int edgeWeight)
    {
        adjacencyList[fromVertex].push_back(Edge(toVertex, edgeWeight));
    }

    void AddUndirectedEdge(int vertexA, int vertexB, int edgeWeight)
    {
        AddDirectedEdge(vertexA, vertexB, edgeWeight);
        AddDirectedEdge(vertexB, vertexA, edgeWeight);
    }

    static Graph MakeRandomUndirectedGraph(
        int numberOfVertices,
        int numberOfEdges,
        int maxEdgeWeight,
        int randomSeed)
    {
        Graph graph(numberOfVertices);

        std::mt19937 randomGenerator(randomSeed);
        std::uniform_int_distribution<int> vertexPicker(0, numberOfVertices - 1);
        std::uniform_int_distribution<int> weightPicker(1, maxEdgeWeight);

        for (int edgeIndex = 0; edgeIndex < numberOfEdges; edgeIndex++)
        {
            int vertexU = vertexPicker(randomGenerator);
            int vertexV = vertexPicker(randomGenerator);

            if (vertexU == vertexV)
            {
                continue;
            }

            int weight = weightPicker(randomGenerator);
            graph.AddUndirectedEdge(vertexU, vertexV, weight);
        }

        return graph;
    }
};
