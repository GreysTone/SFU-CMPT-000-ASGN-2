cmake_minimum_required(VERSION 3.3)
project(CMPT361_ASGN2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    include/Angel.h
    include/CheckError.h
    include/InitShader.cpp
    include/mat.h
    include/vec.h
    fshader.glsl
    global.h
    image_util.cpp
    image_util.h
    Makefile
    raycast.cpp
    scene.cpp
    scene.h
    sphere.cpp
    sphere.h
    trace.cpp
    trace.h
    vector.cpp
    vector.h
    vshader.glsl)

add_executable(CMPT361_ASGN2 ${SOURCE_FILES})