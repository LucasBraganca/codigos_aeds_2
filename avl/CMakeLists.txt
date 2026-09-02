cmake_minimum_required(VERSION 3.16)

project(avl C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Biblioteca contendo o TAD
add_library(avl
    src/avl.c
)

target_include_directories(avl
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Programa principal
add_executable(programa
    src/main.c
)

target_link_libraries(programa
    PRIVATE
        avl
)

# Testes
enable_testing()

add_executable(avl_test
    test/avl_test.c
)

target_link_libraries(avl_test
    PRIVATE
        avl
)

add_test(
    NAME BSTTests
    COMMAND avl_test
)
