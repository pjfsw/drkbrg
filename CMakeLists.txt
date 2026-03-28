cmake_minimum_required(VERSION 3.16)
project(drkbrg C)

# Force C11 compliance
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Look for SDL3 package
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

# --- PIXEL ENGINE (Independent Library) ---
add_library(PixelEngine STATIC 
    pixelengine/ui.c 
    pixelengine/ui.h
    pixelengine/pixelengine.c
    pixelengine/pixelengine.h
)

# Tell Engine where its own headers are, and export that path to consumers
target_include_directories(PixelEngine PUBLIC pixelengine)

# Link SDL3 to the Engine
target_link_libraries(PixelEngine PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)

# Point to the source file inside the src directory
add_executable(drkbrg 
    src/drkbrg.c
    src/game.c)

# Game needs to see Game headers
target_include_directories(drkbrg PRIVATE src)

# Enforce dependency: Game -> Engine
# This automatically allows #include "ui.h" in game.c
target_link_libraries(drkbrg PRIVATE PixelEngine)

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

