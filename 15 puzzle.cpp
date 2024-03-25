#include <iostream>
#include <array>
#include <string_view>
#include <algorithm>
#include "Random.h"

constexpr int g_consoleLines{ 25 };


class Tile {

public:
    Tile() = default;
    explicit Tile(int num)
        : m_num{ num }
    {}

    friend bool operator==(Tile t1,Tile t2) {
        return t1.m_num == t2.m_num;
    }
    friend bool operator!=(Tile t1, Tile t2) { return !(t1 == t2); }

    friend std::ostream& operator<<(std::ostream& out, const Tile& tile) {
        if (tile.m_num > 9)
            out << ' ' << tile.getNum() << ' ';
        else if (tile.m_num > 0)
            out << "  " << tile.getNum() << ' ';
        else if (tile.m_num == 0)
            out << "    ";

        return out;
    }
    bool isEmpty() const{ return m_num == 0; }
    int getNum() const { return m_num; };

private:
    int m_num{};
};


class Direction {
public:
    enum Type {
        up,left,down,right,max_direction
    };

    Direction(Type type)
        : m_type{ type }
    {}

    static Direction getRandomDirecton() {
        Type random{ static_cast<Type>(Random::get(0, max_direction - 1)) };
        return Direction{ random };
    }

    std::string_view toString()const {
        switch (m_type) {
        case up:    return "up";
        case left:  return "left";
        case down:  return "down";
        case right: return "right";
        default:    return "quit";
        }
    }

    Direction operator-() {
        switch (m_type) {
        case up:    m_type = down; break;
        case left:  m_type = right; break;
        case down:  m_type = up; break;
        case right: m_type= left; break;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, Direction d) {
        out << d.toString();

        return out;
    }

    Type getDirection() {
        return m_type;
    }

private:
    Type m_type{};
};


struct Point {
    int x{};
    int y{};


    Point(int px, int py)
        : x{ px }
        , y{ py }
    {}

    Point getAdjacentPoint(Direction dir) {
        switch(dir.getDirection()) {
        case Direction::up:     return Point{ x,     y - 1 };
        case Direction::down:   return Point{ x,     y + 1 };
        case Direction::left:   return Point{ x - 1, y };
        case Direction::right:  return Point{ x + 1, y };
        }
    }

    bool friend operator==(Point p1, Point p2) { return (p1.x == p2.x && p1.y == p2.y); }
    bool friend operator!=(Point p1, Point p2) { return !(p2 == p2); }
};


namespace UserInput {
    
    void ignoreLine() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    bool isValidCommand(char c) {
        switch (c) {
        case 'w':
        case 'a':
        case 's':
        case 'd':
        case 'q':
            return true;
        default:
            return false;
        }
    }

    char getCharacter() {

        char c{};
        std::cin >> c;
        return c;
    }

    Direction::Type convertToDirection(char ch) {
        switch (ch) {
        case 'w': return Direction::up;
        case 'a': return Direction::left;
        case 's': return Direction::down;
        case 'd': return Direction::right;
        case 'q': return Direction::max_direction;
        }
    }

    Direction getCommandFromUser() {
        char ch{};
        while (!isValidCommand(ch))
            ch = getCharacter();
        
        return Direction{ convertToDirection(ch) };
    }

    
}

class Board {
public:

    static void printEmptyLines(int lines) {
        for (int i{ 0 }; i < lines; i++)
            std::cout << '\n';
    }


    friend std::ostream& operator<<(std::ostream& out, const Board& board) {

        printEmptyLines(g_consoleLines);

        for (int y{ 0 }; y < board.SIZE; y++) {
            for (int x{ 0 }; x < board.SIZE; x++) {
                out << board.m_tiles[y][x];
            }
            out << '\n';
        }
        return out;
    }



    bool validPoint(Point pt) {
        return (pt.x >= 0 && pt.x < SIZE)
            && (pt.y >= 0 && pt.y < SIZE);
    }

    Point getEmptyTilePos() const
    {
        for (int y = 0; y < SIZE; ++y)
            for (int x = 0; x < SIZE; ++x)
                if (m_tiles[y][x].isEmpty())
                    return { x,y };

        assert(0 && "There is no empty tile in the board!!!");
        return { -1,-1 };
    }
    bool moveTile(Direction d) {    
        Point p1{ getEmptyTilePos() };
        Point p2{ p1.getAdjacentPoint(-d) };
        if (!validPoint(p2))
            return false;
        swapTiles(p1, p2);
        return true;
    }

    void randomize() {
        for (int i{ 0 }; i < 1000; i++) {
            bool succes = moveTile(Direction{ Direction::getRandomDirecton() });
            if (succes)
                --i;
        }
    }

    bool playerWon() {
        for (int y = 0; y < SIZE; ++y)
            for (int x = 0; x < SIZE; ++x)
                if (m_tiles[y][x] != m_solvedTiles[y][x])
                    return false;
        return true;
    }

private:

    void swapTiles(Point pt1, Point pt2) {
        std::swap(m_tiles[pt1.y][pt1.x], m_tiles[pt2.y][pt2.x]);
    }

    static constexpr int SIZE = 4;
    Tile m_emptyTile{ 0};
    Tile m_tiles[SIZE][SIZE]{
        Tile{ 1 }, Tile { 2 }, Tile { 3 } , Tile { 4 },
        Tile { 5 } , Tile { 6 }, Tile { 7 }, Tile { 8 },
        Tile { 9 }, Tile { 10 }, Tile { 11 }, Tile { 12 },
        Tile { 13 }, Tile { 14 }, Tile { 15 }, Tile { 0 } };

    Tile m_solvedTiles[SIZE][SIZE]{
        Tile{ 1 }, Tile { 2 }, Tile { 3 } , Tile { 4 },
        Tile { 5 } , Tile { 6 }, Tile { 7 }, Tile { 8 },
        Tile { 9 }, Tile { 10 }, Tile { 11 }, Tile { 12 },
        Tile { 13 }, Tile { 14 }, Tile { 15 }, Tile { 0 } };
};


int main()
{



    Random::generate();
    Board board{};

    board.randomize();
    std::cout << board;

    while (true) {

        Direction direction{ UserInput::getCommandFromUser() };

        std::cout << "Valid Command: " << direction << '\n';
        if (board.moveTile(direction)) {
            board.printEmptyLines(g_consoleLines);
            std::cout << board;
        }

        if (direction.getDirection() == Direction::max_direction) {
            std::cout << "\n\nBye!\n\n";
            break;
        }

        if (board.playerWon()) {
            std::cout << "\n\nYou won!\n\n";
            break;
        }
    }
    return 0;
}