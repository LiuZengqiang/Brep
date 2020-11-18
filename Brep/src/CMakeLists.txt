message("----BEGIN SCENE AND SWEEPER----")

file(GLOB_RECURSE SWEEPER_HEADERS sweeper/*.h json/*.h)
file(GLOB_RECURSE SWEEPER_SOURCES sweeper/*.cpp json/*.cpp)
add_library(sweeper_library STATIC ${SWEEPER_HEADERS} ${SWEEPER_SOURCES})
# target_include_directories(sweeper_library PUBLIC ${PROJECT_SOURCE_DIR}/Brep/include)

file(GLOB_RECURSE SCENE_HEADERS scene/*.h datastruct/*.h)
file(GLOB_RECURSE SCENE_SOURCES scene/*.cpp datastruct/*.cpp)
add_library(scene_library STATIC ${SCENE_HEADERS} ${SCENE_SOURCES})

message("----END SCENE AND SWEEPER----")