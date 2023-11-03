cmake_minimum_required(VERSION 3.10)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/sdl2")

include_directories(.)
set(THREADS_PREFER_PTHREAD_FLAG ON)
#option(WIN32 "Build win32 using mingw" OFF)
#option(WIN64 "Build win64 using mingw" OFF)
#option(Debug "Build Debug Version" OFF)
#option(Profile "Build Profile Version" OFF)
#option(Release "Build Release Linux Version" OFF)

# Inspiration: https://github.com/Plethora777/mcpe_viz/blob/master/CMakeLists.txt
#if(WIN32)
#    # Win32 Build
#    add_definitions(-DWIN32)
#
#elseif(WIN64)
#    # Win64 Build
#    add_definitions(-DWIN32 -DWIN64)
#    if(MINGW)
#
#    endif()
#
#else()
#    # Linux/MacOS build
#
#endif()
#
#find_program(SDL2_CONFIG NAMES sdl2-config)
#if (NOT SDL2_CONFIG)
#    message( FATAL_ERROR "Failed to locate sdl2-config." )
#else()
#    message( STATUS "Found sdl2-config: ${SDL2_CONFIG}" )
#endif()
#exec_program(${SDL2_CONFIG} ARGS --libs OUTPUT_VARIABLE SDL2_LIBRARIES)




#cmake_minimum_required(VERSION 3.5)
#project(mygame)

# Create an option to switch between a system sdl library and a vendored sdl library
#option(MYGAME_VENDORED "Use vendored libraries" OFF)

#if(MYGAME_VENDORED)
#    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
#else()
    # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
#    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    # 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
#    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
#endif()

# Create your game executable target as usual
#add_executable(mygame WIN32 mygame.c)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
#if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
#    target_link_libraries(ChessEngine PRIVATE SDL2::SDL2main)
#endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
#target_link_libraries(ChessEngine PRIVATE SDL2::SDL2)




add_subdirectory("./libcuckoo" libcuckoo)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lSDL2 -lSDL2_image -lSDL2_ttf")

add_executable(ChessEngine
        bitboard.cpp bitboard.h
        cli.cpp cli.h
        CMakeLists.txt
        console.cpp console.h
        coutsync.h
        debug.cpp debug.h
        enginetools.cpp enginetools.h
        main.cpp option.h
        position.cpp position.h
        thread.cpp thread.h
        types.cpp types.h
        uci.cpp uci.h
        search.cpp search.h
        evaluation.cpp evaluation.h
        movegen.cpp movegen.h
        move.cpp move.h
        zhash.cpp zhash.h
        transpositiontable.cpp transpositiontable.h
        window.cpp window.h graphics.cpp graphics.h
        libcuckoo moodycamel-concurrentqueue/concurrentqueue.h debugging.h button.h button.cpp thread_win32.h)

#INCLUDE(FindPkgConfig)
#pkg_check_modules(SDL2 REQUIRED sdl2)
#pkg_check_modules(SDL2_IMG REQUIRED SDL2_image)
#pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

#include_directories(${JUNCTION_ALL_INCLUDE_DIRS})
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS}
#        ${SDL2_IMG_INCLUDE_DIRS}
#        ${SDL2_TTF_INCLUDE_DIRS})

#link_directories (${SDL2_LIBRARY_DIRS}
#        ${SDL2_IMG_LIBRARY_DIRS}
#        ${SDL2_TTF_LIBRARY_DIRS})

find_package(Threads REQUIRED)

target_link_libraries(ChessEngine Threads::Threads SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
#        ${SDL2_LIBRARIES}
#        ${SDL2_IMG_LIBRARIES}
#        ${SDL2_TTF_LIBRARIES})

add_custom_command(TARGET ChessEngine PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/sdlroot $<TARGET_FILE_DIR:ChessEngine>)

add_custom_command(TARGET ChessEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/SDL2_x64_dlls $<TARGET_FILE_DIR:ChessEngine>)

#add_custom_command(TARGET ChessEngine PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/bin $<TARGET_FILE_DIR:ChessEngine>)

