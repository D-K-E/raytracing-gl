cmake_minimum_required (VERSION 3.0.2)
project("OpenGL Raytracing")

find_package(OpenGL REQUIRED)

include_directories(
    # my headers
    "./include/"
    )

set( GLFW_SHARED_LIB
    "${PROJECT_SOURCE_DIR}/libs/glfw/src/libglfw.so")

set (CMAKE_CXX_FLAGS "-std=c++17")

set (FLAGS "-ldl -ggdb -Wall -Wextra")

set ( ALL_LIBS
    ${OpenGL}
    ${GLFW_SHARED_LIB}
    #${ASSIMP_SHARED_LIB}
    ${FLAGS}
    )

add_executable(compute01.out 
    "src/glad.c"
    "src/window.hpp"
    "src/compute01.cpp"
    )
add_executable(compute02.out 
    "src/glad.c"
    "src/window.hpp"
    "src/compute02.cpp"
    )
add_executable(compute03.out 
    "src/glad.c"
    "src/window.hpp"
    "src/compute03.cpp"
    )
add_executable(compute04.out 
    "src/glad.c"
    "src/window.hpp"
    "src/compute04.cpp"
    )
add_executable(compute05.out 
    "src/glad.c"
    "src/window.hpp"
    "src/compute05.cpp"
    )

target_link_libraries(compute01.out ${ALL_LIBS})
target_link_libraries(compute02.out ${ALL_LIBS})
target_link_libraries(compute03.out ${ALL_LIBS})
target_link_libraries(compute04.out ${ALL_LIBS})
target_link_libraries(compute05.out ${ALL_LIBS})

install(TARGETS compute01.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
install(TARGETS compute02.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
install(TARGETS compute03.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
install(TARGETS compute04.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
install(TARGETS compute05.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
