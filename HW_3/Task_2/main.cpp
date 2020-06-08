/*
 * Придиус Екатерина, АПО-13
 *
 *
 * Задача 2
 *
 * Дан невзвешенный неориентированный граф.
 * В графе может быть несколько кратчайших путей между какими-то вершинами.
 * Найдите количество различных кратчайших путей между заданными вершинами.
    Требования: сложность O(V+E).
    Формат ввода.
    v: кол-во вершин (макс. 50000),
    n: кол-во ребер (макс. 200000),
    n пар реберных вершин,
    пара вершин u, w для запроса.
    Формат вывода.
    Количество кратчайших путей от u к w.
 */


#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <functional>

class ListGraph
{
public:
    ListGraph(int size) : adjacencyLists(size) {}

    ~ListGraph() {}

    void AddEdge(int from, int to)
    {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(to);
    }

    int VerticesCount()
    {
        return (int)adjacencyLists.size();
    }

    std::vector<int> GetNextVertices(int vertex)
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

private:
    std::vector<std::vector<int>> adjacencyLists;
};

void BFS(ListGraph graph, int vertex, std::vector<int> &quantity, std::vector<int> &parent, std::vector<int> &distances)
{
    std::queue<int> qu;
    qu.push(vertex);

    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();

        for (int nextVertex: graph.GetNextVertices(currentVertex))
        {
           if (distances[nextVertex] > distances[currentVertex] + 1)
           {
               distances[nextVertex] = distances[currentVertex] + 1;
               parent[nextVertex] = currentVertex;
               quantity[nextVertex] = quantity[currentVertex];
               qu.push(nextVertex);
           }
           else if(distances[nextVertex] == distances[currentVertex] + 1 && parent[nextVertex] != currentVertex)
           {
               quantity[nextVertex] += quantity[currentVertex];
           }

        }
    }
}

int mainBFS(ListGraph &graph, int U, int V)
{
    std::vector<int> distances(graph.VerticesCount(), INT32_MAX);
    distances[U] = 0;

    std::vector<int> parent_index(graph.VerticesCount(), -1);
    std::vector<int> paths_quantity(graph.VerticesCount(), 0);
    paths_quantity[U] = 1;

    BFS(graph, U, paths_quantity, parent_index, distances);

    return paths_quantity[V];
}

int main()
{
   int vertices;
   int edges;
   std::cin >> vertices;
   std::cin >> edges;


   ListGraph graph(vertices);
   int from, to;
   for (int i = 0; i < edges; i++)
   {
       std::cin >> from >> to;
       graph.AddEdge(from, to);
       graph.AddEdge(to, from);
   }

   int U, V;
   std::cin >> U >> V;

   int quantity = mainBFS(graph, U, V);
   std::cout << quantity;
   return 0;
}