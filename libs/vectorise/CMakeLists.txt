#
# F E T C H   V E C T O R I S E   L I B R A R Y
#
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(fetch-vectorise)

# CMake Configuration
include(${FETCH_ROOT_CMAKE_DIR}/BuildTools.cmake)

# Compiler Configuration
setup_compiler()

# ------------------------------------------------------------------------------
# Main Library Target
# ------------------------------------------------------------------------------

setup_library(fetch-vectorise)
target_link_libraries(fetch-vectorise PUBLIC fetch-meta fetch-math pthread)

# Define all the test targets
add_test_target()
add_benchmark_target()

# Define all the example targets
add_subdirectory(examples)
