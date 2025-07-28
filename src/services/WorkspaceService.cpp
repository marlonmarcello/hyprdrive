#include "WorkspaceService.hpp"
#include "core/HyprlandClient.hpp"

#include <print>

WorkspaceService::WorkspaceService(HyprlandListener& listener) : listener(listener), subscription_id(0) {
    this->subscription_id = this->listener.subscribe([this](const HyprlandEvent& event) { this->onEvent(event); });

    fetchInitialWorkspaces();
}

WorkspaceService::~WorkspaceService() {
    this->listener.unsubscribe(this->subscription_id);
}

void WorkspaceService::onEvent(const HyprlandEvent& event) {
    if (event.type == "workspace") {
        this->current_workspace = event.data;
        std::println("[WorkspaceService] State updated -> {}", this->current_workspace);
    }
    else if (event.type == "createworkspace") {
        int new_id = all_workspaces.size() + 1;
        all_workspaces.push_back(Workspace{.id = new_id, .name = event.data});
        std::println("[WorkspaceService] Workspace created: {0}. Total: {1}", event.data, all_workspaces.size());
    }
    else if (event.type == "destroyworkspace") {
        std::string destroyed_name = event.data;
        std::erase_if(all_workspaces, [&](const Workspace& ws) { return ws.name == destroyed_name; });
        std::println("[WorkspaceService] Workspace destroyed: {0}. Total: {1}", destroyed_name, all_workspaces.size());
    }
}

void WorkspaceService::fetchInitialWorkspaces() {
    std::println("[WorkspaceService] Fetching initial workspace state...");

    auto workspaces = HyprlandClient::getWorkspaces();

    if (!workspaces.has_value()) {
        std::println(stderr, "[WorkspaceService] Error: {}", workspaces.error());
        return;
    }

    if (workspaces->is_array()) {
        for (const auto& item : *workspaces) {
            all_workspaces.push_back(Workspace{.id = item["id"], .name = item["name"]});
        }
    }

    std::println(stderr, "[WorkspaceService] Inital state loaded. Workspaces: ");
    for (const auto& ws : all_workspaces) {
        std::println("ID: {0} | Name: {1}", ws.id, ws.name);
    }
}
