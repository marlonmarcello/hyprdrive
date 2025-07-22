#pragma once

#include "../core/HyprlandListener.hpp"
#include <string>

class WorkspaceService {
public:
    WorkspaceService(HyprlandListener& listener);
    ~WorkspaceService();

    void onEvent(const HyprlandEvent& event);

private:
    HyprlandListener& listener;
    SubscriptionId subscription_id;
    std::string current_workspace;
};

