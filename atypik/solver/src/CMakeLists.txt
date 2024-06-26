include_directories(
    ${atypik_SOURCE_DIR}/solver/include
    ${atypik_SOURCE_DIR}/btypes/include
    ${atypik_SOURCE_DIR}/3rdparty/smt-switch/include
    )

add_library(Solver
    assertion.cpp
    btypes.cpp
    constraint.cpp
    error.cpp
    solverfactory.cpp
    model.cpp
    modelset.cpp
    vargen.cpp
    )

    target_link_libraries(Solver BElements)
