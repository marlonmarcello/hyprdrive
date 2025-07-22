cmake_minimum_required(VERSION 3.27)
project(hyprdrive)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(hyprdrive
    src/main.cpp
    src/core/HyprlandListener.cpp
    src/core/HyprlandClient.cpp
    src/services/WorkspaceService.cpp
)

target_link_libraries(hyprdrive PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(hyprdrive PUBLIC src)
