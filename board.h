#pragma once
#include <vector>

std::vector<uint8_t> matrix_graph(int n, int arr[][6]);

std::vector<std::vector<uint16_t>> adjacency_list_graph(int n, int arr[][6]);

std::vector<std::vector<uint16_t>> geodesic_graph(int n);