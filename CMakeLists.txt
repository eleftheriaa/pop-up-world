###############################################################################
cmake_minimum_required(VERSION 3.0)
project(VVR_OGL_LABORATORY)

###############################################################################

find_package(OpenGL REQUIRED)

# c++11, -g option is used to export debug symbols for gdb
if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU OR
    ${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  # Using C++11 on OSX requires using libc++ instead of libstd++.
  # libc++ is an implementation of the C++ standard library for OSX.
  if(APPLE)
    if(XCODE)
      set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
      set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
    endif()
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
  endif()
endif()

# define library postfix so that different builds will produce
# distinguished libraries
set(CMAKE_RELEASE_POSTFIX "_r" CACHE string "Release postfix")
set(CMAKE_DEBUG_POSTFIX "_d" CACHE string "Debug postfix")
set(CMAKE_RELWITHDEBINFO_POSTFIX "_rd" CACHE string
  "Release with debug info postfix")
set(CMAKE_MINSIZEREL_POSTFIX "_mr" CACHE string
  "Minimum size release postfix")
mark_as_advanced(CMAKE_RELEASE_POSTFIX)
mark_as_advanced(CMAKE_DEBUG_POSTFIX)
mark_as_advanced(CMAKE_RELWITHDEBINFO_POSTFIX)
mark_as_advanced(CMAKE_MINSIZEREL_POSTFIX)

# ctest
include(CTest)
enable_testing()

# for rdm (emacs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# for grouping into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  message(FATAL_ERROR "Please select another Build Directory ! (and give it a
  clever name, like bin_Visual2012_64bits/)")
endif()
if(CMAKE_SOURCE_DIR MATCHES " ")
  message( "Your Source Directory contains spaces. If you experience problems
    when compiling, this can be the cause." )
endif()
if(CMAKE_BINARY_DIR MATCHES " ")
  message("Your Build Directory contains spaces. If you experience problems
  when compiling, this can be the cause.")
endif()

# Compile external dependencies
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP



########### Dear ImGui import code
set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
add_library(imgui 
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp
        ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp)
target_compile_definitions(imgui PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GLEW)
target_include_directories(imgui PUBLIC ${IMGUI_PATH}/ ${IMGUI_PATH}/backends/)

target_link_libraries(imgui GLEW_1130 glfw)



###############################################################################

include_directories(
  external/glfw-3.2.1/include/GLFW/
  external/glm-0.9.7.1/
  external/glew-1.13.0/include/
  external/Simple-OpenGL-Image-Library/include/
  external/tinyxml2/include/
  external/tinyobjloader/include/
  .
  )

set(ALL_LIBS
  ${OPENGL_LIBRARY}
  glfw
  GLEW_1130
  SOIL
  TINYXML2
  imgui
  )

add_definitions(
  -DTW_STATIC
  -DTW_NO_LIB_PRAGMA
  -DTW_NO_DIRECT3D
  -DGLEW_STATIC
  -D_CRT_SECURE_NO_WARNINGS
  )

###############################################################################
# lab07
add_executable(lab07
  lab07/lab.cpp
  lab07/RigidBody.cpp
  lab07/RigidBody.h
  lab07/Cube.cpp
  lab07/Cube.h
  lab07/Sphere.cpp
  lab07/Sphere.h
  lab07/Box.cpp
  lab07/Box.h
  lab07/MassSpringDamper.cpp
  lab07/MassSpringDamper.h
  lab07/Collision.cpp
  lab07/Collision.h
  lab07/Avatar.cpp
  lab07/Avatar.h
  lab07/Mushroom.cpp
  lab07/Mushroom.h
  lab07/ParticleEmitterInt.cpp
  lab07/ParticleEmitterInt.h
  lab07/FountainEmitter.cpp
  lab07/FountainEmitter.h



  common/util.cpp
  common/util.h
  common/shader.cpp
  common/shader.h
  common/camera.cpp
  common/camera.h
  common/model.cpp
  common/model.h
  common/texture.cpp
  common/texture.h
  common/light.cpp
  common/light.h
  common/stb_image.h
  common/animation.cpp
  common/animation.h

  lab07/StandardShading.fragmentshader
  lab07/StandardShading.vertexshader
  lab07/Depth.fragmentshader
  lab07/Depth.vertexshader
  lab07/SimpleTexture.fragmentshader
  lab07/SimpleTexture.vertexshader
  lab07/Skybox.fragmentshader
  lab07/Skybox.vertexshader
  lab07/ParticleShader.vertexshader
  lab07/ParticleShader.fragmentshader


  )
target_link_libraries(lab07
  ${ALL_LIBS}
  )
# Xcode and Visual working directories
set_target_properties(lab07
  PROPERTIES
  XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lab07/"
  PROJECT_LABEL "Lab 07 - Physics"
  FOLDER "Exercise"
  )
create_target_launcher(lab07 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lab07/")
create_default_target_launcher(lab07 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lab07/")

###############################################################################

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )
SOURCE_GROUP(obj REGULAR_EXPRESSION ".*/.*obj$" )

###############################################################################
# copy
if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

  add_custom_command(
    TARGET lab07 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lab07${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/lab07/"
    )

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
