#include "core/HyprlandListener.hpp"
#include "services/DebugService.hpp"
#include "services/WorkspaceService.hpp"

int main() {
    HyprlandListener listener;

    WorkspaceService workspace_service(listener);

    DebugService debug_service(listener);

    listener.start();

    return 0;
}
