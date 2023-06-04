################## OPENCV #####################
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})
###############################################

################## fmt #################
find_package(fmt REQUIRED)
########################################

SET(UTIL
    benchmark.hpp
    benchmark.cpp
    feature.hpp
    feature.cpp
    trie.hpp
    util.hpp
    util.cpp
)

add_library(${PROJECT_NAME}
    ${UTIL}
)

link_directories(${PROJECT_NAME})

add_executable(main
	${PROJECT_SOURCE_DIR}/src/main.cpp
    ${UTIL}
)

target_link_libraries(main
    ${OpenCV_LIBS}
    fmt::fmt
)

###################################################################################################
# - library targets -------------------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${OpenCV_LIBS}
    ${UTIL}
    fmt::fmt
)