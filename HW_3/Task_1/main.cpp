#include <iostream>
#include <vector>
#include <cassert>
#include <functional>
#include <queue>
#include <set>
#include <iterator>

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph: public IGraph
{
public:
    ListGraph(int size) : adjacencyLists(size) {}

    ListGraph(const IGraph &graph) : adjacencyLists(graph.VerticesCount())
    {
        for (int i = 0; i < graph.VerticesCount(); i++)
        {
            adjacencyLists[i] = graph.GetNextVertices(i);
        }
    }

    ~ListGraph() {}

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(to);
    }

    int VerticesCount() const override
    {
        return (int)adjacencyLists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); from++)
        {
            for (int to: adjacencyLists[from])
            {
                if (to == vertex)
                {
                    prevVertices.push_back(from);
                }
            }
        }

        return prevVertices;
    }

private:
    std::vector<std::vector<int>> adjacencyLists;
};

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, std::function<void(int)> &func)
{
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();

        func(currentVertex);

        for (int nextVertex: graph.GetNextVertices(currentVertex))
        {
            if (!visited[nextVertex])
            {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph &graph, std::function<void(int)> func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            BFS(graph, i, visited, func);
    }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, std::function<void(int)> &func)
{
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex: graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
            DFS(graph, nextVertex, visited, func);
    }

}

void mainDFS(const IGraph &graph, std::function<void(int)> func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            DFS(graph, i, visited, func);
    }
}


class MatrixGraph : public IGraph{
private:
    std::vector<std::vector<bool>> adjacencyMatrix;
public:
    MatrixGraph(int size)
    {
        adjacencyMatrix.resize(size, std::vector<bool>(size, false));
    }

    MatrixGraph(const IGraph &graph)
    {
        adjacencyMatrix.resize(graph.VerticesCount(), std::vector<bool>(graph.VerticesCount(), false));
        for (int i = 0; i < graph.VerticesCount(); i++)
        {
            std::vector<int> next_ver = graph.GetNextVertices(i);
            for (auto &next : next_ver) {
                adjacencyMatrix[i][next] = true;
            }
        }
    }

    ~MatrixGraph() {}

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacencyMatrix.size());
        assert(0 <= to && to < adjacencyMatrix.size());
        adjacencyMatrix[from][to] = true;
    }

    int VerticesCount() const override
    {
        return (int)adjacencyMatrix.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        std::vector<int> next_vertices;
        assert(0 <= vertex && vertex < adjacencyMatrix.size());
        for (int i = 0; i < adjacencyMatrix.size(); i++)
        {
            if (adjacencyMatrix[vertex][i])
                next_vertices.push_back(i);
        }
        return next_vertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyMatrix.size());
        std::vector<int> prevVertices;

        for (int i = 0; i < adjacencyMatrix.size(); i++)
        {
            if (adjacencyMatrix[i][vertex])
                prevVertices.push_back(i);
        }

        return prevVertices;
    }
};

class SetGraph : public IGraph {
public:
    SetGraph(int size) : adjacencyLists(size) {}

    SetGraph(const IGraph &graph) : adjacencyLists(graph.VerticesCount())
    {
        for (int i = 0; i < graph.VerticesCount(); i++)
        {
            for (auto &it : graph.GetNextVertices(i))
            {
                adjacencyLists[i].insert(it);
            }
        }


    }

    ~SetGraph() {}

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].insert(to);
    }

    int VerticesCount() const override
    {
        return (int)adjacencyLists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> next(adjacencyLists[vertex].begin(), adjacencyLists[vertex].end());
        return next;
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); from++)
        {
            for (int to: adjacencyLists[from])
            {
                if (to == vertex)
                {
                    prevVertices.push_back(from);
                }
            }
        }

        return prevVertices;
    }

private:
    std::vector<std::set<int>> adjacencyLists;

};


class ArcGraph : public IGraph {
public:
    ArcGraph() {}

    ArcGraph(const IGraph &graph)
    {
        for (int from = 0; from < graph.VerticesCount(); from++)
        {
            for (auto &to : graph.GetNextVertices(from))
            {
                std::pair<int, int> pair(from, to);
                edgesList.push_back(pair);
            }
        }
    }

    ~ArcGraph() {}

    void AddEdge(int from, int to) override
    {
        std::pair<int, int> pair(from, to);
        edgesList.push_back(pair);
    }

    int VerticesCount() const override
    {
        std::vector<int> vertices;

        for (auto &edges : edgesList)
        {
            auto res = std::find(std::begin(vertices), std::end(vertices), edges.first);
            if (res == std::end(vertices))
                vertices.push_back(edges.first);

            res = std::find(std::begin(vertices), std::end(vertices), edges.second);
            if (res == std::end(vertices))
                vertices.push_back(edges.second);
        }
       return vertices.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        std::vector<int> next;

        for (auto &edges : edgesList)
        {
            if (edges.first == vertex)
                next.push_back(edges.second);
        }
        return next;
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int> prev;

        for (auto &edges : edgesList)
        {
            if (edges.second == vertex)
                prev.push_back(edges.first);
        }
        return prev;
    }

private:
    std::vector<std::pair<int, int>> edgesList;
};


int main(int argc, const char * argv[]) {
    ListGraph graph(7);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 5);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(1, 5);
    graph.AddEdge(1, 6);
    graph.AddEdge(3, 2);
    graph.AddEdge(3, 4);
    graph.AddEdge(3, 6);
    graph.AddEdge(5, 4);
    graph.AddEdge(5, 6);
    graph.AddEdge(6, 4);

    std::cout << "BFS ListGraph:    ";
    mainBFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "DFS ListGraph:    ";
    mainDFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl << std::endl;

    MatrixGraph matrixGraph(graph);

    SetGraph setGraph(matrixGraph);

    ArcGraph arcGraph(setGraph);
    std::cout << "BFS ArcGraph:    ";
    mainBFS(arcGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "DFS ArcGraph:    ";
    mainDFS(arcGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;


    return 0;
}
 /*   std::cout << std::endl;
    for (int i = 0; i < 7; i++)
    {
        std::vector<int> next, prev;
        next = matrixGraph.GetNextVertices(i);
        prev = matrixGraph.GetPrevVertices(i);

        std::cout << "vertex " << i << std::endl;

        if (next.empty())
            std::cout << "no next vertices";
        else
        {
            std::cout << "next: " ;
            for (auto it : next)
                std::cout << it <<  " ";
        }

        std::cout << std::endl;

        if (prev.empty())
            std::cout << "no prev vertices";
        else
        {
            std::cout << "prev: " ;
            for (auto it : prev)
                std::cout << it <<  " ";
        }
        std::cout << std::endl << std::endl;
    }
*/

