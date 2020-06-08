#include <iostream>
#include <array>
#include <unordered_map>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <set>

const char FIELD_SIZE = 16;
const std::array<char, FIELD_SIZE> finalField = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

class GameState
{
public:
    int depth;

    GameState(const std::array<char, FIELD_SIZE> &arr) : field(arr), depth(0)
    {
        emptyPos = -1;
        for (int i = 0; i < FIELD_SIZE; i++)
        {
            if (field[i] == 0)
            {
                emptyPos = i;
                break;
            }
        }
        assert(emptyPos != -1);
    }
    GameState(const GameState &other)
        : field(other.field), emptyPos(other.emptyPos), depth(other.depth)
    {
    }

    bool operator==(const GameState &other) const
    {
        return other.field == field;
    }

    bool IsComplete() const
    {
        return field == finalField;
    }

    bool IsSolvable() const
    {
        int inversions = 0;
        for (int i = 0; i < FIELD_SIZE; i++)
            for (int j = i + 1; j < FIELD_SIZE; j++)
                if (field[i] != 0 && field[j] != 0 && field[i] > field[j])
                    inversions++;
        return (inversions % 2 == 0) == (emptyPos / 4 % 2 == 1);
    }

    bool CanMoveLeft() const
    {
        return emptyPos % 4 != 3;
    }

    bool CanMoveRight() const
    {
        return emptyPos % 4 != 0;
    }

    bool CanMoveDown() const
    {
        return emptyPos > 3;
    }

    bool CanMoveUp() const
    {
        return emptyPos < 12;
    }

    GameState MoveLeft() const
    {
        assert(CanMoveLeft());
        GameState nextState(*this);
        std::swap(nextState.field[emptyPos], nextState.field[emptyPos + 1]);
        nextState.emptyPos++;
        nextState.depth++;
        return nextState;
    }

    GameState MoveRight() const
    {
        assert(CanMoveRight());
        GameState nextState(*this);
        std::swap(nextState.field[emptyPos], nextState.field[emptyPos - 1]);
        nextState.emptyPos--;
        nextState.depth++;
        return nextState;
    }

    GameState MoveUp() const
    {
        assert(CanMoveUp());
        GameState nextState(*this);
        std::swap(nextState.field[emptyPos], nextState.field[emptyPos + 4]);
        nextState.emptyPos += 4;
        nextState.depth++;
        return nextState;
    }
    GameState MoveDown() const
    {
        assert(CanMoveDown());
        GameState nextState(*this);
        std::swap(nextState.field[emptyPos], nextState.field[emptyPos - 4]);
        nextState.emptyPos -= 4;
        nextState.depth++;
        return nextState;
    }

    size_t GetHeuristic()
    {
        int h = 0;
        for (int i = 0; i < FIELD_SIZE; i++) { // проходим по всем костяшкам
            if (field[i] == 0) // пропускаем пустую костяшку
                continue;
            if (i != field[i] - 1) { // если костяшка стоит не на своем месте
                int moving = abs(i / FIELD_SIZE - (field[i] - 1) / FIELD_SIZE) // перемещние по вертикали
                             + 10 * abs(i % FIELD_SIZE - (field[i] - 1) % FIELD_SIZE); // перемещение по горизонтали
                h += moving; // добавляем в эвристику число ходов для постановкии костяшки на место
            }
        }
        return h;
    }

private:

    size_t GetInversionCount() const
    {
        int inversions = 0;
        for (int i = 0; i < FIELD_SIZE; i++)
            for (int j = i + 1; j < FIELD_SIZE; j++)
                if (field[i] != 0 && field[j] != 0 && field[i] > field[j])
                    inversions++;
    }

    std::array<char, FIELD_SIZE> field;
    char emptyPos;
    friend std::ostream& operator<<(std::ostream &os, const GameState &state);
    friend struct GameStateHash;
};

struct GameStateHash
{
    size_t operator()(const GameState &state) const
    {
        size_t hash = 0;
        std::memcpy(&hash, state.field.data(), sizeof(hash));
        return hash;
    }
};

struct compare {
    bool operator() (const std::pair<size_t, GameState>& lhs, const std::pair<size_t, GameState>& rhs) const {
       return lhs.first < rhs.first;
    }
};

std::string GetSolution(const std::array<char, FIELD_SIZE> &startField)
{
    GameState startState(startField);

    if (!startState.IsSolvable())
        return "-1";

    std::set<std::pair<size_t, GameState>, compare> qu;
    std::unordered_map<GameState, char, GameStateHash> visited;

    visited[startState] = 'S';
    qu.emplace(std::make_pair(0, startState));

    while (true)
    {
        GameState state = qu.begin()->second;
        qu.erase(qu.begin());

        if (state.IsComplete())
        {
            break;
        }

        if (state.CanMoveLeft())
        {
            GameState nextState = state.MoveLeft();
            if (visited.find(nextState) == visited.end())
            {
                visited[nextState] = 'L';
                qu.emplace(std::make_pair(nextState.GetHeuristic() + nextState.depth, nextState));
            }
        }
        if (state.CanMoveRight())
        {
            GameState nextState = state.MoveRight();
            if (visited.find(nextState) == visited.end())
            {
                visited[nextState] = 'R';
                qu.emplace(std::make_pair(nextState.GetHeuristic() + nextState.depth, nextState));
            }
        }
        if (state.CanMoveUp())
        {
            GameState nextState = state.MoveUp();
            if (visited.find(nextState) == visited.end())
            {
                visited[nextState] = 'U';
                qu.emplace(std::make_pair(nextState.GetHeuristic() + nextState.depth, nextState));
            }
        }
        if (state.CanMoveDown())
        {
            GameState nextState = state.MoveDown();
            if (visited.find(nextState) == visited.end())
            {
                visited[nextState] = 'D';
                qu.emplace(std::make_pair(nextState.GetHeuristic() + nextState.depth, nextState));
            }
        }
    }

    std::string path;
    GameState state(finalField);

    while (visited[state] != 'S')
    {
        switch (visited[state])
        {
            case 'L':
            {
                path += 'L';
                state = state.MoveRight();
                break;
            }
            case 'R':
            {
                path += 'R';
                state = state.MoveLeft();
                break;
            }
            case 'U':
            {
                path += 'U';
                state = state.MoveDown();
                break;
            }
            case 'D':
            {
                path += 'D';
                state = state.MoveUp();
                break;
            }
        }
    }

    std::cout << path.length() << std::endl;
    std::reverse(path.begin(), path.end());
    return path;
}



std::ostream& operator<<(std::ostream &os, const GameState &state)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            os << static_cast<int>(state.field[i * 4 + j]) << " ";
        }
        os << std::endl;
    }
    return os;
}

int main(int argc, const char * argv[]) {
   // std::array<char, FIELD_SIZE> field = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 11, 13, 14, 15, 12};
    std::array<char, FIELD_SIZE> field{};
    int buf;
    for (int i = 0; i < 16; i++)
    {
        std::cin >> buf;
        field[i] = buf;
    }
    std::cout << GetSolution(field);
    return 0;
}
