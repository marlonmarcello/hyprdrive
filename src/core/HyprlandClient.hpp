#pragma once

#include <nlohmann/json.hpp>
#include <optional>

class HyprlandClient {
  public:
    static std::optional<nlohmann::json> getWorkspaces();
};
