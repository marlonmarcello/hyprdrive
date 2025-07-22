#include "WorkspaceService.hpp"
#include <iostream>
#include <functional>

 WorkspaceService::WorkspaceService(HyprlandListener& listener) : listener(listener), subscription_id(0) {
     auto callback = std::bind(&WorkspaceService::onEvent, this, std::placeholders::_1);
     this->subscription_id = this->listener.subscribe(callback);
 }

WorkspaceService::~WorkspaceService() {
    this->listener.unsubscribe(this->subscription_id);
}

void WorkspaceService::onEvent(const HyprlandEvent& event) {
    if (event.type == "workspace") {
        this->current_workspace = event.data;
        std::cout << "[WorkspaceService] State updated -> " << this->current_workspace << std::endl;
    }
}
