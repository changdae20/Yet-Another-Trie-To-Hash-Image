#include "benchmark.hpp"

void benchmark_naive( std::vector<std::vector<float>> &database, std::vector<std::vector<float>> &input, std::vector<int> &answers, bool output_csv ) {
    if ( output_csv )
        freopen( "result_naive.csv", "w", stdout );

    auto start = std::chrono::high_resolution_clock::now();
    int correct = 0;
    int wrong = 0;
    for ( int i = 0; i < input.size(); ++i ) {
        float min_error = __FLT_MAX__;
        int output = 0;
        for ( int j = 1; j + 1 <= database.size(); ++j ) {
            auto error = Util::distance( input[ i ], database[ j ] );
            if ( error < min_error ) {
                min_error = error;
                output = j;
            }
        }
        if ( output == answers[ i ] )
            ++correct;
        else
            ++wrong;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
    if ( !output_csv )
        fmt::print( "<==== naive algorithm ====>\n" );
    if ( !output_csv )
        fmt::print( "Accuracy : {}/{} ({:.2f}), Total Elapsed Time : {}us\n", correct, input.size(), static_cast<double>( correct ) / input.size(), elapsed_time );

    if ( output_csv )
        fmt::print( "{}, {}, {}, {}\n", correct, wrong, input.size(), elapsed_time );
    std::cout.flush();
}

void benchmark_trie( Trie &trie, std::vector<std::vector<float>> &database, std::vector<std::vector<float>> &input, std::vector<int> &answers, float proj_margin, bool output_csv ) {
    if ( !output_csv ) {
        fmt::print( "<==== trie algorithm ====>\n" );
        fmt::print( "{:^20}|{:^20}|{:^20}|{:^20}|{:^20}|{:^20}|{:^20}|{:^20}\n", "margin", "hash_hit_single", "hash_hit_multi", "hash_not_found", "hash_correct", "hash_wrong", "hash_multi_count", "Elapsed Time" );
    }

    int64_t elapsed_time = 0u;
    int hash_hit_single = 0;  // Trie에서 hash 찾은게 1개인 경우
    int hash_hit_multi = 0;   // Trie에서 hash 찾은게 여러개인 경우
    int hash_not_found = 0;   // Trie에서 hash를 찾지 못한 경우
    int hash_correct = 0;     // Trie에서 hash를 찾았는데 정답이 그 안에 포함되어있는 경우
    int hash_wrong = 0;       // Trie에서 hash를 찾았는데 정답이 그 안에 포함되어있지 않은 경우
    int hash_multi_count = 0; // Trie에서 hash가 여러개 나왔을 때 나온 hash의 개수
    for ( int i = 0; i < input.size(); ++i ) {
        auto quantized = Util::quantize_feature( input[ i ], proj_margin ); // proj_margin두고 input quantize
        auto start = std::chrono::high_resolution_clock::now();
        auto outputs = trie.find( quantized );
        if ( outputs.size() >= 2 ) { // hash collision이 일어나 가능한 답안이 2개 이상인 경우, 이들중에서 distance가 최소인 것을 고른다.
            hash_hit_multi++;
            hash_multi_count += outputs.size();
            float min_error = __FLT_MAX__;
            int output = 0;
            for ( int j = 0; j < outputs.size(); ++j ) {
                auto error = Util::distance( input[ i ], database[ outputs[ j ] ] );
                if ( error < min_error ) {
                    min_error = error;
                    output = outputs[ j ];
                }
            }
            if ( output == answers[ i ] )
                hash_correct++;
            else
                hash_wrong++;
        } else if ( outputs.size() == 1 ) { // Trie에서 찾은 답안이 정확하게 하나인 경우
            hash_hit_single++;
            if ( outputs[ 0 ] == answers[ i ] )
                hash_correct++;
            else
                hash_wrong++;
        } else { // data가 0인경우 naive search
            hash_not_found++;
            float min_error = __FLT_MAX__;
            int output = 0;
            for ( int j = 1; j + 1 <= database.size(); ++j ) {
                auto error = std::inner_product( input[ i ].begin(), input[ i ].end(), database[ j ].begin(), 0.0f, std::plus<>(), []( auto a, auto b ) { return ( a - b ) * ( a - b ); } );
                if ( error < min_error ) {
                    min_error = error;
                    output = j;
                }
            }
            if ( output == answers[ i ] )
                hash_correct++;
            else
                hash_wrong++;
        }
        auto end = std::chrono::high_resolution_clock::now();
        elapsed_time += std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
    }

    if ( !output_csv ) {
        fmt::print( "Accuracy : {}/{} ({:.2f}), Total Elapsed Time : {}us\n", hash_correct, input.size(), static_cast<double>( hash_correct ) / input.size(), elapsed_time );
        fmt::print( "First Hit : {}/{}\n", hash_hit_multi + hash_hit_multi, input.size() );
    } else {
        fmt::print( "{:.2f}, {}, {}, {}, {}, {}, {}, {}\n", proj_margin, hash_hit_single, hash_hit_multi, hash_not_found, hash_correct, hash_wrong, hash_multi_count, elapsed_time );
    }
    std::cout.flush();

    return;
}

void benchmark_random( int sample, int db_size, int feature_length, float max_noise, float proj_margin ) {
    Trie trie;
    std::vector<std::vector<float>> database( db_size + 1 );
    for ( int i = 1; i <= db_size; ++i ) {
        auto temp = Util::random_vector( feature_length );
        database[ i ] = temp;
        auto quantized = Util::quantize_feature( temp, proj_margin );
        trie.insert( quantized, i ); // 예시로, i번째 data는 i값을 가짐.
    }

    std::vector<std::vector<float>> input( sample );
    std::vector<int> answers( sample );
    for ( int i = 0; i < sample; ++i ) {
        answers[ i ] = Util::ranInt( 1, db_size );
        input[ i ] = Util::random_noise( database[ answers[ i ] ], max_noise );
    }
    std::cout << "Trie Build Done!" << std::endl;
    benchmark_naive( database, input, answers );
    benchmark_trie( trie, database, input, answers, proj_margin );
    return;
}

void benchmark_real( float proj_margin, bool output_csv ) {
    if ( !output_csv )
        fmt::print( "Benchmark with margin : {}\n", proj_margin );
    Trie trie;
    std::vector<std::vector<float>> database( 10021 );
    for ( int i = 1; i <= 20; ++i ) {
        auto jacket = cv::imread( fmt::format( "data/{}/jacket.png", i ) );
        auto feature = Feature::generate( jacket );
        database[ i ] = feature;
        auto quantized = Util::quantize_feature( feature, proj_margin );
        trie.insert( quantized, i ); // 예시로, i번째 data는 i값을 가짐.
    }
    for ( int i = 0; i < 10000; ++i ) {
        int feature_length = database[ 1 ].size();
        auto temp = Util::random_vector( feature_length );
        database[ i + 21 ] = temp;
        auto quantized = Util::quantize_feature( temp, proj_margin );
        trie.insert( quantized, i + 21 ); // 예시로, i번째 data는 i값을 가짐.
    }
    if ( !output_csv )
        std::cout << "Trie Build Done!" << std::endl;

    std::vector<std::vector<float>> input;
    std::vector<int> answers;
    for ( int i = 1; i <= 20; ++i ) {
        std::filesystem::directory_iterator it( std::filesystem::current_path() / "data" / fmt::format( "{}", i ).c_str() );
        while ( it != std::filesystem::end( it ) ) {
            const std::filesystem::directory_entry &entry = *it;
            if ( entry.path().filename().extension().compare( ".jpg" ) == 0 ) {
                auto jacket = cv::imread( entry.path().string() );
                auto feature = Feature::generate( jacket );
                input.push_back( feature );
                answers.push_back( i );
            }
            ++it;
        }
    }
    benchmark_naive( database, input, answers, output_csv );
    if ( output_csv )
        freopen( fmt::format( "result_trie.csv", proj_margin ).c_str(), "w", stdout );
    for ( float margin = 0.0f; margin <= proj_margin; margin += 0.02f ) {
        benchmark_trie( trie, database, input, answers, margin, output_csv );
    }
    return;
}