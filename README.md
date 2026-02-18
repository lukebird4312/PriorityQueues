# Priority Queue Graph Benchmark (Dijkstra + Prim)

This project benchmarks **Pairing Heap vs Fibonacci Heap** as the priority queue inside:
- **Dijkstra’s shortest path**
- **Prim’s minimum spanning tree**

The C++ program generates benchmark data into a CSV file, and the Python script can summarize and plot it.

---

## Build + Run (C++)

### Prereqs
- A C++17 compiler (`g++`, `clang++`, or MSVC `cl`)

> Note: This project uses a “unity build” style (the `main.cpp` includes the other `.cpp` files), so you normally compile **only `main.cpp`**.

### Linux / macOS (g++ or clang++)
```bash
# from the project directory
g++ -std=c++17 -O2 -o executable_name main.cpp
./executable_name