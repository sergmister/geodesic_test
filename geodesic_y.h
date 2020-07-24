#pragma once
#include <vector>
#include <stdint.h>
#include "board.h"

extern int neighbors[93][6];

template <typename T> // put in a header file.
void print(T msg) {
    std::cout << msg << std::endl;
}

namespace geodesic_y {

extern int n;
extern std::vector<std::vector<uint16_t>> graph;
extern std::vector<uint16_t> graph_left;
extern std::vector<uint16_t> graph_bottem;
extern std::vector<uint16_t> graph_right;

struct State {

    struct Cell {
        int8_t player = 0;
        uint8_t edge = 0;
        uint16_t parent;
        uint16_t size = 1;
    };

    std::vector<Cell> board_;
    uint16_t board_size;
    int8_t turn;
    int8_t win = 0;

    State(uint16_t bsize, int8_t turn_);

    void SwitchTurn();

    uint16_t Find(uint16_t cell);

    bool Union(uint16_t cell_a, uint16_t cell_b);

    bool TestUnion(uint16_t cell_a, uint16_t cell_b);

    bool Move(uint16_t cell);

    std::vector<uint16_t> ValidMoves();

    int8_t CheckWin();
};

}
