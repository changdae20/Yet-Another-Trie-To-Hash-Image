#include "util.hpp"

namespace Util {
// 두 feature vector의 거리를 구하는 함수
float distance( std::vector<float> &v1, std::vector<float> &v2 ) {
    return std::inner_product( v1.begin(), v1.end(), v2.begin(), 0.0f, std::plus<>(), []( auto a, auto b ) {
        return ( a - b ) * ( a - b );
    } );
}

// [s,e] 에서 uniform distribution으로 랜덤하게 정수 하나 뽑기
int ranInt( int s, int e ) {
    std::random_device rd;
    std::mt19937 gen( rd() );
    std::uniform_int_distribution<int> dis( s, e );
    return dis( gen );
}

// feature vector를 string으로 변환
std::string feature2str( std::vector<float> &v, float margin ) {
    std::string ret;
    for ( int i = 0; i < v.size(); ++i ) {
        if ( margin > 0.0f ) { // margin을 고려할 경우
            if ( 0.5 - margin <= v[ i ] && v[ i ] <= 0.5 + margin )
                ret.push_back( '?' );
            else if ( v[ i ] < 0.5 - margin )
                ret.push_back( '0' );
            else
                ret.push_back( '1' );
        } else { // margin을 고려하지 않을 경우
            if ( v[ i ] < 0.5 )
                ret.push_back( '0' );
            else
                ret.push_back( '1' );
        }
    }
    return ret;
}

// float feature vector(N - size) to binary feature vector(2N - size)
std::vector<bool> quantize_feature( std::vector<float> &feature, float margin ) {
    assert( margin <= 0.5 );
    std::vector<bool> quantized( 2u * feature.size(), false );
    for ( int i = 0; i < feature.size(); ++i ) {
        quantized[ 2 * i ] = ( feature[ i ] >= 0.5 );
        if ( ( 0.5 - margin ) <= feature[ i ] && feature[ i ] <= ( 0.5 + margin ) ) {
            quantized[ 2 * i + 1 ] = true; // 2i+1번째가 true면 해당 feature값이 0.5기준 +- margin 범위 안쪽이라 애매해서 ?(unknown value라는 뜻.
        }
    }
    return quantized;
}

// [minimum, maximum] 에서 uniform distribution으로 랜덤하게 실수 size개 뽑기
std::vector<float> random_vector( size_t size, float minimum, float maximum ) {
    std::vector<float> ans( size );
    std::random_device rd;
    std::mt19937 gen( rd() );
    std::uniform_real_distribution<float> distribution( minimum, maximum );
    for ( auto &el : ans )
        el = distribution( gen );
    return ans;
}

// original vector에서 각 원소마다 [-max_noise, max_noise]만큼의 variation
std::vector<float> random_noise( std::vector<float> &original, float max_noise ) {
    std::vector<float> copied( original );
    auto noise = random_vector( original.size(), -max_noise, max_noise );
    for ( int i = 0; i < original.size(); ++i )
        copied[ i ] = std::clamp( copied[ i ] + noise[ i ], 0.0f, 1.0f );
    return copied;
}
} // namespace Util