project(SortingVisualizer)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE src SortingVisualizer/*.cpp)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

add_executable(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME} OpenGL32 glm glfw3 freetype portaudio glew32 glew32s)