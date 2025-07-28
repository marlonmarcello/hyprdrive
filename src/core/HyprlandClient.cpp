#include "HyprlandClient.hpp"
#include "HyprlandPaths.hpp"
#include "nlohmann/json_fwd.hpp"
#include <expected>
#include <print>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

std::expected<nlohmann::json, std::string> HyprlandClient::getWorkspaces() {

    int sock_fd = HyprlandPaths::getCommandSocket();
    if (sock_fd == -1) {
        return std::unexpected("Client failed to connect");
    }

    const char* command = "j/workspaces";
    if (send(sock_fd, command, strlen(command), 0) == -1) {
        std::println(stderr, "[HyprlandClient] Error: failed to send command '{}'", command);
        close(sock_fd);
        return std::unexpected("Failed to send command");
    }

    std::string response;
    char        buffer[4096];
    ssize_t     bytes_received;

    while ((bytes_received = recv(sock_fd, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, bytes_received);
    }

    close(sock_fd);

    if (response.empty()) {
        std::println(stderr, "[HyprlandClient] Error: Received empty response from Hyprland");
        return std::unexpected("Received empty response");
    }

    return nlohmann::json::parse(response, nullptr, false);
}
