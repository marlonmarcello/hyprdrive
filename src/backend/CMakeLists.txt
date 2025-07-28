# --- Dependencies ---
find_package(Qt6 REQUIRED COMPONENTS Core)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

# --- Module ---
qt_add_library(hyprdrive_backend STATIC)
qt_add_qml_module(hyprdrive_backend
    URI hyprdrive.backend
    VERSION 1.0
    SOURCES
        core/HyprlandListener.hpp
        core/HyprlandListener.cpp
        core/HyprlandClient.hpp
        core/HyprlandClient.cpp
        core/HyprlandPaths.hpp
        services/WorkspaceService.hpp
        services/WorkspaceService.cpp
        services/WindowService.hpp
        services/WindowService.cpp
        services/DebugService.hpp
        services/DebugService.cpp
    RESOURCE_PREFIX "/"
)

target_link_libraries(hyprdrive_backend PRIVATE Qt::Core nlohmann_json::nlohmann_json)
target_include_directories(hyprdrive_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
