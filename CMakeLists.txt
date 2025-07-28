cmake_minimum_required(VERSION 3.27)
project(hyprdrive)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

# --- QT Setup ---
find_package(Qt6 6.9 REQUIRED COMPONENTS Gui Qml Quick WaylandClient)
qt_standard_project_setup(REQUIRES 6.9)

# --- Modules ---
add_subdirectory(src/backend)
add_subdirectory(src/ui)

# --- Executable ---
# Uses the specialized qt_add_executable command.
qt_add_executable(hyprdrive
    src/main.cpp
)
target_compile_features(hyprdrive PUBLIC cxx_std_23)

# --- Link Libraries ---
target_link_libraries(hyprdrive PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::WaylandClient
    hyprdrive_ui
    hyprdrive_backend
)

# --- Includes ---
target_include_directories(hyprdrive PUBLIC src)
