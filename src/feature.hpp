#ifndef FEATURE_HPP
#define FEATURE_HPP

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

namespace Feature {
// get average color of a region - 3 by 3 section
std::vector<float> generate( cv::Mat &img );
}; // namespace Feature

#endif // FEATURE_HPP