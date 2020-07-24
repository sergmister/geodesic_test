#include <vector>
#include <stdint.h>
#include "board.h"
#include "geodesic_y.h"

void print(const char*);
void print(int);
void print(float);

extern int neighbors[93][6];

namespace geodesic_y {

/*
std::vector<std::vector<uint16_t>> graph = adjacency_list_graph(93, neighbors);
int n = 93;
std::vector<uint16_t> graph_left   = {92, 69, 70, 71, 72, 73, 74, 75, 76}; // 0b1
std::vector<uint16_t> graph_bottem = {76, 77, 78, 79, 80, 81, 82, 83, 84}; // 0b01
std::vector<uint16_t> graph_right  = {84, 85, 86, 87, 88, 89, 90, 91, 92}; // 0b001
*/


std::vector<std::vector<uint16_t>> graph = geodesic_graph(5);
int n = graph.size();
std::vector<uint16_t> graph_left   = {26, 27, 28, 29, 18}; // 0b1
std::vector<uint16_t> graph_bottem = {22, 23, 24, 25, 26}; // 0b01
std::vector<uint16_t> graph_right  = {18, 19, 20, 21, 22}; // 0b001
// make the graph generator ouput the sides, like at the end (3) or something


State::State(int bsize, int turn_) {
    turn = turn_;
    board_size = bsize;
    board_.resize(board_size);
    for (int i = 0; i < board_size; i++) {
        board_[i].parent = i;
    }
}

void State::SwitchTurn() {
    turn *= -1;
}

uint16_t State::Find(uint16_t cell) {
    uint16_t parent = board_[cell].parent;
    if (parent != cell) {
        do {
        parent = board_[parent].parent;
        } while (parent != board_[parent].parent);
        board_[cell].parent = parent;
    }
    return parent;
}

bool State::Union(uint16_t cell_a, uint8_t cell_b) {
    uint16_t leader_a = Find(cell_a);
    uint16_t leader_b = Find(cell_b);

    if (leader_a == leader_b) {
        return true;
    }

    if (board_[leader_a].size < board_[leader_b].size) {
        std::swap(leader_a, leader_b);
    }

    board_[leader_b].parent = leader_a;
    board_[leader_a].size += board_[leader_b].size;
    board_[leader_a].edge |= board_[leader_b].edge;

    return false;
}

bool State::Move(uint16_t cell) {
    if (board_[cell].player == 0) {
        board_[cell].player = turn;
        for (int i = 0; i < graph[cell].size(); i++) {
            if (board_[graph[cell][i]].player == turn) {
                Union(cell, graph[cell][i]);
            }
        }
        uint16_t leader = Find(cell);
        if ((board_[leader].edge & 0b1) == 0) {
            for (int i = 0; i < graph_left.size(); i++) {
                if (cell == graph_left[i]) {
                    board_[leader].edge |= 0b1;
                }
            }
        }
        if ((board_[leader].edge & 0b10) == 0) {
            for (int i = 0; i < graph_bottem.size(); i++) {
                if (cell == graph_bottem[i]) {
                    board_[leader].edge |= 0b10;
                }
            }
        }
        if ((board_[leader].edge & 0b100) == 0) {
            for (int i = 0; i < graph_right.size(); i++) {
                if (cell == graph_right[i]) {
                    board_[leader].edge |= 0b100;
                }
            }
        }
        if ((board_[leader].edge & 0b111) == 0b111) {
            win = turn;
        }
        SwitchTurn();
        return true;
    }
    return false;
}

std::vector<uint16_t> State::ValidMoves() {
    std::vector<uint16_t> valid;
    for (uint16_t i = 0; i < board_size; i++) {
        if (board_[i].player == 0) {
            valid.push_back(i);
        }
    }
    return valid;
}

int8_t State::CheckWin() {
    return win;
}

/*
struct State {

    struct Cell {
        int8_t player = 0;
        uint8_t edge = 0;
        uint16_t parent;
        uint16_t size = 1;
    };

    std::vector<Cell> board_;
    int8_t turn;
    int8_t win = 0;

    State(int board_size, int8_t turn_) {
        board_.resize(board_size);
        turn = turn_;
    }

    void SwitchTurn() {
        turn *= -1;
    }

    uint16_t Find(uint16_t cell) {
        uint16_t parent = board_[cell].parent;
        if (parent != cell) {
            do {
            parent = board_[parent].parent;
            } while (parent != board_[parent].parent);
            board_[cell].parent = parent;
        }
        return parent;
    }

    bool Union(uint16_t cell_a, uint8_t cell_b) {
        uint16_t leader_a = Find(cell_a);
        uint16_t leader_b = Find(cell_b);

        if (leader_a == leader_b) {
            return true;
        }

        if (board_[leader_a].size < board_[leader_b].size) {
            std::swap(leader_a, leader_b);
        }

        board_[leader_b].parent = leader_a;
        board_[leader_a].size += board_[leader_b].size;
        board_[leader_a].edge |= board_[leader_b].edge;

        return false;
    }

    bool Move(uint16_t cell) {
        if (board_[cell].player == 0) {
            board_[cell].player = turn;
            for (int i = 0; i < graph[cell].size(); i++) {
                if (board_[graph[cell][i]].player == turn) {
                    Union(cell, graph[cell][i]);
                }
            }
            uint16_t leader = Find(cell);
            if (board_[leader].edge & 0b1 == 0) {
                for (int i = 0; i < graph_left.size(); i++) {
                    if (cell == graph_left[i]) {
                        board_[leader].edge |= 0b1;
                    }
                }
            }
            if (board_[leader].edge & 0b01 == 0) {
                for (int i = 0; i < graph_bottem.size(); i++) {
                    if (cell == graph_bottem[i]) {
                        board_[leader].edge |= 0b01;
                    }
                }
            }
            if (board_[leader].edge & 0b001 == 0) {
                for (int i = 0; i < graph_right.size(); i++) {
                    if (cell == graph_bottem[i]) {
                        board_[leader].edge |= 0b001;
                    }
                }
            }
            if (board_[leader].edge & 0b111 == 0b111) {
                win = turn;
            }
            SwitchTurn();
            return true;
        }
        return false;
    }

    bool CheckWin() {
        return win;
    }
};
*/

}
