cmake_minimum_required(VERSION 3.16)

project(bst C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Biblioteca contendo o TAD
add_library(bst
    src/bst.c
)

target_include_directories(bst
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Programa principal
add_executable(programa
    src/main.c
)

target_link_libraries(programa
    PRIVATE
        bst
)

# Testes
enable_testing()

add_executable(bst_test
    test/bst_test.c
)

target_link_libraries(bst_test
    PRIVATE
        bst
)

add_test(
    NAME BSTTests
    COMMAND bst_test
)
