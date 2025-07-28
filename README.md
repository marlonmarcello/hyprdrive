# Hyprdrive

> [!WARNING]
> Still a work in progress

A custom shell tailored specifically to Hyprland.

## Prerequisites

  * **[cmake](https://cmake.org/download/)** (3.21 or newer)
  * A C++ compiler that supports C++ 23
  * [Slint](https://docs.slint.dev/latest/docs/cpp/cmake#install-slint)
  * [hyprwayland-scanner](https://github.com/hyprwm/hyprwayland-scanner)

## Project Overview

Hyprdrive is a custom desktop shell environment being built for the Hyprland Wayland compositor. The project's goal is to create a functional and modular shell by developing a series of independent "services" that communicate with the underlying system and window manager. The project is being developed in C++ with a focus on modern practices, robust architecture, and a deep understanding of systems programming concepts.

## Goals and Objectives

**Primary Goal:** Develop a complete, GNOME-like desktop shell with custom components (panel, control center, etc.).

**Architectural Goal:** Build a highly modular system where features are encapsulated in services. This will be achieved by using the Observer (Pub/Sub) design pattern, with a central event broadcaster and multiple subscriber services.

**Technical Goal:** Utilize modern C++ (C++23 and newer) and standard tooling (CMake) to create a robust and maintainable codebase.

**Learning Goal:** Ultimately, this will serve as a hands-on project to learn low-level systems programming, C++ best practices, API interaction, and more.

## Current Status & Accomplishments

The project is currently a functional, command-line-based event processing engine.

### Module 1: C++ Environment & Build System

- A complete C++ development toolchain (g++, CMake, GDB) has been established.
- A scalable project structure using src/core and src/services directories is in place.
- Dependency management has been implemented using CMake's FetchContent module to integrate the nlohmann/json library.

### Module 2: Core IPC & Event Broadcasting

- A reusable `HyprlandListener` class has been implemented. This class acts as the central Subject in our Observer pattern.
- The listener connects to the Hyprland event socket, reads the data stream robustly, parses events into a structured `HyprlandEvent` object, and dispatches these events to all subscribers.
- The architecture supports multiple subscribers through a subscribe/unsubscribe mechanism using std::map and std::function for callbacks.

### Module 3: Service Implementation

- The first feature, `WorkspaceService`, has been implemented. It subscribes to the listener and maintains the state of the currently active workspace.
- A `DebugService` has been created to demonstrate the flexibility of the architecture, allowing for raw logging of all events for debugging purposes without polluting feature-specific services.

## Future Roadmap

The following modules are planned to build upon the current foundation.

### Phase 1: Complete Core Data Services

- Enhance `WorkspaceService`: The immediate next step. We will implement the `HyprlandClient` to make an initial query to the Hyprland command socket to get the full list of available workspaces. This will involve:
- Implementing a `HyprlandClient` class to send one-off commands.
- Parsing the JSON response from Hyprland.
- Storing the full workspace state (list of all workspaces + the active one).
- Keeping the list updated via `createworkspace` and `destroyworkspace` events.
- Create `WindowService`: A new service that subscribes to activewindowv2 and windowtitle events to track the currently focused window and its title.

### Phase 2: GUI Integration

- Integrate a GUI Toolkit~, most likely QT as that's what Hyprland is using~. QT and GTK are awful, I really tried. Setting up QT was a nbightmare of obscure cmake flags and functions. GTK was promising, I managed to get a hello world relatively easy but working with XML is just a terrible experience. Ended up using Slint, love the reactivity and how easy it was to integrate with CMake and C++.
- Build a Panel (The "Bar"): Create the first UI component. This will be a top-level window configured as a Wayland layer surface.
- Connect Services to UI: The UI panel will instantiate and use the services. For example, it will use the WorkspaceService to get the list of workspaces and render them as clickable buttons. The onEvent callbacks in our services will be modified to emit signals that the UI can react to, updating the display in real-time.

### Phase 3: Control Center and System Services

- Implement a D-Bus Listener: Create a new "core" listener for the D-Bus system bus.
- Develop System Services: Build new services for networking, Bluetooth, audio, etc., that subscribe to the `DBusListener`.
- Build Control Center UI: Create a new UI application for the control center that uses these system services to display information and send commands back to D-Bus.

### Phase 4: High-Level Shell CLI

- Implement Command-Line Parsing: Parse arguments like `toggleCommandCenter` or `setVolume`.
- Create an Internal Command Bus: Develop a mechanism for the CLI to communicate with the running shell components and services to trigger actions.
