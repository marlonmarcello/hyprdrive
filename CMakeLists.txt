cmake_minimum_required(VERSION 3.27)
project(hyprdrive)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(hyprdrive
    src/main.cpp
    src/core/HyprlandListener.cpp
    src/core/HyprlandClient.cpp
    src/services/WorkspaceService.cpp
    src/services/DebugService.cpp
)

target_link_libraries(hyprdrive PRIVATE nlohmann_json::nlohmann_json)
target_compile_features(hyprdrive PUBLIC cxx_std_23)
target_include_directories(hyprdrive PUBLIC src)
