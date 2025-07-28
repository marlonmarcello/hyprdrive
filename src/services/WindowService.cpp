#include "services/WindowService.hpp"
#include "core/HyprlandListener.hpp"
#include <cstddef>
#include <print>
#include <string>

WindowService::WindowService(HyprlandListener& listener) : listener(listener) {
    this->subscription_id = this->listener.subscribe([this](const HyprlandEvent& event) { this->onEvent(event); });
}

WindowService::~WindowService() {
    this->listener.unsubscribe(this->subscription_id);
}

void WindowService::onEvent(const HyprlandEvent& event) {
    if (event.type == "activewindow") {
        size_t comma_pos = event.data.find(',');

        if (comma_pos != std::string::npos) {
            this->current_window.w_class = event.data.substr(0, comma_pos);
            this->current_window.title   = event.data.substr(comma_pos + 1);
        }
        else {
            this->current_window.w_class = event.data;
            this->current_window.title   = "";
        }

        std::println("[WindowService] Active window -> Class: {0} | Title: {1} ", this->current_window.w_class, this->current_window.title);
    }
}
