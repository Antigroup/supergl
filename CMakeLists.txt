cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(cotire)

project(supergl)

if(WIN32)
    if(NOT WIN_ROOT)
        set(WIN_ROOT "C:/local")
    endif(NOT WIN_ROOT)
    if(CMAKE_CL_64)
        set(CMAKE_LIBRARY_ARCHITECTURE "x64")
    else(CMAKE_CL_64)
        set(CMAKE_LIBRARY_ARCHITECTURE "x86")
    endif(CMAKE_CL_64)
    set(CMAKE_PREFIX_PATH ${WIN_ROOT})
    
    if(NOT PYTHON_INC_DIR)
        set(PYTHON_INC_DIR "C:/Python34/include")
    endif(NOT PYTHON_INC_DIR)
    if(NOT PYTHON_LIB_DIR)
        set(PYTHON_LIB_DIR "C:/Python34/libs")
    endif(NOT PYTHON_LIB_DIR)
endif(WIN32)

find_library(assimp_LIB
    assimp
    PATHS ${ASSIMP_LIB_DIR}
    )
    
find_library(DevIL_LIB
    DevIL IL
    PATHS ${DEVIL_LIB_DIR}
    )

#*Should* work for RPI, Windows, and Linux Desktops
find_library(OpenGL_LIB
    glew32 GLEW GLESv2
    PATHS ${OPENGL_LIB_DIR}
    )
    
find_library(Python_LIB
    python34
    PATHS ${PYTHON_LIB_DIR}
    )
    
find_library(SDL2_LIB
    SDL2
    PATHS ${SDL_LIB_DIR}
    )
    
find_library(SDL2main_LIB
    SDL2main
    PATHS ${SDL_LIB_DIR}
    )
    
find_path(Python_INC
    Python.h
    PATHS ${PYTHON_INC_DIR})
    
#TODO: Find Boost.Python for Unix/RPI
    
set(supergl_LINK_LIBRARIES
    ${assimp_LIB}
    ${DevIL_LIB}
    ${OpenGL_LIB}
    ${SDL2_LIB}
    ${SDL2main_LIB}
    ${Python_LIB}
    )
    
set(supergl_INCLUDE_DIRS
    ${Python_INC}
    ${CMAKE_PREFIX_PATH}/include)
    
if(WIN32)
    set(supergl_LINK_LIBRARIES
        ${supergl_LINK_LIBRARIES}
        opengl32
        )
endif(WIN32)

file(GLOB supergl_SOURCE_FILES
    ${supergl_SOURCE_DIR}/src/*.cpp)
    
file(GLOB supergl_HEADER_FILES
    ${supergl_SOURCE_DIR}/src/*.h)
    
if(WIN32)
    set(supergl_SOURCE_FILES ${supergl_SOURCE_FILES} ${supergl_HEADER_FILES})
	add_definitions("/MP")
endif(WIN32)

include_directories(${supergl_INCLUDE_DIRS})
link_directories("${CMAKE_PREFIX_PATH}/lib/${CMAKE_LIBRARY_ARCHITECTURE}") #For boost auto linking

add_library(supergl SHARED ${supergl_SOURCE_FILES})
target_link_libraries(supergl ${supergl_LINK_LIBRARIES})

set_target_properties(supergl PROPERTIES COTIRE_UNITY_SOURCE_MAXIMUM_NUMBER_OF_INCLUDES "-j")

if(WIN32)
    set_target_properties(supergl PROPERTIES SUFFIX ".pyd")
endif(WIN32)

cotire(supergl)