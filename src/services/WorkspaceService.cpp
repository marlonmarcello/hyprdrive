#include "WorkspaceService.hpp"
#include "core/HyprlandClient.hpp"
#include "nlohmann/json_fwd.hpp"
#include <functional>
#include <iostream>

WorkspaceService::WorkspaceService(HyprlandListener& listener)
    : listener(listener), subscription_id(0) {

    auto callback =
        std::bind(&WorkspaceService::onEvent, this, std::placeholders::_1);
    this->subscription_id = this->listener.subscribe(callback);

    fetchInitialWorkspaces();
}

WorkspaceService::~WorkspaceService() {
    this->listener.unsubscribe(this->subscription_id);
}

void WorkspaceService::onEvent(const HyprlandEvent& event) {
    if (event.type == "workspace") {
        this->current_workspace = event.data;
        std::cout << "[WorkspaceService] State updated -> "
                  << this->current_workspace << std::endl;
    }

    // TODO: implement create and destroy
}

void WorkspaceService::fetchInitialWorkspaces() {
    std::cout << "[WorkspaceService] Fetching initial workspace state..."
              << std::endl;

    auto workspaces_json_opt = HyprlandClient::getWorkspaces();

    if (!workspaces_json_opt) {
        std::cerr << "[WorkspaceService] Failed to fetch workspaces."
                  << std::endl;
        return;
    }

    nlohmann::json workspaces_json = *workspaces_json_opt;

    if (workspaces_json.is_array()) {
        for (const auto& item : workspaces_json) {
            Workspace ws;
            ws.id = item["id"];
            ws.name = item["name"];
            all_workspaces.push_back(ws);
        }
    }

    std::cout << "[WorkspaceService] Inital state loaded. Workspaces: "
              << std::endl;
    for (const auto& ws : all_workspaces) {
        std::cout << "ID: " << ws.id << ", Name: " << ws.name << std::endl;
    }
}
