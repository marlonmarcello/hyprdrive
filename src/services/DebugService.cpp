#include "DebugService.hpp"
#include <print>

DebugService::DebugService(HyprlandListener& listener) : listener(listener) {
    this->subscription_id = this->listener.subscribe([this](const HyprlandEvent& event) { this->onEvent(event); });
}

DebugService::~DebugService() {
    this->listener.unsubscribe(this->subscription_id);
}

void DebugService::onEvent(const HyprlandEvent& event) {
    // This service's only job is to log everything.
    std::println("[DEBUG] Event: {0} | Data: {1}", event.type, event.data);
}
