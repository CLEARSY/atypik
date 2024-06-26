include_directories(
    ${atypik_SOURCE_DIR}/3rdparty/tinyxml2
    ${atypik_SOURCE_DIR}/btypes/include
    ${atypik_SOURCE_DIR}/io/include
    ${atypik_SOURCE_DIR}/belements/include
    ${atypik_SOURCE_DIR}/solver/include
    )

add_library(IO
    bxmlparser.cpp
    bxmlposition.cpp
    context.cpp
    machinetypes.cpp
    parser.cpp
    pogparser.cpp
    writer.cpp
    )

target_link_libraries(IO TINYXML2 BElements Tools)