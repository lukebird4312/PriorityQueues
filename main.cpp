#include <fstream>
#include <string>
#include <chrono>

#define UNITY_BUILD 1

#include "Graph.cpp"
#include "PairingHeap.cpp"
#include "FibonacciHeap.cpp"
#include "Dijkstra.cpp"
#include "Prim.cpp"

static void WriteRow(std::ofstream& out,
                     const std::string& graphType,
                     int vertexCount,
                     int edgeCount,
                     const std::string& algorithmName,
                     const std::string& heapName,
                     int trialIndex,
                     long long totalUs,
                     long long insertCount,
                     long long deleteMinCount,
                     long long decreaseKeyCount,
                     long long insertNs,
                     long long deleteMinNs,
                     long long decreaseKeyNs)
{
    out << graphType << ","
        << vertexCount << ","
        << edgeCount << ","
        << algorithmName << ","
        << heapName << ","
        << trialIndex << ","
        << totalUs << ","
        << insertCount << ","
        << deleteMinCount << ","
        << decreaseKeyCount << ","
        << insertNs << ","
        << deleteMinNs << ","
        << decreaseKeyNs << "\n";
}

int main()
{
    std::ofstream out("results.csv");
    out << "graph_type,vertices,edges,algorithm,heap,trial,total_us,insert_count,deletemin_count,decreasekey_count,insert_ns,deletemin_ns,decreasekey_ns\n";

    int trials = 5;
    int maxWeight = 20;

    for (int sizeIndex = 0; sizeIndex < 2; sizeIndex++)
    {
        int n = (sizeIndex == 0) ? 1000 : 5000;

        int sparseEdges = 5 * n;
        int denseEdges = 20 * n;

        for (int trial = 0; trial < trials; trial++)
        {
            Graph randomSparse = Graph::MakeRandomUndirectedGraph(n, sparseEdges, maxWeight, 1000 + trial);
            Graph randomDense = Graph::MakeRandomUndirectedGraph(n, denseEdges, maxWeight, 2000 + trial);

            int gridSide;
            if (sizeIndex == 0)
            {
                gridSide = 30;
            }
            else
            {
                gridSide = 70;
            }
            Graph gridGraph = Graph::MakeGridUndirectedGraph(gridSide, gridSide, maxWeight, 3000 + trial);

            int k = n / 4;
            Graph worstCase = Graph::MakeSyntheticWorstCaseGraph(n, k, maxWeight);

            Graph graphs[4] = {randomSparse, randomDense, gridGraph, worstCase};
            std::string graphNames[4] = {"random_sparse", "random_dense", "grid", "synthetic_worst"};

            for (int graphIndex = 0; graphIndex < 4; graphIndex++)
            {
                Graph& g = graphs[graphIndex];
                int V = g.Count();
                int E = g.UndirectedEdgeCount();

                ResetPairingHeapStats();
                auto start1 = std::chrono::steady_clock::now();
                DijkstraUsingPairingHeap(g, 0);
                auto end1 = std::chrono::steady_clock::now();
                auto s1 = GetPairingHeapStats();
                long long total1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();
                WriteRow(out, graphNames[graphIndex], V, E, "dijkstra", "pairing", trial, total1, s1.insertCount, s1.deleteMinCount, s1.decreaseKeyCount, s1.insertNs, s1.deleteMinNs, s1.decreaseKeyNs);

                ResetFibonacciHeapStats();
                auto start2 = std::chrono::steady_clock::now();
                DijkstraUsingFibonacciHeap(g, 0);
                auto end2 = std::chrono::steady_clock::now();
                auto s2 = GetFibonacciHeapStats();
                long long total2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();
                WriteRow(out, graphNames[graphIndex], V, E, "dijkstra", "fibonacci", trial, total2, s2.insertCount, s2.deleteMinCount, s2.decreaseKeyCount, s2.insertNs, s2.deleteMinNs, s2.decreaseKeyNs);

                ResetPairingHeapStats();
                auto start3 = std::chrono::steady_clock::now();
                PrimUsingPairingHeap(g, 0);
                auto end3 = std::chrono::steady_clock::now();
                auto s3 = GetPairingHeapStats();
                long long total3 = std::chrono::duration_cast<std::chrono::microseconds>(end3 - start3).count();
                WriteRow(out, graphNames[graphIndex], V, E, "prim", "pairing", trial, total3, s3.insertCount, s3.deleteMinCount, s3.decreaseKeyCount, s3.insertNs, s3.deleteMinNs, s3.decreaseKeyNs);

                ResetFibonacciHeapStats();
                auto start4 = std::chrono::steady_clock::now();
                PrimUsingFibonacciHeap(g, 0);
                auto end4 = std::chrono::steady_clock::now();
                auto s4 = GetFibonacciHeapStats();
                long long total4 = std::chrono::duration_cast<std::chrono::microseconds>(end4 - start4).count();
                WriteRow(out, graphNames[graphIndex], V, E, "prim", "fibonacci", trial, total4, s4.insertCount, s4.deleteMinCount, s4.decreaseKeyCount, s4.insertNs, s4.deleteMinNs, s4.decreaseKeyNs);
            }
        }
    }

    return 0;
}
