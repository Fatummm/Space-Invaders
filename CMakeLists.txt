cmake_minimum_required(VERSION 3.28)
project(SpaceInvaders)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)


add_library(Base src/base.cpp)
add_library(Entities src/entities.cpp)
add_library(Interface src/interface.cpp)


if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using GCC compiler")
    add_compile_options(-Wall -Wextra -O2)
endif()


add_executable(Game src/main.cpp)
set_target_properties(Game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

target_link_libraries(Game Base Entities Interface)
target_link_libraries(Game sfml-graphics sfml-window sfml-system)