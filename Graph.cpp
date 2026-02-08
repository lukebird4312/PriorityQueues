#include <vector>
#include <random>

struct Edge
{
    int to;
    int weight;

    Edge(int to, int weight) : to(to), weight(weight) {}
};

class Graph
{
public:
    std::vector<std::vector<Edge>> adj;

    explicit Graph(int n)
    {
        adj.resize(n);
    }

    int Count() const
    {
        return static_cast<int>(adj.size());
    }

    int UndirectedEdgeCount() const
    {
        long long total = 0;
        for (int u = 0; u < Count(); u++)
        {
            total += (long long)adj[u].size();
        }
        return (int)(total / 2);
    }

    void AddEdge(int u, int v, int weight)
    {
        adj[u].push_back(Edge(v, weight));
    }

    void AddUndirectedEdge(int u, int v, int weight)
    {
        AddEdge(u, v, weight);
        AddEdge(v, u, weight);
    }

    static Graph MakeRandomUndirectedGraph(int n, int m, int maxW, int seed)
    {
        Graph g(n);

        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> nodeDist(0, n - 1);
        std::uniform_int_distribution<int> wDist(1, maxW);

        for (int i = 0; i < m; i++)
        {
            int u = nodeDist(rng);
            int v = nodeDist(rng);

            if (u == v)
            {
                continue;
            }

            int w = wDist(rng);
            g.AddUndirectedEdge(u, v, w);
        }

        return g;
    }

    static Graph MakeGridUndirectedGraph(int rows, int cols, int maxW, int seed)
    {
        int n = rows * cols;
        Graph g(n);

        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> wDist(1, maxW);

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                int u = r * cols + c;

                if (c + 1 < cols)
                {
                    int v = u + 1;
                    int w = wDist(rng);
                    g.AddUndirectedEdge(u, v, w);
                }

                if (r + 1 < rows)
                {
                    int v = u + cols;
                    int w = wDist(rng);
                    g.AddUndirectedEdge(u, v, w);
                }
            }
        }

        return g;
    }

    static Graph MakeSyntheticWorstCaseGraph(int n, int k, int baseW)
    {
        Graph g(n);

        for (int i = 0; i < n - 1; i++)
        {
            g.AddUndirectedEdge(i, i + 1, 1);
        }

        for (int i = 0; i < k; i++)
        {
            int w = baseW - i;
            if (w < 1)
            {
                w = 1;
            }

            for (int v = k; v < n; v++)
            {
                if (v == i)
                {
                    continue;
                }
                g.AddUndirectedEdge(i, v, w);
            }
        }

        return g;
    }
};
