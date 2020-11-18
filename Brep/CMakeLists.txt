# CMakeList.txt: Brep 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

message("----BEGIN PROJECT----")
message(${PROJECT_SOURCE_DIR})


include_directories(${PROJECT_SOURCE_DIR}/Brep/include)

include_directories(${PROJECT_SOURCE_DIR}/Brep/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/Brep/src)
# 将源代码添加到此项目的可执行文件。
add_executable(Brep "main.cpp")
target_link_libraries(Brep PRIVATE ${PROJECT_SOURCE_DIR}/Brep/lib/freeglut.lib sweeper_library scene_library)
message("----END PROJECT----")
