#include "HyprlandClient.hpp"
#include "HyprlandPaths.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

nlohmann::json HyprlandClient::getWorkspaces() {
  auto socket_path = HyprlandPaths::getCommandSocketPath();
  if (!socket_path) {
    std::cerr << "Error: Required environment variables not set." << std::endl;
  }

  int sock_fd = HyprlandPaths::connectToSocket(*socket_path);
  if (sock_fd == -1) {
    std::cerr << "Error: client failed to connect" << std::endl;
    return nullptr;
  }

  const char* command = "hyprctl workspaces -j";
  if (send(sock_fd, command, strlen(command), 0) == -1) {
    std::cerr << "Error: failed to send command '" + std::string(command) + "'" << std::endl;
    return nullptr;
  }

  std::string response;
  char buffer[4096];
  ssize_t bytes_received;

  while ((bytes_received = recv(sock_fd, buffer, sizeof(buffer), 0)) > 0) {
    response.append(buffer, bytes_received);
  }

  close(sock_fd);

  return nlohmann::json::parse(response);
}