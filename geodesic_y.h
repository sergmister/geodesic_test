#ifndef GEODESIC_Y
#define GEODESIC_Y
#include <vector>
#include <stdint.h>
#include "board.h"

namespace geodesic_y {

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

    State(int bsize, int turn_);

    void SwitchTurn();

    uint16_t Find(uint16_t cell);

    bool Union(uint16_t cell_a, uint8_t cell_b);

    bool TestUnion(uint16_t cell_a, uint8_t cell_b);

    bool Move(uint16_t cell);

    std::vector<uint16_t> ValidMoves();

    int8_t CheckWin();
};

}
#endif
