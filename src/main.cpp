#include "core/HyprlandListener.hpp"
#include "services/DebugService.hpp"
#include "services/WindowService.hpp"
#include "services/WorkspaceService.hpp"
#include "ui-main.h"
#include <thread>

int main() {
    // Listener and services
    HyprlandListener listener;
    WorkspaceService workspace_service(listener);
    WindowService    window_service(listener);
    DebugService     debug_service(listener);

    // UI
    auto ui = AppWindow::create();

    // Thread for listener
    std::thread listener_thread([&listener]() { listener.start(); });

    // UI runs on main  thread
    ui->run();

    // Cleanup
    listener.stop();
    listener_thread.join();

    return 0;
}
