cmake_minimum_required(VERSION 3.16)
project(game_hearts VERSION 0.1.0 LANGUAGES C CXX)

# add_executable(game_hearts main.cpp)

# include(CTest)
# enable_testing()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 
option(BUILD_SHARED_LIBS "Build shared libraries" OFF) 

include(FetchContent) 
FetchContent_Declare(SFML 
    GIT_REPOSITORY https://github.com/SFML/SFML.git 
    GIT_TAG 2.6.x) 
FetchContent_MakeAvailable(SFML) 
 
add_executable(game_hearts main.cpp ) 
target_link_libraries(game_hearts sfml-graphics) 
target_compile_features(game_hearts PRIVATE cxx_std_17) 