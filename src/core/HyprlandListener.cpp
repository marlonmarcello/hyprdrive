#include "HyprlandListener.hpp"
#include "HyprlandPaths.hpp"
#include <print>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

HyprlandListener::HyprlandListener() : socket_fd(-1) {}

HyprlandListener::~HyprlandListener() {
    if (socket_fd != -1) {
        std::println("[HyprlandListener] Closing socket");
        close(socket_fd);
    }
}

SubscriptionId HyprlandListener::subscribe(EventCallback callback) {
    SubscriptionId id = next_id++;
    subscribers[id] = callback;
    return id;
}

void HyprlandListener::unsubscribe(SubscriptionId id) { subscribers.erase(id); }

void HyprlandListener::start() {
    socket_fd = HyprlandPaths::getEventSocket();
    if (socket_fd == -1) {
        return;
    }

    std::println(
        "[HyprlandListener] Successfully connected! Listening for events...");

    std::string buffer_str;
    char raw_buffer[4096];
    while (true) {
        ssize_t bytes = recv(socket_fd, raw_buffer, sizeof(raw_buffer), 0);
        if (bytes <= 0) {
            std::println(stderr,
                         "[HyprlandListener] Connection closed or error.");
            break;
        }
        buffer_str.append(raw_buffer, bytes);

        size_t newline_pos;
        while ((newline_pos = buffer_str.find('\n')) != std::string::npos) {
            std::string message = buffer_str.substr(0, newline_pos);
            buffer_str.erase(0, newline_pos + 1);

            HyprlandEvent event;
            size_t delim_pos = message.find(">>");

            if (delim_pos != std::string::npos) {
                event.type = message.substr(0, delim_pos);
                event.data = message.substr(delim_pos + 2);
            } else {
                // Temporary fallback for now
                event.type = message;
            }

            for (const auto& [id, callback] : subscribers) {
                callback(event);
            }
        }
    }
}
