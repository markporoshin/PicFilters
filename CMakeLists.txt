cmake_minimum_required(VERSION 3.15)
project(ClassworkOpenGL C)

set(CMAKE_C_STANDARD 99)

if (APPLE)
    include_directories(/System/Library/Frameworks)
    find_library(GLUT_LIBRARY GLUT)
    find_library(OpenGl_LIBRARY OpenGL)
    mark_as_advanced(GLUT_LIBRARY OpenGl_LIBRARY)
    set(EXTRA_LIBS ${GLUT_LIBRARY} ${OpenGl_LIBRARY})
ENDIF(APPLE)

add_executable(ClassworkOpenGL main.c pic.c pic.h LUT.c LUT.h LINFIL.c LINFIL.h NLFIL.c NLFIL.h)
target_link_libraries(ClassworkOpenGL ${EXTRA_LIBS})