cmake_minimum_required(VERSION 3.7)

    project(superficies)

    set(CMAKE_CXX_STANDARD 14)

    # GLUT
    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    if(NOT GLUT_FOUND)
        message(ERROR "GLUT not found!")
    endif(NOT GLUT_FOUND)

    # OpenGL
    find_package(OpenGL REQUIRED)
    include_directories(${OpenGL_INCLUDE_DIRS})
    link_directories(${OpenGL_LIBRARY_DIRS})
    add_definitions(${OpenGL_DEFINITIONS})
    if(NOT OPENGL_FOUND)
        message(ERROR "OPENGL not found!")
    endif(NOT OPENGL_FOUND)

    include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
    )

    file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )

    add_executable(superficies ${all_SRCS})
    target_link_libraries(superficies -lm ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})