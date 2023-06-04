#ifndef UTIL_HPP
#define UTIL_HPP

#include <bits/stdc++.h>

namespace Util {
// 두 feature vector의 거리를 구하는 함수
float distance( std::vector<float> &v1, std::vector<float> &v2 );
// [s,e] 에서 uniform distribution으로 랜덤하게 정수 하나 뽑기
int ranInt( int s, int e );
// feature vector를 string으로 변환
std::string feature2str( std::vector<float> &v, float margin = 0.0f );
// float feature vector(N - size) to binary feature vector(2N - size)
std::vector<bool> quantize_feature( std::vector<float> &feature, float margin = 0.1f );
// [minimum, maximum] 에서 uniform distribution으로 랜덤하게 실수 size개 뽑기
std::vector<float> random_vector( size_t size = 20, float minimum = 0.0f, float maximum = 1.0f );
// original vector에서 각 원소마다 [-max_noise, max_noise]만큼의 variation
std::vector<float> random_noise( std::vector<float> &original, float max_noise = 0.2f );
} // namespace Util

#endif // UTIL_HPP