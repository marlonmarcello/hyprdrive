#pragma once

#include "../core/HyprlandListener.hpp"
#include <qobject.h>
#include <qtmetamacros.h>
#include <string>

struct Workspace {
    int         id;
    std::string name;
};

class WorkspaceService : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(std::string activeWorkspace READ activeWorkspace NOTIFY activeWorkspaceChanged)

  public:
    WorkspaceService(HyprlandListener& listener);
    ~WorkspaceService();

    void onEvent(const HyprlandEvent& event);

  private:
    HyprlandListener&      listener;
    SubscriptionId         subscription_id;

    std::string            current_workspace;
    std::vector<Workspace> all_workspaces;

    void                   fetchInitialWorkspaces();
};
