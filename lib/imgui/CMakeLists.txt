set(CMAKE_CXX_STANDARD 20)

file(GLOB imgui_src
    "imgui*.cpp"
    "backends/imgui_impl_glfw.cpp"
    "backends/imgui_impl_opengl3.cpp"
)

add_library(imgui ${imgui_src})
target_include_directories(imgui PUBLIC "./backends/" "./")

target_link_libraries(imgui glfw)
