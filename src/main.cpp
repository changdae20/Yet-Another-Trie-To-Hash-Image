#include "benchmark.hpp"
#include "feature.hpp"
#include "util.hpp"
#include <bits/stdc++.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <opencv2/opencv.hpp>

int main( int argc, char **argv ) {
    benchmark_real( 0.3, argc >= 2 && strcmp( argv[ 1 ], "--csv=true" ) == 0 );
    return 0;
}