cmake_minimum_required(VERSION 3.10)

project(SoundBoardPlusPlus)

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(SoundBoardPlusPlus SBPP.cpp)

target_link_libraries(SoundBoardPlusPlus PRIVATE imgui::imgui glfw OpenGL::GL)
