#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include "feature.hpp"
#include "trie.hpp"
#include "util.hpp"
#include <chrono>
#include <fmt/core.h>
#include <vector>

void benchmark_naive( std::vector<std::vector<float>> &database, std::vector<std::vector<float>> &input, std::vector<int> &answers, bool output_csv = false );
void benchmark_trie( Trie &trie, std::vector<std::vector<float>> &database, std::vector<std::vector<float>> &input, std::vector<int> &answers, float proj_margin = 0.2f, bool output_csv = false );
void benchmark_random( int sample = 50, int db_size = 10000, int feature_length = 20, float max_noise = 0.2f, float proj_margin = 0.2f );
void benchmark_real( float proj_margin = 0.2f, bool output_csv = false );
#endif // BENCHMARK_HPP