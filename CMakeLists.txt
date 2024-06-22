cmake_minimum_required(VERSION 3.16)
project(proj LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(proj main.cpp)
target_link_libraries(proj sfml-graphics)
target_compile_features(proj PRIVATE cxx_std_17)

# if(WIN32)
#     add_custom_command(
#         TARGET proj
#         COMMENT "Copy OpenAL DLL"
#         PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
#         VERBATIM)
# endif()
# cmake_minimum_required(VERSION 3.16)
# project(proj LANGUAGES CXX)

# # Устанавливаем стандарт C++
# set(CMAKE_CXX_STANDARD 17)

# # Указываем директории с заголовочными файлами SFML
# include_directories(
#     # ${CMAKE_CURRENT_SOURCE_DIR}
#     /C:/Users/Sopha/Downloads/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/include 
# )

# # Указываем директорию с библиотеками SFML
# link_directories(
#     /C:/Users/Sopha/Downloads/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/lib
# )
# add_executable(proj main.cpp)

# # Подключение библиотек SFML
# target_link_libraries(proj sfml-graphics)

# # target_compile_options(main PRIVATE -std=c++17) 

# # Устанавливаем стандарт C++ для проекта
# target_compile_features(proj PRIVATE cxx_std_17)

# # Копирование OpenAL DLL (если нужно)
# if(WIN32)
#     add_custom_command(
#         TARGET proj
#         COMMENT "Copy OpenAL DLL"
#         PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sfml/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:proj>
#         VERBATIM)
# endif() 
