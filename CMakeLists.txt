cmake_minimum_required(VERSION 3.16)
project(drkbrg C)

# Force C11 compliance
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Look for SDL3 package
find_package(SDL3 REQUIRED)

# Point to the source file inside the src directory
add_executable(drkbrg 
    src/drkbrg.c
    src/game.c
    src/ui.c)

# Compiler switches for pedantic errors
if(MSVC)
    # For Visual Studio compiler
    target_compile_options(drkbrg PRIVATE /W4 /WX /wd4100 /wd4101)
else()
    # For GCC and Clang
    target_compile_options(drkbrg PRIVATE 
        -O3
        -Wall 
        -Wextra 
        -Wpedantic 
        -Werror 
        -Wno-unused-variable 
        -Wno-unused-parameter
    )
endif()

# Link SDL3 library
target_link_libraries(drkbrg PRIVATE SDL3::SDL3)