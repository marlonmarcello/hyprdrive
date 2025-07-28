#pragma once

#include <expected>
#include <nlohmann/json.hpp>

class HyprlandClient {
  public:
    static std::expected<nlohmann::json, std::string> getWorkspaces();
};
