-- Set the project name
set_project("hyprdrive")

-- Set the minimum required XMake version
set_xmakever("2.7.0")

-- Set C++ standard to C++23
set_languages("c++23")

-- Enable compile commands output (like CMake's `CMAKE_EXPORT_COMPILE_COMMANDS`)
set_config("ccache", true) -- optional, for faster builds
add_rules("mode.debug", "mode.release")

-- Add remote package: nlohmann_json
add_requires("nlohmann_json 3.12.0")

-- Define the target executable
target("hyprdrive")
set_kind("binary")

-- Add source files
add_files(
    "src/main.cpp",
    "src/**/*.cpp"
)

-- Include directory
add_includedirs("src")

-- Link with nlohmann_json
add_packages("nlohmann_json")
