#pragma once

#include "core/HyprlandListener.hpp"
#include <string>

struct Window {
    std::string w_class;
    std::string title;
};

class WindowService {
  public:
    WindowService(HyprlandListener& listener);
    ~WindowService();

    void onEvent(const HyprlandEvent& event);

  private:
    HyprlandListener& listener;
    SubscriptionId    subscription_id;

    Window            current_window;
};
