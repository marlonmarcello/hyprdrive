#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <string>

struct HyprlandEvent {
    std::string type;
    std::string data;
};
using SubscriptionId = size_t;
using EventCallback  = std::function<void(const HyprlandEvent&)>;

class HyprlandListener {
  public:
    HyprlandListener();
    ~HyprlandListener();

    void           start();
    void           stop();

    SubscriptionId subscribe(EventCallback callback);
    void           unsubscribe(SubscriptionId id);

  private:
    std::atomic<bool>                       is_running{true};
    int                                     socket_fd;
    SubscriptionId                          next_id = 0;

    std::map<SubscriptionId, EventCallback> subscribers;
};
