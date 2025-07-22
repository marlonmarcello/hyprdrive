#pragma once

#include <string>
#include <optional>
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace HyprlandPaths {
  inline std::optional<std::string> getSocketPath(const std::string& socketName) {
    const char* xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
    const char* signature = getenv("HYPRLAND_INSTANCE_SIGNATURE");

    if (!xdg_runtime_dir || !signature) {
      return std::nullopt;
    }

    return std::string(xdg_runtime_dir) + "/hypr/" + std::string(signature) + socketName;
  }

  inline std::optional<std::string> getEventSocketPath() {
    return getSocketPath(".socket2.sock");
  }

  inline std::optional<std::string> getCommandSocketPath() {
    return getSocketPath(".socket.sock");
  }

  inline int connectToSocket(const std::string& socketPath) {
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd == -1) {
      std::cerr << "Error: failed to create socket." << std::endl;
      return -1;
    }

    sockaddr_un server_addr{};
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, socketPath.c_str(), sizeof(server_addr.sun_path) - 1);

    if (connect(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        close(sock_fd);
        return -1;
    }

    return sock_fd;
  }
}