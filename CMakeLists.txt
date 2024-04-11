cmake_minimum_required(VERSION 3.12)

project(ViperEngine)
set(CMAKE_CXX_STANDARD 20)

add_executable(ViperEngine "main.cpp" "externals.h" "externals.cpp" 
"scripts/camera/camera.h" "scripts/camera/camera.cpp" 
"scripts/light/phong/PhongEngine.h" "scripts/light/phong/PhongEngine.cpp" 
"scripts/configs/configs.h" 
"scripts/utils/utils.h" "scripts/utils/utils.cpp" 
"scripts/ScreenSizeMode/screenSizeMode.h" "scripts/ScreenSizeMode/screenSizeMode.cpp"
"scripts/appTime/appTime.cpp" "scripts/appTime/appTime.h" 
"scripts/noise/VPEnoise.h" "scripts/noise/VPEnoise.cpp" "scripts/GLMVecToString/GLMVecToString.h" "scripts/getNull/getNull.h" "scripts/vec/VPEvec.h")

target_include_directories(ViperEngine PRIVATE deps/glm 
                                              deps/glfw/include
                                              deps/glew/include
                                              deps/irrKlang/include)

target_link_directories(ViperEngine PRIVATE deps/glfw/lib-vc2019)
target_link_directories(ViperEngine PRIVATE deps/glew/lib/Release/x64)
target_link_directories(ViperEngine PRIVATE deps/irrKlang/lib/Winx64-visualStudio)
                                           
target_link_libraries(ViperEngine PRIVATE glfw3.lib glew32.lib opengl32.lib irrKlang.lib)

add_custom_command(TARGET ViperEngine POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/deps/irrKlang/bin/winx64-visualStudio/irrKlang.dll" "${CMAKE_BINARY_DIR}/irrKlang.dll"
                   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/deps/glew/bin/Release/x64/glew32.dll" "${CMAKE_BINARY_DIR}/glew32.dll"
                   COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders"
                   COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/textures" "${CMAKE_BINARY_DIR}/textures"
                   COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/scripts" "${CMAKE_BINARY_DIR}/scripts"
                   COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/mesh" "${CMAKE_BINARY_DIR}/mesh"
                   COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/sounds" "${CMAKE_BINARY_DIR}/sounds")
