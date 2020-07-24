#include <iostream>
#include <chrono>
#include <vector>
#include <stdint.h>
#include <math.h>
#include "board.h"
#include "geodesic_y.h"

typedef std::chrono::high_resolution_clock Clock;

int randint(int n) {
	return rand() % n;
}

int test() {
    geodesic_y::State state(geodesic_y::n, 1);
    std::vector<uint16_t> valid_moves;
    //int moves = 0;
    while (true) {
        valid_moves = state.ValidMoves();
        if (valid_moves.size() < 1) { // saftey check
            std::cin.get();
            throw std::runtime_error("your message");
        }
        state.Move(valid_moves[randint(valid_moves.size())]);
        //moves++;
        if (state.CheckWin() != 0) {
            return state.CheckWin();
            //return moves;
        }
    }
}

int main() {

    srand(time(0));

    int sum = 0;

    print(geodesic_y::graph.size());

    auto t1 = Clock::now();

    for (int t = 0; t < 10000; t++) {
        sum += test(); // get slow fast with larger graphs, could the move checks be optimized?
    } //symetries for AI?

    auto t2 = Clock::now();
    print(sum);
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << std::endl;
}