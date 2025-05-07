cmake_minimum_required(VERSION 3.30)
project(Reprocuctor_Musica)

set(CMAKE_CXX_STANDARD 20)

add_executable(Reprocuctor_Musica main.cpp
        src/Reproductor.cpp
        src/Reproductor.h)

target_include_directories(Reprocuctor_Musica
        PRIVATE ${PROJECT_SOURCE_DIR}/include
)