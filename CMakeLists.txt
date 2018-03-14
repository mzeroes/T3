cmake_minimum_required(VERSION 3.8)

project(T3)

set(CMAKE_CXX_STANDARD 11)

include_directories(src/include)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)


#Avoid multiple mains for targets
list(REMOVE_ITEM SRC_FILES  ${PROJECT_SOURCE_DIR}/src/main.cpp)


#file(GLOB TEST_FILES ${PROJECT_SOURCE_DIR}/src/test/*.cpp)


add_executable(T3 src/main.cpp ${SRC_FILES})

# TEST DIR
add_subdirectory(test)

