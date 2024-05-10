cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

include(vcpkg.cmake) # needs to happen before project()

project(
    "qut-gcs" # replace this with the name of your executable
    VERSION 0.1.0
    LANGUAGES CXX
)

# add all headers (.h, .hpp) to this
set(PRJ_HEADERS 
    src
)

# add all source files (.cpp) to this, except the one with main()
set(PRJ_SOURCES 
    src/rendering/mesh.cpp
    src/rendering/camera.cpp
    src/rendering/object.cpp
    src/implot/implot_demo.cpp
    src/implot/implot_items.cpp
    src/implot/implot.cpp
)

# set the source file containing main()
set(PRJ_MAIN 
    src/main.cpp
)

# add library targets here
set(PRJ_LIBRARIES 
    fmt::fmt
    imgui::imgui
    OpenGL::GL
    glm::glm
    unofficial::gl3w::gl3w
)

# add dependency find_package calls and similar here
find_package(fmt CONFIG REQUIRED)
find_package(imgui REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} ${PRJ_HEADERS} ${PRJ_SOURCES} ${PRJ_MAIN})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PRJ_LIBRARIES})

# Copy shaders to build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory              
        ${CMAKE_SOURCE_DIR}/src/shaders shaders
    COMMENT "Copying shaders" VERBATIM
)
