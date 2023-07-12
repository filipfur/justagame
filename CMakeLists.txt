cmake_minimum_required(VERSION 3.22) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lithium-app)

include_directories(
    include
)

file(GLOB SOURCES
    src/*.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

file(GLOB TEST_SOURCES
    src/game.cpp
    test/*.cpp)

add_executable(test_game ${TEST_SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} lithium)

add_subdirectory(lithium)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
install(TARGETS test_game DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)