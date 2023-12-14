cmake_minimum_required(VERSION 3.6.0)

project(LightWaveSim VERSION 0.1.0)
file(GLOB source_files
    "src/*.cpp"
    "src/managers/*.cpp"
    "src/application/*.cpp"
    "src/render/*.cpp"
    "src/states/*.cpp"
    "src/render_algorithm/*.cpp"
    "src/scene/*.cpp"
    "src/serialisation/*.cpp"
    "src/camera/*.cpp"
    "src/platform/OpenGL/*.cpp"
    "src/scene/objects/*.cpp"
    "src/ui/*.cpp")

add_subdirectory(lib/glad)
add_subdirectory(lib/imgui)
add_subdirectory(lib/glm)
add_subdirectory(lib/ImGuiFileDialog)


add_executable(app ${source_files})

set_property(TARGET app PROPERTY CXX_STANDARD 20)

target_include_directories(app PUBLIC "src/")

target_link_libraries(app glfw)
target_link_libraries(app glad)
target_link_libraries(app imgui)
target_link_libraries(app glm)
target_link_libraries(app ImGuiFileDialog)