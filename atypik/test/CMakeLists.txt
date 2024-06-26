cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(template-generator templategenerator.cpp)
add_executable(types-comparator typescomp.cpp)

target_include_directories(template-generator PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/smt-switch/include")
target_link_directories(template-generator PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/smt-switch/build")
target_link_directories(template-generator PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/smt-switch/build/cvc5")

target_include_directories(types-comparator PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/smt-switch/include")
target_link_directories(types-comparator PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/smt-switch/build")
target_link_directories(types-comparator PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/smt-switch/build/cvc5")


include_directories(
    ${atypik_SOURCE_DIR}/3rdparty/tinyxml2
    ${atypik_SOURCE_DIR}/io/include
    ${atypik_SOURCE_DIR}/solver/include
    ${atypik_SOURCE_DIR}/belements/include
    ${atypik_SOURCE_DIR}/test/include
    )
link_directories(
    ${atypik_SOURCE_DIR}/io/src
    ${atypik_SOURCE_DIR}/solver/src
    ${atypik_SOURCE_DIR}/belements/src
    ${atypik_SOURCE_DIR}/test/src
    )

target_link_libraries(template-generator IO)
target_link_libraries(template-generator BElements)
target_link_libraries(template-generator Solver)
target_link_libraries(template-generator TINYXML2)
target_link_libraries(template-generator Test)

target_link_libraries(template-generator libsmt-switch.so)
target_link_libraries(template-generator libsmt-switch-cvc5.so)

target_link_libraries(types-comparator IO)
target_link_libraries(types-comparator BElements)
target_link_libraries(types-comparator Solver)
target_link_libraries(types-comparator TINYXML2)
target_link_libraries(types-comparator Test)

target_link_libraries(types-comparator libsmt-switch.so)
target_link_libraries(types-comparator libsmt-switch-cvc5.so)

add_subdirectory(src)

include(CTest.cmake)


