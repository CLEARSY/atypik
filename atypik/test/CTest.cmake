enable_testing()

set (BxmlTestFolders
    "test_arithmetic"
    "test_explicit_typing"
    "test_functions"
    "test_instructions"
    "test_instructions"
    "test_instructions"
    "test_operations"
    "test_operations"
    "test_pairs"
    "test_parameters"
    "test_relations"
    "test_sets"
    "test_sets"
    )

set(BxmlToTest
   "Arithmetic"
   "ExplicitTyping"
   "Functions"
   "Instructions"
   "Instructions_r"
   "Instructions_i"
   "Operations"
   "Operations_i"
   "Pairs"
   "Parameters"
   "Relation"
   "Sets"
   "Sets_i"
   )

list(LENGTH BxmlTestFolders nbTests)
math(EXPR range "${nbTests} - 1")

foreach (index RANGE ${range})
    list(GET BxmlTestFolders ${index} name)
    list(GET BxmlToTest ${index} machine)
    set(folder "${CMAKE_SOURCE_DIR}/test/data/${name}")
    add_test(NAME "${name}-${machine}"
        COMMAND types-comparator "${folder}/src/${machine}.bxml" "--bxml" "${folder}/expected/${machine}.csv" "${folder}/src/")
endforeach()
