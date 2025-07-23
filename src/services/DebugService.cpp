#include "DebugService.hpp"
#include <functional>
#include <iostream>

DebugService::DebugService(HyprlandListener& listener) : listener(listener) {
    auto callback =
        std::bind(&DebugService::onEvent, this, std::placeholders::_1);
    this->subscription_id = this->listener.subscribe(callback);
}

DebugService::~DebugService() {
    this->listener.unsubscribe(this->subscription_id);
}

void DebugService::onEvent(const HyprlandEvent& event) {
    // This service's only job is to log everything.
    std::cout << "[DEBUG] Event: " << event.type << ", Data: " << event.data
              << std::endl;
}
