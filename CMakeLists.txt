cmake_minimum_required (VERSION 3.8)

project ("LearnOpenGL")

include_directories("Thirdparty/include")
link_directories("Thirdparty/bin")
link_libraries(opengl32 glfw3)

add_executable (LearnOpenGL "Thirdparty/src/stb_image.cpp" "Thirdparty/src/glad.c" "Source/Main.cpp")

file(GLOB RESOURCE_FILES "Resource/*")
file(COPY ${RESOURCE_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})