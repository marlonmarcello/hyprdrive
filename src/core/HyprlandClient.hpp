#pragma once

#include <string>
#include <nlohmann/json.hpp>

class HyprlandClient {
  public:
    static nlohmann::json getWorkspaces();
};