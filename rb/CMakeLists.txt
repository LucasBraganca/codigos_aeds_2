cmake_minimum_required(VERSION 3.16)

project(rb C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Biblioteca contendo o TAD
add_library(rb
    src/rb.c
)

target_include_directories(rb
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Programa principal
add_executable(programa
    src/main.c
)

target_link_libraries(programa
    PRIVATE
        rb
)

# Testes
enable_testing()

add_executable(rb_test
    test/rb_test.c
)

target_link_libraries(rb_test
    PRIVATE
        rb
)

# Cada caso e independente para uma falha nao ocultar os demais.
foreach(caso propriedades criar operacoes remocoes_simples transplantar
             insercao_pai_preto remocao_dois_filhos)
    add_test(NAME RB_${caso} COMMAND rb_test ${caso})
endforeach()

set(casos_insercao raiz_vermelha tio_vermelho triangulo linha)
set(indice 0)
foreach(caso IN LISTS casos_insercao)
    foreach(espelho RANGE 0 1)
        add_test(NAME RB_insercao_${caso}_espelho_${espelho}
                 COMMAND rb_test insercao ${indice} ${espelho})
    endforeach()
    math(EXPR indice "${indice} + 1")
endforeach()

set(casos_remocao irmao_vermelho irmao_preto_sobrinhos_pretos
    sobrinho_proximo_vermelho sobrinho_distante_vermelho propagacao)
set(indice 1)
foreach(caso IN LISTS casos_remocao)
    foreach(espelho RANGE 0 1)
        foreach(direto RANGE 0 1)
            add_test(NAME RB_remocao_${caso}_espelho_${espelho}_direto_${direto}
                     COMMAND rb_test remocao ${indice} ${espelho} ${direto})
        endforeach()
    endforeach()
    math(EXPR indice "${indice} + 1")
endforeach()

foreach(espelho RANGE 0 1)
    add_test(NAME RB_insercao_cascata_espelho_${espelho}
             COMMAND rb_test insercao_cascata ${espelho})
endforeach()
