cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")

project(voiceapp)

include(cmake/flags.cmake)
include(cmake/dependencies.cmake)

option(RUN_TESTS "Creates and runs unit tests for the project" OFF)

if(RUN_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

include_directories(inc)
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

add_dependencies(${PROJECT_NAME} libtts)
add_dependencies(${PROJECT_NAME} libstt)
add_dependencies(${PROJECT_NAME} libgptchat)
add_dependencies(${PROJECT_NAME} libshellcmd)

target_link_libraries(${PROJECT_NAME}
    tts
    stt
    gptchat
    shellcmd
    ${Boost_LIBRARIES}
)
