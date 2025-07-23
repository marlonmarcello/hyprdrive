#include "HyprlandClient.hpp"
#include "HyprlandPaths.hpp"
#include "nlohmann/json_fwd.hpp"
#include <iostream>
#include <optional>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

std::optional<nlohmann::json> HyprlandClient::getWorkspaces() {

    int sock_fd = HyprlandPaths::getCommandSocket();
    if (sock_fd == -1) {
        return std::nullopt;
    }

    const char* command = "j/workspaces";
    if (send(sock_fd, command, strlen(command), 0) == -1) {
        std::cerr << "[HyprlandClient] Error: failed to send command '" +
                         std::string(command) + "'"
                  << std::endl;
        close(sock_fd);
        return std::nullopt;
    }

    std::string response;
    char buffer[4096];
    ssize_t bytes_received;

    while ((bytes_received = recv(sock_fd, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, bytes_received);
    }

    close(sock_fd);

    if (response.empty()) {
        std::cerr
            << "[HyprlandClient] Error: Received empty response from Hyprland."
            << std::endl;
        return std::nullopt;
    }

    return nlohmann::json::parse(response, nullptr, false);
}
