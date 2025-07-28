#pragma once
#include "../core/HyprlandListener.hpp"

class DebugService {
  public:
    DebugService(HyprlandListener& listener);
    ~DebugService();
    void onEvent(const HyprlandEvent& event);

  private:
    HyprlandListener& listener;
    SubscriptionId    subscription_id;
};
