/*
 * Придиус Екатерина, АПО-13
 *
 * Задача 3
 *
 * Требуется отыскать самый выгодный маршрут между городами.
 * Требования: время работы O((N+M)logN),
 *      где N-количество городов, M-известных дорог между ними.
 * Формат входных данных.
        Первая строка содержит число N – количество городов.
        Вторая строка содержит число M - количество дорог.
        Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
        Последняя строка содержит маршрут (откуда и куда нужно доехать).
    Формат выходных данных.
        Вывести длину самого выгодного маршрута.
 */

#include <iostream>
#include <vector>
#include <cassert>
#include <set>

class ListGraph
{
public:
    ListGraph(int size) : adjacencyLists(size) {}

    ~ListGraph() {}

    void AddEdge(int from, int to, int weight)
    {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(std::make_pair(to, weight));
        adjacencyLists[to].push_back(std::make_pair(from, weight));
    }

    int VerticesCount()
    {
        return (int)adjacencyLists.size();
    }

    std::vector<std::pair<int, int>> GetNextVertices(int vertex)
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

private:
    std::vector<std::vector<std::pair<int, int>>> adjacencyLists;
};

int Deikstra(ListGraph &graph, int from, int to)
{
    std::vector<bool> dequeued(graph.VerticesCount(), false);
    std::vector<int> distances(graph.VerticesCount(), INT32_MAX);

    distances[from] = 0;

    std::set<std::pair<int, int>> queue;
    queue.emplace(std::make_pair(0, from));

    while (!queue.empty())
    {
        int vertex = (queue.begin())->second; //берем значение текущей вершины
        queue.erase(queue.begin());
        dequeued[vertex] = true;              //помечаем вершину, как взятую из очереди

        std::vector<std::pair<int, int>> related = graph.GetNextVertices(vertex);
        for (auto &next : related)
        {
            if (distances[next.first] > distances[vertex] + next.second)  //проверяем кратчайший путь
            {

                if (distances[next.first] != INT32_MAX)      //удаляем старое значение из очереди
                    queue.erase(std::make_pair(distances[next.first], next.first));

                distances[next.first] = distances[vertex] + next.second; //обновляем значение и добавляем в очередь вершину
                queue.emplace(std::make_pair(distances[next.first], next.first));
            }
        }
    }

    if (distances[to] != INT32_MAX)
        return distances[to];
    else
        return -1;
}


int main()
{
    int vertices;
    int edges;
    std::cin >> vertices;
    std::cin >> edges;


    ListGraph graph(vertices);
    int from, to, weight;
    for (int i = 0; i < edges; i++)
    {
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
    }

    std::cin >> from >> to;

    int distance = Deikstra(graph, from, to);
    std::cout << distance;
    return 0;
}