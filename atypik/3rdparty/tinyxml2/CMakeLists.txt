project(TINYXML2)

set(CMAKE_CXX_STANDARD 11)

set(TINYXML2_SRC
    tinyxml2.cpp
)

set(TINYXML2_HEADERS
    tinyxml2.h
)

add_library(TINYXML2 ${TINYXML2_SRC} ${TINYXML2_HEADERS})
