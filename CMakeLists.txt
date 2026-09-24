# Initialize CMake
cmake_minimum_required(VERSION 3.16)
project(DLC CXX)

# Set C++ compiler version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)

# Find and initialize external libraries
set(SDL_X11_XTEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/extern/SDL EXCLUDE_FROM_ALL)
set(SDLTTF_VENDORED OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/extern/SDL_ttf EXCLUDE_FROM_ALL)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/mavsdk-install)
find_package(MAVSDK REQUIRED)

# Create executable with source code
file(GLOB_RECURSE SOURCE ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(DLC ${SOURCE})

# Include executable headers
target_include_directories(DLC PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link external libraries to executable
target_link_libraries(DLC PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        MAVSDK::mavsdk
)
target_compile_definitions(DLC PRIVATE SDL_MAIN_HANDLED)
