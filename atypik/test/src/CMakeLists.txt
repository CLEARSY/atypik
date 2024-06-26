include_directories(
    ${atypik_SOURCE_DIR}/3rdparty/tinyxml2
    ${atypik_SOURCE_DIR}/io/include
    ${atypik_SOURCE_DIR}/test/include
    )

add_library(Test
    csverror.cpp
    csvreader.cpp
    csvwriter.cpp
    )

target_link_libraries(Test TINYXML2 IO)
