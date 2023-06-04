#include "feature.hpp"

namespace Feature {
template <int N>
std::function<std::vector<float>( cv::Mat & )> mean_RGB_NxN = []( cv::Mat &img ) {
    std::vector<float> output( 3 * N * N ); // [B,G,R][width][height]
    auto [ width, height ] = img.size();
    for ( int c = 0; c < 3; ++c ) {
        for ( int x_offset = 0; x_offset < N; ++x_offset ) {
            for ( int y_offset = 0; y_offset < N; ++y_offset ) {
                float sum = 0;
                for ( int x = 0; x < width / N; ++x ) {
                    for ( int y = 0; y < height / N; ++y ) {
                        sum += img.at<cv::Vec3b>( y_offset * height / N + y, x_offset * width / N + x )[ c ] / 255.0f;
                    }
                }
                output[ c * N * N + x_offset * N + y_offset ] = sum / ( width / N * height / N );
            }
        }
    }
    return output;
};

template <int N>
std::function<std::vector<float>( cv::Mat & )> std_RGB_NxN = []( cv::Mat &img ) {
    std::vector<float> output( 3 * N * N ); // [B,G,R][width][height]
    auto [ width, height ] = img.size();
    for ( int c = 0; c < 3; ++c ) {
        for ( int x_offset = 0; x_offset < N; ++x_offset ) {
            for ( int y_offset = 0; y_offset < N; ++y_offset ) {
                float sum_of_square = 0;
                float sum = 0;
                for ( int x = 0; x < width / N; ++x ) {
                    for ( int y = 0; y < height / N; ++y ) {
                        sum += img.at<cv::Vec3b>( y_offset * height / N + y, x_offset * width / N + x )[ c ] / 255.0f;
                        sum_of_square += std::pow( img.at<cv::Vec3b>( y_offset * height / N + y, x_offset * width / N + x )[ c ] / 255.0f, 2 );
                    }
                }
                output[ c * N * N + x_offset * N + y_offset ] = std::sqrt( sum_of_square / ( width / N * height / N ) - std::pow( sum / ( width / N * height / N ), 2 ) );
            }
        }
    }
    return output;
};

// get n-th significant blob feature : WIP
std::function<std::vector<float>( cv::Mat &, int )> blob_feature = []( cv::Mat &img, int n ) {
    std::vector<float> output( 3 ); // (x,y,size)
    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create();

    std::vector<cv::KeyPoint> keypoints;
    detector->detect( img, keypoints );
    return output;
};

// get n-th significant corner
template <int N>
std::function<std::vector<float>( cv::Mat & )> corner_feature = []( cv::Mat &img ) {
    std::vector<float> output( 2 ); // x,y
    // corner detect using Harris, find N-th significant eigenvalue
    cv::Mat gray;
    cv::cvtColor( img, gray, cv::COLOR_BGR2GRAY );
    cv::Mat cornerResponse;
    cv::cornerHarris( gray, cornerResponse, 3, 3, 0.04 );
    cv::normalize( cornerResponse, cornerResponse, 0, 1, cv::NORM_MINMAX );

    double threshold = 0.01;
    cv::Mat cornerMask = cornerResponse > threshold;

    cv::Mat cornerCoordinates;
    cv::findNonZero( cornerMask, cornerCoordinates );

    std::vector<cv::Point> cornerPoints;
    for ( int i = 0; i < cornerCoordinates.total(); ++i ) {
        cv::Point pt = cornerCoordinates.at<cv::Point>( i );
        cornerPoints.push_back( pt );
    }

    std::sort( cornerPoints.begin(), cornerPoints.end(), [ &cornerResponse ]( cv::Point a, cv::Point b ) {
        return cornerResponse.at<float>( a ) > cornerResponse.at<float>( b );
    } );

    cv::Point nthCorner = cornerPoints[ N ];

    output[ 0 ] = std::clamp( nthCorner.x / static_cast<float>( img.cols ), 0.0f, 1.0f );
    output[ 1 ] = std::clamp( nthCorner.y / static_cast<float>( img.rows ), 0.0f, 1.0f );
    return output;
};

std::vector<float> generate( cv::Mat &img ) {
    auto features = {
        mean_RGB_NxN<2>,
        mean_RGB_NxN<3>,
        mean_RGB_NxN<4>,
        mean_RGB_NxN<5>,
        std_RGB_NxN<2>,
        std_RGB_NxN<3>,
        std_RGB_NxN<4>,
        std_RGB_NxN<5> };
    std::vector<float> output;
    for ( auto &feature : features ) {
        auto [ width, height ] = img.size();
        auto feature_output = feature( img );
        output.insert( output.end(), feature_output.begin(), feature_output.end() );
    }
    return output;
}
}; // namespace Feature