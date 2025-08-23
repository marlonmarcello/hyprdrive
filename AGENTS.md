## Course Syllabus: Building hyprdrive in Rust

### Module 1: Rust Foundations & Project Setup

1.1: Installing the Rust toolchain (rustup) and creating our project with Cargo.

1.2: Introduction to Rust Syntax: variables, functions, and control flow.

1.3: The Ownership Model: Understanding Rust's core principles of ownership, borrowing, and lifetimes.

### Module 2: Interacting with Hyprland (IPC)

2.1: Asynchronous Rust

2.2: Connecting to the Hyprland event socket

2.3: Building a robust, asynchronous event listener.

2.4: Implementing the Observer Pattern in idiomatic Rust.

### Module 3: State Management & Services

3.1: Defining data with structs and enums.

3.2: Implementing the WorkspaceService to manage state.

3.3: Safe Concurrency: Using Arc and Mutex to share state safely between threads.

### Module 4: Building the UI with Slint

4.1: Integrating the Slint UI toolkit with Cargo.

4.2: Designing the panel UI in the .slint markup language.

4.3: Creating the data binding bridge between our Rust services and the Slint UI.

### Module 5: Creating the Shell Panel

5.1: Interfacing with Wayland using our services.

5.2: Implementing the wlr-layer-shell protocol to anchor our window to the screen.
