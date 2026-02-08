#include <iostream>
#include <vector>
#include <chrono>

#define UNITY_BUILD 1 // This is a simple way to include all .cpp files in one compilation unit, which can speed up compile times for small projects. Only necessary to include in Prim.cpp and Dijkstra.cpp, since they both need Graph.cpp, PairingHeap.cpp, and FibonacciHeap.cpp. 

#include "Graph.cpp"
#include "PairingHeap.cpp"
#include "FibonacciHeap.cpp"
#include "Dijkstra.cpp"
#include "Prim.cpp"

static long long AverageMicroseconds(const std::vector<long long>& runTimesInMicroseconds)
{
    long long sum = 0;
    for (size_t index = 0; index < runTimesInMicroseconds.size(); index++)
    {
        sum += runTimesInMicroseconds[index];
    }
    return sum / (long long)runTimesInMicroseconds.size();
}

template <typename FunctionToTime>
static long long TimeMicroseconds(FunctionToTime functionToRun)
{
    auto startTime = std::chrono::steady_clock::now();
    functionToRun();
    auto endTime = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
}

int main()
{
    int numberOfVertices = 5000;
    int numberOfEdges = 20000;

    Graph graph = Graph::MakeRandomUndirectedGraph(
        numberOfVertices,
        numberOfEdges,
        /*maxEdgeWeight*/ 100,
        /*randomSeed*/ 12345
    );

    int numberOfTrials = 5;

    std::vector<long long> dijkstraPairingTimes;
    //std::vector<long long> dijkstraFibonacciTimes;

    std::vector<long long> primPairingTimes;
    //std::vector<long long> primFibonacciTimes;

    for (int trialIndex = 0; trialIndex < numberOfTrials; trialIndex++)
    {
        long long dijkstraPairingTime = TimeMicroseconds([&]()
        {
            DijkstraUsingPairingHeap(graph, 0);
        });
        dijkstraPairingTimes.push_back(dijkstraPairingTime);

        /*
        long long dijkstraFibonacciTime = TimeMicroseconds([&]()
        {
            DijkstraUsingFibonacciHeap(graph, 0);
        });
        dijkstraFibonacciTimes.push_back(dijkstraFibonacciTime);
        */

        long long primPairingTime = TimeMicroseconds([&]()
        {
            PrimUsingPairingHeap(graph, 0);
        });
        primPairingTimes.push_back(primPairingTime);

        /*
        long long primFibonacciTime = TimeMicroseconds([&]()
        {
            PrimUsingFibonacciHeap(graph, 0);
        });
        primFibonacciTimes.push_back(primFibonacciTime);
        */
    }

    std::cout << "Trials: " << numberOfTrials << "\n";
    std::cout << "Dijkstra (PairingHeap) avg us:   " << AverageMicroseconds(dijkstraPairingTimes) << "\n";
    //std::cout << "Dijkstra (FibonacciHeap*) avg us:" << AverageMicroseconds(dijkstraFibonacciTimes) << "\n";
    std::cout << "Prim     (PairingHeap) avg us:   " << AverageMicroseconds(primPairingTimes) << "\n";
    //std::cout << "Prim     (FibonacciHeap*) avg us:" << AverageMicroseconds(primFibonacciTimes) << "\n";

    return 0;
}
