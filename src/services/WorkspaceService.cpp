#include "WorkspaceService.hpp"
#include "core/HyprlandClient.hpp"
#include "nlohmann/json_fwd.hpp"
#include <functional>
#include <print>

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
        std::println("[WorkspaceService] State updated -> {}",
                     this->current_workspace);
    }

    // TODO: implement create and destroy
}

void WorkspaceService::fetchInitialWorkspaces() {
    std::println("[WorkspaceService] Fetching initial workspace state...");

    auto workspaces = HyprlandClient::getWorkspaces();

    if (!workspaces.has_value()) {
        std::println(stderr, "[WorkspaceService] Error: {}",
                     workspaces.error());
        return;
    }

    if (workspaces->is_array()) {
        for (const auto& item : *workspaces) {
            all_workspaces.push_back(
                Workspace{.id = item["id"], .name = item["name"]});
        }
    }

    std::println(stderr,
                 "[WorkspaceService] Inital state loaded. Workspaces: ");
    for (const auto& ws : all_workspaces) {
        std::println("ID: {0} | Name: {1}", ws.id, ws.name);
    }
}
