cmake_minimum_required(VERSION 3.27)
project(hyprdrive)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

# --- Wayland Protocol Generation ---
set(PROTOCOL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protocols)
set(PROTOCOL_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/protocols)

# Layer shell
set(PROTOCOL_LAYER_SHELL ${PROTOCOL_DIR}/wlr-layer-shell-unstable-v1.xml)
set(PROTOCOL_LAYER_SHELL_HEADER ${PROTOCOL_GENERATED_DIR}/wlr-layer-shell-unstable-v1.hpp)
add_custom_command(
    OUTPUT ${PROTOCOL_LAYER_SHELL_HEADER}
    COMMAND hyprwayland-scanner --client ${PROTOCOL_LAYER_SHELL} ${PROTOCOL_GENERATED_DIR}
    DEPENDS ${PROTOCOL_LAYER_SHELL}
    COMMENT "Generating C++ header for wlr-layer-shell"
)

set(PROTOCOL_FOREIGN_MANAGEMENT ${PROTOCOL_DIR}/wlr-foreign-toplevel-management-unstable-v1.xml)
set(PROTOCOL_FOREIGN_MANAGEMENT_HEADER ${PROTOCOL_GENERATED_DIR}/wlr-foreign-toplevel-management-unstable-v1.hpp)

add_custom_command(
    OUTPUT ${PROTOCOL_FOREIGN_MANAGEMENT_HEADER}
    COMMAND hyprwayland-scanner --client ${PROTOCOL_FOREIGN_MANAGEMENT} ${PROTOCOL_GENERATED_DIR}
    DEPENDS ${PROTOCOL_FOREIGN_MANAGEMENT}
    COMMENT "Generating C++ header for wlr-foreign-toplevel-management"
)

add_custom_target(GenerateWaylandProtocols DEPENDS ${PROTOCOL_LAYER_SHELL_HEADER} ${PROTOCOL_FOREIGN_MANAGEMENT_HEADER})

# --- Dependencies ---
include(FetchContent)

# Slint
find_package(Slint QUIET)
if (NOT Slint_FOUND)
    message("Slint could not be located in the CMake module search path. Downloading it from Git and building it locally")

    # Wayland only
    set(SLINT_FEATURE_BACKEND_WINIT OFF CACHE BOOL "" FORCE)
    set(SLINT_FEATURE_BACKEND_WINIT_WAYLAND ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        Slint
        GIT_REPOSITORY https://github.com/slint-ui/slint.git
        # `release/1` will auto-upgrade to the latest Slint >= 1.0.0 and < 2.0.0
        # `release/1.0` will auto-upgrade to the latest Slint >= 1.0.0 and < 1.1.0
        GIT_TAG release/1
        SOURCE_SUBDIR api/cpp
    )
    FetchContent_MakeAvailable(Slint)
endif (NOT Slint_FOUND)

# nlohmann_json
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

# --- Executable ---
# Uses the specialized qt_add_executable command.
add_executable(hyprdrive
    src/main.cpp
    src/core/HyprlandListener.hpp
    src/core/HyprlandListener.cpp
    src/core/HyprlandClient.hpp
    src/core/HyprlandClient.cpp
    src/core/HyprlandPaths.hpp
    src/services/WorkspaceService.hpp
    src/services/WorkspaceService.cpp
    src/services/WindowService.hpp
    src/services/WindowService.cpp
    src/services/DebugService.hpp
    src/services/DebugService.cpp
)
target_compile_features(hyprdrive PUBLIC cxx_std_23)
add_dependencies(hyprdrive GenerateWaylandProtocols)

# --- Link Libraries ---
target_link_libraries(hyprdrive PRIVATE nlohmann_json::nlohmann_json Slint::Slint)

# --- UI sources ---
slint_target_sources(hyprdrive src/ui/ui-main.slint)

# --- Includes ---
target_include_directories(hyprdrive PUBLIC src)
target_include_directories(hyprdrive PUBLIC
    src
    ${PROTOCOL_GENERATED_DIR}
)

# On Windows, copy the Slint DLL next to the application binary so that it's found.
if (WIN32)
    add_custom_command(TARGET my_application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:my_application> $<TARGET_FILE_DIR:my_application> COMMAND_EXPAND_LISTS)
endif()
