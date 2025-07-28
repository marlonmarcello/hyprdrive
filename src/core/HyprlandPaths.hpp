#pragma once

#include <cstdlib>
#include <print>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace HyprlandPaths {

    inline int getSocket(const std::string& socketName) {
        const char* xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
        const char* signature       = getenv("HYPRLAND_INSTANCE_SIGNATURE");

        if (!xdg_runtime_dir || !signature) {
            std::println(stderr, "[HyprlandPaths] Error: Required environment variables not set");
            return -1;
        }

        int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock_fd == -1) {
            std::println(stderr, "[HyprlandPaths] Error: failed to create socket");
            return -1;
        }

        std::string socket_path = std::string(xdg_runtime_dir) + "/hypr/" + std::string(signature) + "/" + socketName;

        sockaddr_un server_addr{};
        server_addr.sun_family = AF_UNIX;
        strncpy(server_addr.sun_path, socket_path.c_str(), sizeof(server_addr.sun_path) - 1);

        if (connect(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            std::println(stderr, "[HyprlandPaths] Error: failed to connect to socket {}", socket_path);
            close(sock_fd);
            return -1;
        }

        return sock_fd;
    }

    inline int getEventSocket() {
        return getSocket(".socket2.sock");
    }

    inline int getCommandSocket() {
        return getSocket(".socket.sock");
    }

} // namespace HyprlandPaths
