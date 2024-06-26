cmake_minimum_required(VERSION 3.5)

project(atypik LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(atypik main.cpp)

#set(GPROF_FLAG "-pg")
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GPROF_FLAG}")
#set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GPROF_FLAG}")

#set(TBB_FLAG "-ltbb")
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${TBB_FLAG}")
#set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${TBB_FLAG}")

target_include_directories(atypik PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/smt-switch/include")

add_subdirectory(3rdparty)
add_subdirectory(belements)
add_subdirectory(io)
add_subdirectory(solver)
add_subdirectory(tools)

enable_testing()
add_subdirectory(test)

include_directories(
    ${atypik_SOURCE_DIR}/3rdparty/tinyxml2
    ${atypik_SOURCE_DIR}/io/include
    ${atypik_SOURCE_DIR}/solver/include
    ${atypik_SOURCE_DIR}/belements/include
    ${atypik_SOURCE_DIR}/tools/include
    )
link_directories(
    ${atypik_SOURCE_DIR}/io/src
    ${atypik_SOURCE_DIR}/solver/src
    ${atypik_SOURCE_DIR}/belements/src
    ${atypik_SOURCE_DIR}/tools/src
    )

target_link_libraries(atypik TINYXML2 IO BElements Solver Tools)

target_link_directories(atypik PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/smt-switch/build")
target_link_directories(atypik PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/smt-switch/build/cvc5")
target_link_libraries(atypik libsmt-switch.so)
target_link_libraries(atypik libsmt-switch-cvc5.so)




